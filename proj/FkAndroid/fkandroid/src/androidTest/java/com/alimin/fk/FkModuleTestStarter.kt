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
        assertTrue(nativeRunAllTest())
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
}