package com.alimin.fk

import androidx.test.InstrumentationRegistry
import androidx.test.runner.AndroidJUnit4
import org.junit.Assert.assertTrue
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class CPPUnitTest {
    private fun checkUnitTest() {
        assertTrue("Enable unit test please.", BuildConfig.ENABLE_UNIT_TEST)
    }

    @Test
    fun testAll() {
        checkUnitTest()
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        FilmKilns.init(appContext)
        assertTrue(CPPTest().testAll())
    }

    @Test
    fun testClassType() {
        checkUnitTest()
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        FilmKilns.init(appContext)
        assertTrue(CPPTest().testClassType())
    }
}

class CPPTest {
    external fun testAll(): Boolean
    external fun testClassType(): Boolean
}