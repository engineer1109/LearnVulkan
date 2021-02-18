package com.engineer1109.learnvulkan.activities;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.engineer1109.learnvulkan.databinding.ActivityBaseRenderBinding;

public class BaseRenderActivity extends AppCompatActivity {

    static final String TAG = "BaseRenderActivity";

    private ActivityBaseRenderBinding mBinding = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = ActivityBaseRenderBinding.inflate(getLayoutInflater());
        setContentView(mBinding.getRoot());
    }
}