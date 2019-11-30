package com.lmy.common.ext

import android.support.v7.app.AlertDialog
import android.view.Gravity

fun AlertDialog.showBottom() {
    window?.setGravity(Gravity.BOTTOM)
    show()
}