package com.lmy.common.ext

import android.view.Gravity
import androidx.appcompat.app.AlertDialog

fun AlertDialog.showBottom() {
    window?.setGravity(Gravity.BOTTOM)
    show()
}