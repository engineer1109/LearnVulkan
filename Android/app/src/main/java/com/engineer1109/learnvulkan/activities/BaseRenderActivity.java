package com.engineer1109.learnvulkan.activities;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowInsetsController;
import android.view.WindowManager;

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
        mViewModel.setBinding(mBinding);
        mViewModel.showMenu(mBinding.getRoot());

        ActionBar actionbar = getSupportActionBar();
        actionbar.hide();

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        );

        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.P){
            notchFullScreen();
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.P)
    public void notchFullScreen(){
        WindowManager.LayoutParams lp = getWindow().getAttributes();
        lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        getWindow().setAttributes(lp);
    }

    @Override
    protected void onResume(){
        super.onResume();
        Log.i(TAG, "onResume");
    }

    @Override
    protected void onPause(){
        super.onPause();
        Log.i(TAG, "onPause");
        mViewModel.pause();
        mViewModel.waitCurrentFrameComplete();
        mViewModel.destroyANativeWindow();
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        Log.i(TAG,"onDestroy");
        mViewModel.destroyRender();
    }
}