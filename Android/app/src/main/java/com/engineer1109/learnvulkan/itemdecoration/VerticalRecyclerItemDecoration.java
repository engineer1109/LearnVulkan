package com.engineer1109.learnvulkan.itemdecoration;

import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.view.View;

import androidx.annotation.ColorInt;
import androidx.recyclerview.widget.RecyclerView;

public class VerticalRecyclerItemDecoration extends RecyclerView.ItemDecoration {
    private int mSpaceHeight = 0;
    private Drawable mDivider = null;

    public VerticalRecyclerItemDecoration(@ColorInt int color, int space) {
        mSpaceHeight = space;
        mDivider = new ColorDrawable(color);
    }

    @Override
    public void onDraw(Canvas c, RecyclerView parent, RecyclerView.State state) {
        if (mSpaceHeight <= 0) {
            super.onDraw(c, parent, state);
            return;
        }
        int left, right, top, bot;
        //
        left = parent.getPaddingLeft();
        right = parent.getWidth() - parent.getPaddingRight();
        for (int i = 0; i < parent.getChildCount(); i++) {
            final View child = parent.getChildAt(i);
            final RecyclerView.LayoutParams params = (RecyclerView.LayoutParams) child.getLayoutParams();
            top = child.getBottom() + params.bottomMargin;
            bot = top + mSpaceHeight;
            mDivider.setBounds(left, top, right, bot);
            mDivider.draw(c);
        }
    }

    @Override
    public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
        if (mSpaceHeight > 0) {
            outRect.bottom = mSpaceHeight;
        }

    }
}
