package com.lmy.samplenative.ui

import androidx.appcompat.app.AlertDialog
import com.lmy.hwvcnative.tools.AlFFUtils
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_ff_cmd.*
import kotlin.concurrent.thread

class FFCMDActivity : BaseActivity() {
    var dialog: AlertDialog? = null
    val sb = StringBuffer()
    override fun getLayoutResource(): Int = R.layout.activity_ff_cmd
    override fun initView() {
        enterBtn.setOnClickListener {
            doing()
        }
    }

    private fun doing() {
        dialog = AlertDialog.Builder(this).setMessage("Doing").setCancelable(false).show()
        thread(start = true) {
            val time = System.currentTimeMillis()
            val ret = AlFFUtils.exec(cmdEdit.text.toString())
            val cost = System.currentTimeMillis() - time
            runOnUiThread {
                dialog?.dismiss()
                sb.append("exec cost ${cost}ms, done $ret\n")
                logView.text = sb.toString()
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        dialog?.dismiss()
    }
}