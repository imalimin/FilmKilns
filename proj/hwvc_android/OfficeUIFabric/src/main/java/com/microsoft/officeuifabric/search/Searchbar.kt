/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.search

import android.app.SearchableInfo
import android.content.Context
import android.support.v4.content.ContextCompat
import android.support.v7.widget.AppCompatImageView
import android.support.v7.widget.SearchView
import android.util.AttributeSet
import android.view.View
import android.widget.*
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.util.isVisible
import com.microsoft.officeuifabric.util.showKeyboard
import com.microsoft.officeuifabric.view.TemplateView

/**
 * [Searchbar] provides a [SearchView] with a search icon, back button, close icon,
 * and progress indicator. It is designed to be used as the primary search experience at the top of the app,
 * in conjunction with the Toolbar or stand alone.
 */
open class Searchbar : TemplateView, SearchView.OnQueryTextListener {
    /**
     * Returns the current query text from the search view.
     */
    val query: CharSequence
        get() = searchView?.query ?: ""

    /**
     * Query hint text for the search view.
     */
    var queryHint: String = context.getString(R.string.searchbar_query_hint_default)
        set(value) {
            if (field == value)
                return
            field = value
            updateViews()
        }

    /**
     * Show the circular progress bar to indicate search is in progress.
     */
    var showSearchProgress: Boolean = false
        set(value) {
            if (field == value)
                return
            field = value
            updateViews()
        }

    /**
     * Sets the [SearchableInfo] for the search view.
     */
    var searchableInfo: SearchableInfo? = null
        set(value) {
            if (field == value)
                return
            field = value
            updateViews()
        }

    /**
     * Provides callbacks for when text is entered in the search view and when that query is submitted.
     */
    var onQueryTextListener: SearchView.OnQueryTextListener? = null

    /**
     * Provides a callback for when the search view focus changes.
     */
    var onQueryTextFocusChangeListener: OnFocusChangeListener? = null

    /**
     * Provides a callback for when the search back button is clicked and the search view is closed.
     */
    var onCloseListener: SearchView.OnCloseListener? = null

