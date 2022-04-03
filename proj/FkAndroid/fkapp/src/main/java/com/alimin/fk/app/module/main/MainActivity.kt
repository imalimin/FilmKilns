package com.alimin.fk.app.module.main

import android.os.Bundle
import com.alimin.fk.app.R
import com.lmy.common.ext.disableStatusBarPadding
import com.lmy.common.ext.setLightMode
import com.lmy.common.ui.BaseActivity
import com.lmy.common.ui.fragment.BaseLazyFragment
import com.lmy.mvp.util.replaceFragmentInActivity

class MainActivity : BaseActivity(), BaseLazyFragment.OnFragmentInteractionListener {
    override val layoutResID: Int = R.layout.activity_fragment_container

    override fun initView() {
        setLightMode(true)
        disableStatusBarPadding()
        val fragment = supportFragmentManager.findFragmentById(R.id.fragment_container) as MainFragment?
            ?: MainFragment.newInstance().also {
                replaceFragmentInActivity(it, R.id.fragment_container)
            }
        MainPresenter(fragment)
    }

    override fun onFragmentInteraction(what: Int, data: Bundle) {

    }
}