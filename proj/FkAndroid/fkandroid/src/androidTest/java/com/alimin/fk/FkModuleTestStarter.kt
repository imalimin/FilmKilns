package com.alimin.fk

import androidx.test.InstrumentationRegistry
import androidx.test.runner.AndroidJUnit4
import org.junit.Assert.assertTrue
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class FkModuleTestStarter {
    private fun checkUnitTestEnable() {
        assertTrue("Enable unit test please.", BuildConfig.ENABLE_UNIT_TEST)
    }

    @Test
    fun runAllTest() {
        checkUnitTestEnable()
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        FilmKilns.init(appContext)
        val ret = nativeRunAllTest()
        val result = nativeGetResult()
        val sb = StringBuffer();
        result.forEach {
            sb.append(it)
            sb.append("\n")
        }
        assertTrue("${result.size} CASES FAILED: \n${sb}", ret)
    }

    @Test
    fun runTestClassType() {
        checkUnitTestEnable()
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        FilmKilns.init(appContext)
        assertTrue(nativeRunTestClassType())
    }

    private external fun nativeRunAllTest(): Boolean
    private external fun nativeRunTestClassType(): Boolean
    private external fun nativeGetResult(): Array<String>
}