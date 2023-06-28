package com.alimin.fk

import android.app.Application
import android.content.Context
import androidx.test.runner.AndroidJUnit4
import androidx.test.core.app.ApplicationProvider
import org.junit.Assert.assertTrue
import org.junit.Test
import org.junit.runner.RunWith
import java.io.File
import java.io.FileOutputStream

@RunWith(AndroidJUnit4::class)
class FkModuleTestStarter {
    private fun checkUnitTestEnable() {
        assertTrue("Enable unit test please.", BuildConfig.ENABLE_UNIT_TEST)
    }

    @Test
    fun runAllTest() {
        checkUnitTestEnable()
        System.loadLibrary("FkAndroidTest")
        val appContext = ApplicationProvider.getApplicationContext<Application>()
        FilmKilns.init(appContext)
        initializeAssets(appContext)

        val ret = nativeRunAllTest()
        val result = nativeGetResult()
        val sb = StringBuffer();
        result.forEach {
            sb.append(it)
            sb.append("\n")
        }
        assertTrue("${result.size} CASES FAILED: \n${sb}", ret)
    }

    private external fun nativeRunAllTest(): Boolean
    private external fun nativeRunTestClassType(): Boolean
    private external fun nativeGetResult(): Array<String>

    private fun initializeAssets(context: Context) {
        initializeDir(context)
        context.assets.list("images")?.forEach {
            val dstDir = File(context.externalCacheDir, "images")
            if (!dstDir.exists()) {
                dstDir.mkdirs()
            }
            val dstFile = File(dstDir, it)
            if (!dstFile.exists()) {
                val fos = FileOutputStream(dstFile)

                val buf = ByteArray(1024 * 4)
                val fis = context.assets.open("images/${it}")
                var len = 0
                while (true) {
                    len = fis.read(buf)
                    if (len <= 0) {
                        break
                    }
                    fos.write(buf, 0, len)
                }
            }
        }
    }

    private fun initializeDir(context: Context) {
        var dir = File(context.externalCacheDir, "draft")
        if (dir.exists()) {
            dir.deleteRecursively()
        }
        dir = File(context.externalCacheDir, "mkdirs")
        if (dir.exists()) {
            dir.deleteRecursively()
        }
    }
}