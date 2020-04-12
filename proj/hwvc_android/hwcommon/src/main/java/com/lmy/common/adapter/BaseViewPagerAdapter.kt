package com.lmy.common.adapter

import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentPagerAdapter
import androidx.viewpager.widget.ViewPager


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