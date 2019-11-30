package com.lmy.common.ui.fragment

import android.content.Context
import android.os.Bundle
import android.support.v4.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup

abstract class BaseLazyFragment : Fragment() {
    protected var initedView = false//是否调用onCreateView的标记
    protected var inited = false//是否调用initView的标记
    private var mListener: OnFragmentInteractionListener? = null

    init {
        arguments = Bundle()
    }

    protected abstract fun getLayoutView(): Int


    protected abstract fun initView()

    fun onVisibleToUser() {

    }

    fun initView(view: View?) {
        initView()
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        initedView = true
        if (inited) initView(view)
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(getLayoutView(), container, false)
    }

    // TODO: Rename method, update argument and hook method into UI event
    fun onButtonPressed(what: Int, data: Bundle) {
        if (mListener != null) {
            mListener!!.onFragmentInteraction(what, data)
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is OnFragmentInteractionListener) {
            mListener = context
        } else {
            throw RuntimeException(context.toString() + " must implement OnFragmentInteractionListener")
        }
    }

    override fun onDetach() {
        super.onDetach()
        mListener = null
    }

    override fun onDestroyView() {
        super.onDestroyView()
        initedView = false
        inited = false
    }

    /**
     * adapter中的每个fragment切换的时候都会被调用，如果是切换到当前页，那么isVisibleToUser==true，否则为false
     */
    override fun setUserVisibleHint(isVisibleToUser: Boolean) {
        super.setUserVisibleHint(isVisibleToUser)
        //        Log.v(MainCourseFragment.class, "isVisibleToUser=" + isVisibleToUser + ",inited=" + inited);
        if (isVisibleToUser && !inited) {
            onVisibleToUser()
            inited = true
            if (initedView)
            //只有调用过onActivityCreated后才能initView
                initView(view)
        }
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     *
     *
     * See the Android Training lesson [Communicating with Other Fragments](http://developer.android.com/training/basics/fragments/communicating.html) for more information.
     */
    interface OnFragmentInteractionListener {
        fun onFragmentInteraction(what: Int, data: Bundle)
    }
}

fun <T> Fragment.getTargetActivity(): T? {
    return activity as T
}