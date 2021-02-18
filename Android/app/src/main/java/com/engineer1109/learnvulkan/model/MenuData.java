package com.engineer1109.learnvulkan.model;

import java.io.Serializable;

public class MenuData implements Serializable {
    private int mID = 0;

    private String mMenuText = "";

    private int mKind = 0;

    public int getID() {
        return mID;
    }

    public void setID(int mID) {
        this.mID = mID;
    }

    public String getMenuText() {
        return mMenuText;
    }

    public void setMenuText(String mMenuText) {
        this.mMenuText = mMenuText;
    }

    public int getKind() {
        return mKind;
    }

    public void setKind(int mKind) {
        this.mKind = mKind;
    }
}
