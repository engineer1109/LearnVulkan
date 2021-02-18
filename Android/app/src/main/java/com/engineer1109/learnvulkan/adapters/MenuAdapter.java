package com.engineer1109.learnvulkan.adapters;

import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.engineer1109.learnvulkan.databinding.LayoutMenuBinding;
import com.engineer1109.learnvulkan.model.MenuData;

import java.util.List;

public abstract class MenuAdapter extends RecyclerView.Adapter<MenuAdapter.VH> {
    static final String TAG = "MenuAdapter";

    private LayoutMenuBinding mBinding = null;

    private List<MenuData> mDatas;

    public static class VH extends RecyclerView.ViewHolder {
        private SparseArray<View> mViews;
        private View mConvertView;

        public VH(View v) {
            super(v);
            mConvertView = v;
            mViews = new SparseArray<>();
        }

        public static VH get(ViewGroup parent, int layoutId) {
            View convertView = LayoutInflater.from(parent.getContext()).inflate(layoutId, parent, false);
            return new VH(convertView);
        }

        public <T extends View> T getView(int id) {
            View v = mViews.get(id);
            if (v == null) {
                v = mConvertView.findViewById(id);
                mViews.put(id, v);
            }
            return (T) v;
        }

        public void setText(int id, String value) {
            TextView view = getView(id);
            view.setText(value);
        }

        public View getView() {
            return mConvertView;
        }
    }

    public MenuAdapter(List<MenuData> data) {
        this.mDatas = data;
    }

    @NonNull
    @Override
    public VH onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        mBinding = LayoutMenuBinding.inflate(LayoutInflater.from(parent.getContext()), parent, false);
        View view = mBinding.getRoot();
        //VH.get(parent,getLayoutId(viewType));
        VH vh = new VH(view);
        return vh;
    }

    @Override
    public void onBindViewHolder(@NonNull VH holder, int position) {
        convert(holder, mDatas.get(position), position);
        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            }
        });
    }

    @Override
    public int getItemCount() {
        return mDatas.size();
    }

    public abstract void convert(VH holder, MenuData data, int position);
}
