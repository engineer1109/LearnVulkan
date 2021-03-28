package com.engineer1109.learnvulkan.viewmodel;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.view.View;

import androidx.lifecycle.ViewModel;
import androidx.recyclerview.widget.LinearLayoutManager;

import com.engineer1109.learnvulkan.R;
import com.engineer1109.learnvulkan.activities.BaseRenderActivity;
import com.engineer1109.learnvulkan.adapters.MenuAdapter;
import com.engineer1109.learnvulkan.databinding.ActivityStartBinding;
import com.engineer1109.learnvulkan.itemdecoration.VerticalRecyclerItemDecoration;
import com.engineer1109.learnvulkan.model.MenuData;

import java.util.ArrayList;
import java.util.List;

public class MainViewModel extends ViewModel {

    static final String TAG = "MainViewModel";

    private ActivityStartBinding mBinding = null;

    public ActivityStartBinding getBinding() {
        return mBinding;
    }

    public void setBinding(ActivityStartBinding mBinding) {
        this.mBinding = mBinding;
    }

    public Context getContext() {
        return mBinding.getRoot().getContext();
    }

    @SuppressLint("ResourceAsColor")
    public void showMenu(View view) {
        List<MenuData> datas = this.generateMenus();
        Context context = view.getContext();
        mBinding.rcMenu.setLayoutManager(new LinearLayoutManager(view.getContext()));
        mBinding.rcMenu.addItemDecoration(new VerticalRecyclerItemDecoration(R.color.black, 0));
        mBinding.rcMenu.setAdapter(new MenuAdapter(datas) {
            @Override
            public void convert(MenuAdapter.VH holder, MenuData data, final int position) {
                holder.setText(R.id.btn_menu, data.getMenuText());
                holder.getView(R.id.btn_menu).setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Intent intent = new Intent(getContext(), BaseRenderActivity.class);
                        intent.putExtra("ID", position);
                        getContext().startActivity(intent);
                    }
                });
            }
        });
    }

    private List<MenuData> generateMenus() {
        List<MenuData> datas = new ArrayList<>();
        addMenuButton(datas, 1, "Triangle");
        addMenuButton(datas, 2, "StaticCube");
        addMenuButton(datas, 3, "Texture2dCube");
        addMenuButton(datas, 4, "SkyboxCube");
        return datas;
    }

    private void addMenuButton(List<MenuData> datas, int id, String text){
        MenuData data = new MenuData();
        data.setID(id);
        data.setMenuText(text);
        datas.add(data);
    }
}
