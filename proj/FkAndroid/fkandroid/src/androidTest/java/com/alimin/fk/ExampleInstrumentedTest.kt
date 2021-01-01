package com.alimin.fk

import androidx.test.InstrumentationRegistry
import androidx.test.runner.AndroidJUnit4
import org.junit.Assert

import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {
    @Test
    fun useAppContext() {
        // Context of the app under test.
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        assertEquals("com.alimin.fk.test", appContext.packageName)
    }

    @Test
    fun testAll() {
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        println(appContext.packageName)
        FilmKilns.init(appContext)
        CPPTest().testAll()
    }
}

class CPPTest {
    external fun testAll()
}
