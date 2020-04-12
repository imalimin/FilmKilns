/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabricdemo.demos

import android.os.Bundle
import android.view.View
import com.microsoft.officeuifabric.drawer.Drawer
import com.microsoft.officeuifabric.drawer.DrawerDialog
import com.microsoft.officeuifabric.drawer.OnDrawerContentCreatedListener
import com.microsoft.officeuifabricdemo.DemoActivity
import com.microsoft.officeuifabricdemo.R
import com.microsoft.officeuifabricdemo.util.createPersonaList
import kotlinx.android.synthetic.main.activity_drawer.*
import kotlinx.android.synthetic.main.demo_drawer_content.view.*

class DrawerActivity : DemoActivity(), OnDrawerContentCreatedListener {
    override val contentLayoutId: Int
        get() = R.layout.activity_drawer

    private var drawerDialogDemo: DrawerDialog? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        show_drawer_button.setOnClickListener {
            val drawerDemo = Drawer.newInstance(R.layout.demo_drawer_content)
            drawerDemo.show(supportFragmentManager, null)
        }

        show_drawer_dialog_button.setOnClickListener {
            drawerDialogDemo = DrawerDialog(this)
            drawerDialogDemo?.onDrawerContentCreatedListener = this
            drawerDialogDemo?.setContentView(R.layout.demo_drawer_content)
            drawerDialogDemo?.show()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        drawerDialogDemo?.dismiss()
    }

    override fun onDrawerContentCreated(drawerContents: View) {
        val personaList = createPersonaList(this)
        drawerContents.drawer_demo_persona_list.personas = personaList
    }
}