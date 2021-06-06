package com.engineer1109.learnvulkan.viewmodel;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.util.Log;
import android.view.Surface;
import android.view.View;

import androidx.lifecycle.ViewModel;
import androidx.recyclerview.widget.LinearLayoutManager;

import com.engineer1109.learnvulkan.R;
import com.engineer1109.learnvulkan.activities.BaseRenderActivity;
import com.engineer1109.learnvulkan.adapters.MenuAdapter;
import com.engineer1109.learnvulkan.databinding.ActivityBaseRenderBinding;
import com.engineer1109.learnvulkan.databinding.ActivityStartBinding;
import com.engineer1109.learnvulkan.itemdecoration.VerticalRecyclerItemDecoration;
import com.engineer1109.learnvulkan.model.MenuData;
import com.engineer1109.learnvulkan.render.BaseRender;

import java.util.ArrayList;
import java.util.List;

public class BaseRenderViewModel extends ViewModel {
    static String TAG = "BaseRenderViewModel";

    private BaseRender mRender = new BaseRender();

    private ActivityBaseRenderBinding mBinding = null;

    public ActivityBaseRenderBinding getBinding() {
        return mBinding;
    }

    public void setBinding(ActivityBaseRenderBinding mBinding) {
        this.mBinding = mBinding;
    }

    private int mTouchMode = 0;

    public void init(int id) {
        mRender.init(id);
    }

    public void setSurface(Surface surface) {
        mRender.setSurface(surface);
    }

    public void setAssetManager(AssetManager assetManager) {
        mRender.setAssetManager(assetManager);
    }

    public void render() {
        mRender.render();
    }

    public void destroyRender() {
        mRender.quit();
    }

    public void destroySurface() {
        mRender.destroySurface();
    }

    public void rebuildSurface() {
        mRender.rebuildSurface();
    }

    public void pause() {
        mRender.pause();
    }

    public void resume() {
        mRender.resume();
    }

    public void waitCurrentFrameComplete() {
        mRender.waitCurrentFrameComplete();
    }

    public void destroyANativeWindow() {
        mRender.destroyANativeWindow();
    }

    public void setTouchPos(float x, float y) {
        mRender.setTouchPos(x, y);
    }

    public void setTouchPosSecond(float x, float y) {
        mRender.setTouchPosSecond(x, y);
    }

    public void setTouchMode(int mode) {
        mTouchMode = mode;
        mRender.setTouchMode(mode);
    }

    public int getTouchMode() {
        return mTouchMode;
    }

    public void resetTouch() {
        mRender.resetTouch();
    }

    public int getRenderStatus() {
        return 1;
    }

    public boolean isStarted() {
        return mRender.isStarted();
    }

    public void quitRender(View view) {
        mRender.quit();
        while (mRender.getInstance() != 0) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        Activity activity = (Activity) view.getContext();
        activity.finish();
    }

    @SuppressLint("ResourceAsColor")
    public void showMenu(View view) {
        List<MenuData> datas = this.generateMenus();
        if (datas.size() == 0) {
            mBinding.rcFunc.setVisibility(View.INVISIBLE);
            return;
        }
        Context context = view.getContext();
        mBinding.rcFunc.setLayoutManager(new LinearLayoutManager(view.getContext()));
        mBinding.rcFunc.addItemDecoration(new VerticalRecyclerItemDecoration(R.color.black, 0));
        mBinding.rcFunc.setAdapter(new MenuAdapter(datas) {
            @Override
            public void convert(MenuAdapter.VH holder, MenuData data, final int position) {
                holder.setText(R.id.btn_menu, data.getMenuText());
                holder.getView(R.id.btn_menu).setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Log.i(TAG, ""+position);
                        mRender.runFunction(position);
                    }
                });
            }
        });
    }

    private List<MenuData> generateMenus() {
        List<MenuData> datas = new ArrayList<>();
        if (mRender.getID() + 1 == 5) {
            addMenuButton(datas, 1, "Reflect");
        }
        if (mRender.getID() + 1 == 7) {
            addMenuButton(datas, 1, "DebugQuad");
        }
        return datas;
    }

    private void addMenuButton(List<MenuData> datas, int id, String text) {
        MenuData data = new MenuData();
        data.setID(id);
        data.setMenuText(text);
        datas.add(data);
    }
}
