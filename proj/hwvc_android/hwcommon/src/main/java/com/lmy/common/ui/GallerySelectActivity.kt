package com.lmy.common.ui

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.database.Cursor
import android.graphics.Color
import android.graphics.drawable.ColorDrawable
import android.media.ExifInterface
import android.os.Build
import android.os.Bundle
import android.provider.MediaStore
import android.support.v4.app.Fragment
import android.support.v4.app.LoaderManager
import android.support.v4.content.CursorLoader
import android.support.v4.content.Loader
import android.support.v7.widget.GridLayoutManager
import android.support.v7.widget.ListPopupWindow
import android.support.v7.widget.RecyclerView
import android.text.TextUtils
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.Toast
import com.lmy.common.R
import com.lmy.common.adapter.GalleryAdapter
import com.lmy.common.adapter.GalleryFolderAdapter
import com.lmy.common.ext.OnRecyclerItemClickListener
import com.lmy.common.ext.deviceScreenSize
import com.lmy.common.ext.getNavigationBarHeight
import com.lmy.common.model.entity.Folder
import kotlinx.android.synthetic.main.activity_gallery_select.*
import java.io.File

/**
 * @author limingyi
 * Created on 2017/5/19 0019 10:56.
 */
class GallerySelectActivity : BaseActivity(), View.OnClickListener, AdapterView.OnItemClickListener,
        OnRecyclerItemClickListener.OnItemClickListener {

    companion object {
        //        val DATA = "DATA"
        val DATA_SRC = "DATA_SRC"
        val DATA_DEST = "DATA_DEST"
        val MAX_WIDTH = "MAX_WIDTH"
        private val MAX_SELECT_COUNT = "MAX_SELECT_COUNT"

        fun getResultDtata(data: Intent?): ArrayList<String>? = data?.getStringArrayListExtra(DATA_SRC)

        fun request(activity: Activity, requestCode: Int, maxSelectCount: Int) {
            activity.startActivityForResult(createIntent(activity, 0, maxSelectCount), requestCode)
        }

        fun request(fragment: Fragment, requestCode: Int, maxSelectCount: Int) {
            fragment.startActivityForResult(createIntent(fragment.context, 0, maxSelectCount), requestCode)
        }

        fun request(activity: Activity, requestCode: Int, maxWidth: Int, maxSelectCount: Int) {
            activity.startActivityForResult(createIntent(activity, maxWidth, maxSelectCount), requestCode)
        }

        fun request(fragment: Fragment, requestCode: Int, maxWidth: Int, maxSelectCount: Int) {
            fragment.startActivityForResult(createIntent(fragment.context, maxWidth, maxSelectCount), requestCode)
        }

        private fun createIntent(context: Context?, maxWidth: Int, maxSelectCount: Int): Intent {
            return Intent(context, GallerySelectActivity::class.java).putExtra(MAX_SELECT_COUNT, maxSelectCount)
                    .putExtra(MAX_WIDTH, maxWidth)
        }

        fun clearCache(context: Context) {
            val file = GallerySelectActivity.getCacheDir(context)
            if (!file.isDirectory) return
            val files = file.listFiles()
            for (f in files)
                f.delete()
        }

        private fun getCacheDir(context: Context): File {
            val file = File(context.externalCacheDir, "gallery/")
            if (!file.exists()) file.mkdirs()
            return file
        }
    }

    private val SPAN_COUNT = 3
    // 不同loader定义
    private val LOADER_ALL = 0
    private val LOADER_CATEGORY = 1

    private var mFolderPopupWindow: ListPopupWindow? = null
    private var mAdapter: GalleryAdapter? = null
    private var mFolderAdapter: GalleryFolderAdapter? = null
    private var mAllImages: ArrayList<File>? = null
    private var mFolders = ArrayList<Folder>()
    /**
     * 最大选择张数，默认单选
     */
    private var mMaxSelectCount = 1
    /**
     * 最大宽度，0不作处理
     */
    private var mMaxWidth = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        initView()
    }

    override val layoutResID: Int = R.layout.activity_gallery_select

    override fun initView() {
        setDarkStatusBar()
        fillStatusBar()
        fillNavigation()
        config()
        mRecyclerView.setPadding(mRecyclerView!!.paddingLeft, mRecyclerView!!.paddingTop, mRecyclerView!!.paddingRight,
                mRecyclerView!!.paddingBottom + getNavigationBarHeight())
        mToolbar?.setTitleTextAppearance(this, R.style.ToolbarTextAppearance)
        showToolbar(mToolbar, R.string.gallery, R.mipmap.ic_back_black)
        mAdapter = GalleryAdapter()
        mAdapter?.bindData(ArrayList<File>())
        mRecyclerView?.layoutManager = GridLayoutManager(this, SPAN_COUNT)
        mRecyclerView?.addOnItemTouchListener(OnRecyclerItemClickListener(this, this))
        mRecyclerView?.adapter = mAdapter
        initFolderPopup()
        //加载全部图片
        supportLoaderManager.initLoader<Cursor>(LOADER_ALL, Bundle.EMPTY, mLoaderCallback)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            //set Margin
            val height = getNavigationBarHeight()
            (mFolderLayout?.layoutParams as ViewGroup.MarginLayoutParams).bottomMargin += height
            (mDoneBtn?.layoutParams as ViewGroup.MarginLayoutParams).bottomMargin += height
            mFolderLayout?.requestLayout()
            mDoneBtn?.requestLayout()
        }
    }

    private fun config() {
        mMaxSelectCount = intent.getIntExtra(MAX_SELECT_COUNT, mMaxSelectCount)
        mMaxWidth = intent.getIntExtra(MAX_WIDTH, 0)
        setResult(Activity.RESULT_CANCELED, Intent())
    }

    private fun initFolderPopup() {
        mFolderAdapter = GalleryFolderAdapter()
        val size = deviceScreenSize()
        val width = size[0]
        val height = (size[1] * (4.5f / 8.0f)).toInt()
        mFolderPopupWindow = ListPopupWindow(this)
        mFolderPopupWindow?.setBackgroundDrawable(ColorDrawable(Color.WHITE))
        mFolderPopupWindow?.setAdapter(mFolderAdapter)
        mFolderPopupWindow?.setContentWidth(width)
        mFolderPopupWindow?.width = width
        mFolderPopupWindow?.height = height
        mFolderPopupWindow?.anchorView = mFolderView
        mFolderPopupWindow?.isModal = true
        mFolderPopupWindow?.setOnItemClickListener(this)
    }

    /**
     * 图片扩展信息操作接口
     */
    private var mExifInterface: ExifInterface? = null
    private var mExifMap = emptyMap<String, String>()

    private fun submit() {
        val result = getSelectedPath()
        setResult(Activity.RESULT_OK, Intent().putStringArrayListExtra(DATA_SRC, result)
                .putStringArrayListExtra(DATA_DEST, result))
        finish()
    }

    private val JPEG_FILE_PREFIX = "IMG_"
    private val JPEG_FILE_SUFFIX = ".jpg"

    fun createTmpFile(index: Int): File {
        val tmp = File.createTempFile(JPEG_FILE_PREFIX, JPEG_FILE_SUFFIX, Companion.getCacheDir(this))
        tmp.deleteOnExit()
        return tmp
    }

    fun getSelectedPath(): ArrayList<String>? {
        return mAdapter!!.selectedItems?.mapTo(ArrayList()) { it.absolutePath }
    }

    override fun onClick(v: View) {
        if (R.id.mFolderView == v.id || R.id.mFolderLayout == v.id) {
            if (mFolderPopupWindow!!.isShowing) {
                mFolderPopupWindow?.dismiss()
            } else {
                mFolderPopupWindow?.show()
                var index = mFolderAdapter!!.selectIndex
                index = if (index == 0) index else index - 1
                mFolderPopupWindow?.listView!!.setSelection(index)
            }
        } else if (R.id.mDoneBtn == v.id) {
            submit()
        }
    }

    /**
     * Image点击回调

     * @param parent
     * *
     * @param view
     * *
     * @param position
     */
    override fun onItemClick(parent: RecyclerView, view: View?, position: Int) {
        if (mAdapter!!.selectedCount == mMaxSelectCount && !mAdapter!!.isSelected(position)) {
            Toast.makeText(this, R.string.msg_amount_limit, Toast.LENGTH_LONG).show()
            return
        }
        mAdapter?.select(parent, position)
        mDoneBtn?.isEnabled = mAdapter!!.selectedCount > 0
        /**
         * 如果是单选则直接返回
         */
        if (1 == mMaxSelectCount)
            submit()
    }

    /**
     * Folder点击回调

     * @param parent
     * *
     * @param view
     * *
     * @param position
     * *
     * @param id
     */
    override fun onItemClick(parent: AdapterView<*>, view: View, position: Int, id: Long) {
        mDoneBtn?.isEnabled = mAdapter!!.selectedCount > 0
        mFolderAdapter?.selectIndex = position
        if (position == 0) {
            supportLoaderManager.restartLoader<Cursor>(LOADER_ALL, Bundle.EMPTY, mLoaderCallback)
            mFolderView?.setText(R.string.folder_all)
            //            if (mIsShowCamera) {
            //                mImageAdapter.setShowCamera(true);
            //            } else {
            //                mImageAdapter.setShowCamera(false);
            //            }
        } else {
            val folder = parent.adapter.getItem(position) as Folder
            if (null != folder) {
                mAdapter?.bindData(folder!!.images)
                mFolderView?.text = folder!!.name
            }
            //            mImageAdapter.setShowCamera(false);
        }
        mRecyclerView?.smoothScrollToPosition(0)
        parent.postDelayed({ mFolderPopupWindow?.dismiss() }, 300)
    }

    private fun preview() {
//        if (0 == mAdapter!!.selectedCount) return
//        val result = getSelectedPath()
//        if (null == result || result.size == 0) return
//        GalleryPreviewActivity.request(this, result, 0)
    }

    private fun getFolder(path: String): Folder? {
        for (f in mFolders.iterator()) {
            if (TextUtils.equals(f.path, path)) {
                return f
            }
        }
        return null
    }

    private val mLoaderCallback = object : LoaderManager.LoaderCallbacks<Cursor> {
        private val IMAGE_PROJECTION = arrayOf(MediaStore.Images.Media.DATA, MediaStore.Images.Media.DISPLAY_NAME,
                MediaStore.Images.Media.DATE_ADDED, MediaStore.Images.Media.MIME_TYPE, MediaStore.Images.Media.SIZE,
                MediaStore.Images.Media._ID)

        override fun onCreateLoader(id: Int, args: Bundle?): Loader<Cursor> {
            if (id == LOADER_ALL) {
                val cursorLoader = CursorLoader(this@GallerySelectActivity,
                        MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_PROJECTION,
                        IMAGE_PROJECTION[4] + ">0 AND " + IMAGE_PROJECTION[3] + "=? OR " + IMAGE_PROJECTION[3] + "=? ",
                        arrayOf("image/jpeg", "image/png"), IMAGE_PROJECTION[2] + " DESC")
                return cursorLoader
            } else if (id == LOADER_CATEGORY) {
                val cursorLoader = CursorLoader(this@GallerySelectActivity,
                        MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_PROJECTION,
                        IMAGE_PROJECTION[4] + ">0 AND " + IMAGE_PROJECTION[0] + " like '%" + args?.getString("path") + "%'", emptyArray(),
                        IMAGE_PROJECTION[2] + " DESC")
                return cursorLoader
            }

            return CursorLoader(this@GallerySelectActivity)
        }

        override fun onLoadFinished(loader: Loader<Cursor>, data: Cursor?) {
            if (data != null && data.count > 0) {
                /**
                 * 如果已经读取过数据，则跳过查询
                 */
                if (mAllImages == null) {
                    mAllImages = ArrayList<File>()
                    data.moveToFirst()
                    do {
                        val path = data.getString(data.getColumnIndexOrThrow(IMAGE_PROJECTION[0]))
                        val image = File(path)
//                        val name = data.getString(data.getColumnIndexOrThrow(IMAGE_PROJECTION[1]))
//                        val dateTime = data.getLong(data.getColumnIndexOrThrow(IMAGE_PROJECTION[2]))
                        if (image.exists()) {
                            mAllImages?.add(image)
                        }
                        if (mFolderAdapter?.count == 0) {
                            // 获取文件夹名称
                            val folderFile = File(path).parentFile
                            if (folderFile != null && folderFile.exists()) {
                                val fp = folderFile.absolutePath
                                val f = getFolder(fp)
                                if (f == null) {
                                    val folder = Folder.create(folderFile.name, fp, image)
                                    folder.images = ArrayList<File>()
                                    folder.images?.add(image)
                                    mFolders.add(folder)
                                } else {
                                    f.images?.add(image)
                                }
                            }
                        }
                    } while (data.moveToNext())
                }
                mAdapter?.bindData(mAllImages)
                if (mFolderAdapter?.count == 0) {
                    mFolderAdapter?.bindData(mFolders)
                }
            }
        }

        override fun onLoaderReset(loader: Loader<Cursor>) {

        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.menu_preview, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when (item.itemId) {
            R.id.action_preview -> preview()
            else -> return super.onOptionsItemSelected(item)
        }

        return true
    }
}