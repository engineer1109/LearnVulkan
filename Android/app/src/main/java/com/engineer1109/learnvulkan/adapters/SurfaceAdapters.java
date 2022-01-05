package com.engineer1109.learnvulkan.adapters;

import android.annotation.SuppressLint;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.databinding.BindingAdapter;

import com.engineer1109.learnvulkan.viewmodel.BaseRenderViewModel;

public class SurfaceAdapters {
    static final String TAG = "SurfaceAdapters";

    @BindingAdapter("android:surfaceViewCallback")
    public static void setSurfaceViewCallback(final SurfaceView view, final BaseRenderViewModel viewModel) {
        view.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {

            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
                Log.i(TAG, "surfaceChanged");
                viewModel.setSurface(view.getHolder().getSurface());
                viewModel.setAssetManager(view.getContext().getAssets());
                if(viewModel.isStarted()){
                    //viewModel.destroySurface();
                    viewModel.rebuildSurface();
                    viewModel.resume();
                }
                else{
                    viewModel.render();
                }
            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
                Log.i(TAG, "surfaceDestroyed");
                //viewModel.destroySurface();
            }
        });
    }

    @SuppressLint("ClickableViewAccessibility")
    @BindingAdapter("android:surfaceTouchEvent")
    public static void setSurfaceTouchEvent(final SurfaceView view, final BaseRenderViewModel viewModel) {
        view.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                float x1, y1, x2, y2;
                switch (event.getAction() & MotionEvent.ACTION_MASK) {
                    case MotionEvent.ACTION_DOWN:
                        viewModel.resetTouch();
                        viewModel.setTouchMode(1);
                        break;
                    case MotionEvent.ACTION_UP:
                        viewModel.setTouchMode(0);
                        viewModel.resetTouch();
                        break;
                    case MotionEvent.ACTION_POINTER_DOWN:
                        x1 = event.getX(0);
                        y1 = event.getY(0);
                        x2 = event.getX(1);
                        y2 = event.getY(1);
                        viewModel.setTouchMode(2);
                        viewModel.setTouchPos(x1, y1);
                        viewModel.setTouchPosSecond(x2, y2);
                        Log.i(TAG, "getTouchMode" + viewModel.getTouchMode());
                        break;
                    case MotionEvent.ACTION_POINTER_UP:
                        viewModel.setTouchMode(viewModel.getTouchMode() - 1);
                        viewModel.setTouchPosSecond(0, 0);
                        Log.i(TAG, "getTouchMode" + viewModel.getTouchMode());
                        break;
                    case MotionEvent.ACTION_MOVE:
                        //System.out.println("test0");
                        if (viewModel.getTouchMode() == 1) {
                            x1 = event.getX(0);
                            y1 = event.getY(0);
                            viewModel.setTouchPos(x1, y1);
                        }
                        if (viewModel.getTouchMode() >= 2) {
                            x2 = event.getX(1);
                            y2 = event.getY(1);
                            viewModel.setTouchPosSecond(x2, y2);
                        }
                        Log.i(TAG, "getTouchMode" + viewModel.getTouchMode());
                        break;
                    case MotionEvent.ACTION_CANCEL:
                        viewModel.setTouchMode(0);
                        viewModel.resetTouch();
                        break;
                    case MotionEvent.ACTION_OUTSIDE:
                        break;
                }
                return view.onTouchEvent(event);
            }
        });
    }
}
