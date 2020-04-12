package com.lmy.common.ext

import android.text.Editable
import android.text.TextWatcher
import android.widget.EditText

fun EditText.setOnTextChangedListener(listener: (e: Editable?) -> Unit) {
    addTextChangedListener(object : TextWatcher {
        override fun afterTextChanged(s: Editable?) {
            listener(s)
        }

        override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {
        }

        override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
        }
    })
}