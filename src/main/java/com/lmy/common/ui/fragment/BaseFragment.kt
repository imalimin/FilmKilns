package com.lmy.common.ui.fragment

import android.os.Bundle

abstract class BaseFragment : BaseLazyFragment() {
    override fun onActivityCreated(savedInstanceState: Bundle?) {
        userVisibleHint = true
        super.onActivityCreated(savedInstanceState)
    }
}