    @JvmOverloads
    constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0) : super(context, attrs, defStyleAttr)

    /**
     * Sets the query text for the search view.
     */
    fun setQuery(query: CharSequence, submit: Boolean) {
        searchView?.setQuery(query, submit)
    }

    override fun clearFocus() {
        super.clearFocus()
        searchView?.clearFocus()
    }

    override fun onQueryTextSubmit(query: String): Boolean =
        onQueryTextListener?.onQueryTextSubmit(query) ?: false

    override fun onQueryTextChange(query: String): Boolean {
        updateCloseIconVisibility()
        return onQueryTextListener?.onQueryTextChange(query) ?: false
    }

    // Template

    override val templateId: Int = R.layout.view_searchbar

    private var searchbar: FrameLayout? = null
    private var searchViewContainer: LinearLayout? = null
    private var searchIcon: ImageView? = null
    private var searchBackButton: ImageButton? = null
    private var searchView: SearchView? = null
    private var searchCloseButton: ImageButton? = null
    private var searchProgress: ProgressBar? = null

    override fun onTemplateLoaded() {
        super.onTemplateLoaded()

        searchbar = findViewInTemplateById(R.id.searchbar)
        searchViewContainer = findViewInTemplateById(R.id.search_view_container)
        searchIcon = findViewInTemplateById(R.id.search_icon)
        searchBackButton = findViewInTemplateById(R.id.search_back_button)
        searchView = findViewInTemplateById(R.id.search_view)
        searchCloseButton = findViewInTemplateById(R.id.search_close)
        searchProgress = findViewInTemplateById(R.id.search_progress)

        // Hide the default search view close button from TalkBack and get rid of the space it takes up.
        val closeButton = searchView?.findViewById<AppCompatImageView>(R.id.search_close_btn)
        closeButton?.importantForAccessibility = View.IMPORTANT_FOR_ACCESSIBILITY_NO
        closeButton?.setPadding(0, 0, 0, 0)

        updateViews()
        setupListeners()
        setUnfocusedState()
    }

    private fun updateViews() {
        searchView?.queryHint = queryHint
        searchView?.setSearchableInfo(searchableInfo)
        updateProgressVisibility()
    }

    private fun setupListeners() {
        setOnClickListener {
            searchView?.requestFocus()
        }

        searchBackButton?.setOnClickListener {
            clearFocus()
            onCloseListener?.onClose()
        }

        searchView?.setOnQueryTextListener(this)
        searchView?.setOnQueryTextFocusChangeListener { searchView, hasFocus ->
            if (hasFocus) {
                showKeyboard()
                setFocusedState()
            } else {
                setUnfocusedState()
            }

            onQueryTextFocusChangeListener?.onFocusChange(searchView, hasFocus)
        }

        searchCloseButton?.setOnClickListener {
            setQuery("", false)
        }
    }

    private fun setUnfocusedState() {
        setQuery("", false)
        updateSearchViewSpacing()
        searchViewContainer?.background = ContextCompat.getDrawable(context, R.drawable.search_view_container_background)
        searchIcon?.isVisible = true
        searchBackButton?.isVisible = false
        showSearchProgress = false
    }

    private fun setFocusedState() {
        updateSearchViewSpacing()
        searchViewContainer?.background = null
        searchIcon?.isVisible = false
        searchBackButton?.isVisible = true
    }

    private fun updateProgressVisibility() {
        searchProgress?.isVisible = showSearchProgress
        updateSearchViewContainerMarginEnd()
    }

    private fun updateCloseIconVisibility() {
        searchCloseButton?.isVisible = query.isNotEmpty()
        updateSearchViewContainerMarginEnd()
    }

    private fun updateSearchViewContainerMarginEnd() {
        val withIcons = showSearchProgress || searchCloseButton?.isVisible == true
        val searchViewContainerMarginEndResourceId = if (withIcons)
            R.dimen.uifabric_searchbar_search_view_container_with_icons_margin_end
        else
            R.dimen.uifabric_searchbar_search_view_container_margin_horizontal

        val lp = searchViewContainer?.layoutParams as? FrameLayout.LayoutParams ?: return
        lp.marginEnd = context.resources.getDimension(searchViewContainerMarginEndResourceId).toInt()
        searchViewContainer?.layoutParams = lp
    }

    private fun updateSearchViewSpacing() {
        // Search edit frame
        val searchEditFrame = searchView?.findViewById<LinearLayout>(R.id.search_edit_frame)
        val searchEditFrameMarginStartResourceId = if (hasFocus())
            R.dimen.uifabric_searchbar_with_back_button_search_view_margin_start
        else
            R.dimen.uifabric_searchbar_with_search_icon_search_view_margin_start

        val searchEditFrameLayoutParams = searchEditFrame?.layoutParams as? LinearLayout.LayoutParams
        searchEditFrameLayoutParams?.marginStart = resources.getDimension(searchEditFrameMarginStartResourceId).toInt()
        searchEditFrame?.layoutParams = searchEditFrameLayoutParams

        // Search text
        val searchSrcText = searchView?.findViewById<SearchView.SearchAutoComplete>(R.id.search_src_text)
        val searchSrcTextPaddingStartResourceId = if (hasFocus())
            R.dimen.uifabric_searchbar_with_back_button_search_view_text_padding_start
        else
            R.dimen.uifabric_searchbar_with_search_icon_search_view_text_padding_start

        searchSrcText?.setPaddingRelative(
            resources.getDimension(searchSrcTextPaddingStartResourceId).toInt(),
            searchSrcText.paddingTop,
            searchSrcText.paddingEnd,
            searchSrcText.paddingBottom
        )
    }
}