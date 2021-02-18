package com.engineer1109.learnvulkan.activities;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

import android.os.Bundle;

import com.engineer1109.learnvulkan.databinding.ActivityStartBinding;
import com.engineer1109.learnvulkan.viewmodel.MainViewModel;

public class StartActivity extends AppCompatActivity {

    static final String TAG = "StartActivity";

    ActivityStartBinding mBinding = null;

    private MainViewModel mMainViewModel = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = ActivityStartBinding.inflate(getLayoutInflater());
        setContentView(mBinding.getRoot());

        mMainViewModel = new ViewModelProvider(this).get(MainViewModel.class);
        mBinding.setMainViewModel(mMainViewModel);
        mMainViewModel.setBinding(mBinding);
        mMainViewModel.showMenu(mBinding.rcMenu);
    }
}