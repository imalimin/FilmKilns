package com.lmy.file.ui.dialog

import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.fragment.app.DialogFragment
import androidx.recyclerview.widget.LinearLayoutManager
import com.lmy.common.ext.setOnItemClickListener
import com.lmy.common.model.entity.Error
import com.lmy.common.ui.dialog.BaseDialogFragment
import com.lmy.file.R
import com.lmy.file.adapter.FileAdapter
import com.lmy.file.impl.LocalDiscoverer
import kotlinx.android.synthetic.main.dialog_file.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import java.io.File
import java.util.*

class FileDialog : BaseDialogFragment() {
    private val job = Job()
    private val uiScope = CoroutineScope(Dispatchers.Main + job)
    private val adapter = FileAdapter { LocalDiscoverer.instance }
    private var lastIndex = 0
    private var isBack = false

    override fun getLayoutView(): Int = R.layout.dialog_file

    override fun isBottomSheet(): Boolean {
        return true
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setStyle(DialogFragment.STYLE_NORMAL, R.style.AlertDialog_Bottom)
    }

    override fun initView() {
        backBtn.setOnClickListener { back(it) }
        recyclerView.adapter = adapter
        list()
        recyclerView.setOnItemClickListener { parent, view, position ->
            val file = adapter.items[position]
            if (File(LocalDiscoverer.instance.getCurrentPath(), file).isFile) {
                enter(file)
                return@setOnItemClickListener
            }
            lastIndex = position
            isBack = false
            adapter.clear()
            open(file)
        }
    }

    fun onShowFileList(files: Array<String>) {
        adapter.bindData(ArrayList<String>(files.asList()))
        if (isBack) {
            (recyclerView.layoutManager as LinearLayoutManager).apply {
                scrollToPositionWithOffset(lastIndex, 0)
            }
        }
    }

    fun onProgress(progress: Float, index: Int, curName: String) {

    }

    fun showError(error: Int, msg: String) {
        if (Int.MAX_VALUE == error) {
            dismiss()
            return
        }
        Toast.makeText(context, msg, Toast.LENGTH_LONG).show()
    }

    private fun back(v: View) {
        isBack = true
        back()
    }

    private fun enter(file: String) {
        onButtonPressed(arguments!!.getInt("operate"),
                Bundle().apply { putString("path", "${LocalDiscoverer.instance.getCurrentPath()}/${file}") })
        dismiss()
    }

    override fun onDetach() {
        super.onDetach()
    }

    companion object {

        fun newInstance(operate: Int) = FileDialog().apply {
            arguments = Bundle().apply { putInt("operate", operate) }
        }
    }

    private fun open(file: String) {
        LocalDiscoverer.instance.open(file) {
            uiScope.launch {
                if (null == it) {
                    val error = Error.error(Int.MAX_VALUE, "This path not a directory, or not exists.")
                    showError(error.error, error.msg!!)
                } else {
                    onShowFileList(it)
                }
            }
        }
    }

    private fun back() {
        LocalDiscoverer.instance.back {
            uiScope.launch {
                if (null == it) {
                    val error = Error.error(Int.MAX_VALUE, "Root path.")
                    showError(error.error, error.msg!!)
                    return@launch
                }
                if (it.isEmpty()) {
                    val error = Error.error(Int.MAX_VALUE, "This path not a directory, or not exists.")
                    showError(error.error, error.msg!!)
                } else {
                    onShowFileList(it)
                }
            }
        }
    }

    private fun list() {
        LocalDiscoverer.instance.list {
            uiScope.launch {
                if (null == it) {
                    val error = Error.error(Int.MAX_VALUE, "Root path.")
                    showError(error.error, error.msg!!)
                    return@launch
                }
                if (it.isEmpty()) {
                    val error = Error.error(Int.MAX_VALUE, "This path not a directory, or not exists.")
                    showError(error.error, error.msg!!)
                } else {
                    onShowFileList(it)
                }
            }
        }
    }
}