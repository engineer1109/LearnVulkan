package com.engineer1109.learnvulkan.activities;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;

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
}