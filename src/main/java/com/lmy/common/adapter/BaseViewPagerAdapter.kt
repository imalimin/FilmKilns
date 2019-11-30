package com.lmy.common.adapter

import android.support.v4.app.Fragment
import android.support.v4.app.FragmentManager
import android.support.v4.app.FragmentPagerAdapter
import android.support.v4.view.ViewPager

abstract class BaseViewPagerAdapter(private val fm: FragmentManager) : FragmentPagerAdapter(fm) {
    private var viewPager: ViewPager? = null

    fun attach(viewPager: ViewPager): BaseViewPagerAdapter {
        this.viewPager = viewPager
        viewPager.adapter = this
        return this
    }

    fun getFragment(position: Int): Fragment? {
        if (null == viewPager) throw RuntimeException("Please attach a ViewPager")
        return fm.findFragmentByTag("android:switcher:" + viewPager!!.id + ":" + position)
    }
}