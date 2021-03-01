package com.engineer1109.learnvulkan.viewmodel;

import android.app.Activity;
import android.content.res.AssetManager;
import android.view.Surface;
import android.view.View;

import androidx.lifecycle.ViewModel;

import com.engineer1109.learnvulkan.render.BaseRender;

public class BaseRenderViewModel extends ViewModel {
    private BaseRender mRender = new BaseRender();

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

    public void destroyRender(){
        mRender.quit();
    }

    public void destroySurface(){
        mRender.destroySurface();
    }

    public void rebuildSurface(){
        mRender.rebuildSurface();
    }

    public void setTouchPos(float x, float y){ mRender.setTouchPos(x, y);}

    public void setTouchPosSecond(float x, float y){ mRender.setTouchPosSecond(x, y);}

    public void setTouchMode(int mode){ mTouchMode = mode; mRender.setTouchMode(mode);}

    public int getTouchMode() {return mTouchMode;}

    public void resetTouch(){ mRender.resetTouch();}

    public int getRenderStatus() {return 1;}

    public boolean isStarted() {return mRender.isStarted();}

    public void quitRender(View view){
        mRender.quit();
        while(mRender.getInstance() != 0){
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        Activity activity = (Activity) view.getContext();
        activity.finish();
    }
}
