package com.lmy.file.ui.fragment

import android.content.Intent
import android.os.Bundle
import android.support.v7.widget.LinearLayoutManager
import android.widget.Toast
import com.lmy.common.ext.fromFile
import com.lmy.common.ext.setOnItemClickListener
import com.lmy.common.model.entity.Error
import com.lmy.common.ui.fragment.BaseFragment
import com.lmy.file.R
import com.lmy.file.adapter.FileAdapter
import com.lmy.file.impl.LocalDiscoverer
import kotlinx.android.synthetic.main.fragment_file.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import java.io.File
import java.util.*

class FileFragment : BaseFragment() {
    private val job = Job()
    private val uiScope = CoroutineScope(Dispatchers.Main + job)
    private val adapter = FileAdapter { LocalDiscoverer.instance }
    private var lastIndex = 0
    private var handleIndex = 0
    private var isBack = false

    override fun getLayoutView(): Int = R.layout.fragment_file

    override fun initView() {
        recyclerView.adapter = adapter
        list()
        recyclerView.setOnItemClickListener { parent, view, position ->
            val name = adapter.items[position]
            val file = File(LocalDiscoverer.instance.getCurrentPath(), name)
            if (file.isFile) {
                activity?.startActivity(Intent().fromFile(context!!, file))
                return@setOnItemClickListener
            }
            lastIndex = position
            isBack = false
            adapter.clear()
            open(name)
        }
    }

    fun onBackPressed() {
        isBack = true
        back()
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
        if (progress >= 1f) {
            list()
        }
    }

    fun showError(error: Int, msg: String) {
        if (Int.MAX_VALUE == error) {
            LocalDiscoverer.instance.close()
            activity?.finish()
            return
        }
        Toast.makeText(context, msg, Toast.LENGTH_LONG).show()
    }

    companion object {
        fun newInstance() = FileFragment().apply { arguments = Bundle() }
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