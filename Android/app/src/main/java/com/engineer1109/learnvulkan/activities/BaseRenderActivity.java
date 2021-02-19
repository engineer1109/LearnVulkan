package com.engineer1109.learnvulkan.activities;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

import android.content.Intent;
import android.os.Bundle;

import com.engineer1109.learnvulkan.databinding.ActivityBaseRenderBinding;
import com.engineer1109.learnvulkan.viewmodel.BaseRenderViewModel;

public class BaseRenderActivity extends AppCompatActivity {

    static final String TAG = "BaseRenderActivity";

    private ActivityBaseRenderBinding mBinding = null;

    private BaseRenderViewModel mViewModel = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = ActivityBaseRenderBinding.inflate(getLayoutInflater());
        setContentView(mBinding.getRoot());

        Intent intent = getIntent();
        int id = intent.getIntExtra("ID", 0);

        mViewModel = new ViewModelProvider(this).get(BaseRenderViewModel.class);
        mBinding.setBaseRenderViewModel(mViewModel);
        mViewModel.init(id);
    }
}