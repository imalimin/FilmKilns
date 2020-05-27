package com.lmy.samplenative.ui

import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import com.lmy.hwvcnative.tools.AlFFUtils
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_ff_cmd.*
import kotlin.concurrent.thread

class FFCMDActivity : BaseActivity() {
    var dialog: AlertDialog? = null
    override fun getLayoutResource(): Int = R.layout.activity_ff_cmd
    override fun initView() {
        enterBtn.setOnClickListener {
            doing()
        }
    }

    private fun doing() {
        dialog = AlertDialog.Builder(this).setMessage("Doing").setCancelable(false).show()
        thread(start = true) {
            val ret = AlFFUtils.exec(cmdEdit.text.toString())
            runOnUiThread {
                Toast.makeText(
                    this@FFCMDActivity,
                    "exec done $ret",
                    Toast.LENGTH_LONG
                ).show()
                dialog?.dismiss()
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        dialog?.dismiss()
    }
}