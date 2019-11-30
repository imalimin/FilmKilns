package com.lmy.common.ui.dialog

import android.app.Dialog
import android.content.Context
import android.os.Bundle
import android.support.v4.app.DialogFragment
import android.support.v4.app.Fragment
import android.support.v4.app.FragmentManager
import android.support.v4.app.FragmentTransaction
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.lmy.common.R
import com.lmy.common.ui.fragment.BaseLazyFragment

abstract class BaseDialogFragment : DialogFragment() {
    private var mListener: BaseLazyFragment.OnFragmentInteractionListener? = null
    private var transition = FragmentTransaction.TRANSIT_FRAGMENT_OPEN

    open fun isBottomSheet(): Boolean {
        return false
    }

    fun <T> Fragment.getTargetActivity(): T? {
        return activity as T
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setStyle(DialogFragment.STYLE_NORMAL, if (isBottomSheet()) R.style.BaseAlertDialog_Bottom else R.style.BaseAlertDialog)
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        val dialog = super.onCreateDialog(savedInstanceState)
        dialog.window!!.setGravity(if (isBottomSheet()) Gravity.BOTTOM else Gravity.CENTER)
        return dialog
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View? {
        return inflater.inflate(getLayoutView(), container, false)
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        initView()
    }

    /**
     * 全屏显示
     *
     * @param manager
     * @param tag
     */
    @Deprecated("")
    fun showFull(manager: FragmentManager, tag: String) {
        val ft = manager.beginTransaction()
        ft.setTransition(transition)
        showFull(ft, tag)
    }

    /**
     * 全屏显示
     *
     * @param transaction
     * @param tag
     */
    fun showFull(transaction: FragmentTransaction, tag: String) {
        transaction.replace(android.R.id.content, this, tag).addToBackStack(null).commit()
    }

    override fun show(manager: FragmentManager, tag: String) {
        manager.beginTransaction().setTransition(transition)
        super.show(manager, tag)
    }

    override fun show(transaction: FragmentTransaction, tag: String): Int {
        transaction.setTransition(transition)
        return super.show(transaction, tag)
    }

    // TODO: Rename method, update argument and hook method into UI event
    fun onButtonPressed(what: Int, data: Bundle) {
        if (mListener != null)
            mListener!!.onFragmentInteraction(what, data)
    }

    override fun onAttach(context: Context?) {
        super.onAttach(context)
        if (context is BaseLazyFragment.OnFragmentInteractionListener) {
            mListener = context
        }
    }

    override fun onDetach() {
        super.onDetach()
        mListener = null
    }

    override fun onDestroyView() {
        super.onDestroyView()
    }

    fun setTransition(transition: Int) {
        this.transition = transition
    }

    protected abstract fun getLayoutView(): Int

    protected abstract fun initView()

    fun show(transaction: FragmentTransaction, fragment: DialogFragment) {}
}