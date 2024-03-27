import org.jetbrains.kotlin.konan.properties.Properties
import java.io.FileInputStream

plugins {
    alias(libs.plugins.androidApplication)
    alias(libs.plugins.jetbrainsKotlinAndroid)
}

android {
    namespace = "com.alimin.pic"
    compileSdk = Integer.parseInt(libs.versions.targetVersion.get())

    defaultConfig {
        applicationId = "com.alimin.pic"
        minSdk = Integer.parseInt(libs.versions.minVersion.get())
        targetSdk = Integer.parseInt(libs.versions.targetVersion.get())
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        vectorDrawables {
            useSupportLibrary = true
        }
        ndk {
            stl="libstdc++"
            abiFilters += "arm64-v8a"
//            abiFilters "armeabi-v7a", "arm64-v8a"
        }
    }

    var isDebug = false
    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            isDebug = false
        }
        debug {
            isDebug = true
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
    buildFeatures {
        compose = false
        viewBinding = true
    }
    composeOptions {
        kotlinCompilerExtensionVersion = "1.5.1"
    }
    packaging {
        resources {
            excludes += "/META-INF/{AL2.0,LGPL2.1}"
        }
    }
    val props = Properties().apply {
        load(FileInputStream(File(rootProject.rootDir, "local.properties")))
    }
    val keyAliasAny = props["keyAlias"]
    val keyPasswordAny = props["keyPassword"]
    val storePasswordAny = props["storePassword"]
    if (keyAliasAny != null && keyPasswordAny != null && storePasswordAny != null) {
        signingConfigs {
            getByName("debug") {
                storeFile = file("../lmyooyo.jks")
                keyAlias = keyAliasAny.toString()
                keyPassword = keyPasswordAny.toString()
                storePassword = storePasswordAny.toString()
            }
            if (!isDebug) {
                getByName("release") {
                    storeFile = file("../lmyooyo.jks")
                    keyAlias = keyAliasAny.toString()
                    keyPassword = keyPasswordAny.toString()
                    storePassword = storePasswordAny.toString()
                }
            }
        }
    } else {
        signingConfigs {
            getByName("debug") {
                storeFile = file("../alimin_debug.jks")
                keyAlias = "debug"
                keyPassword = "123456"
                storePassword = "123456"
            }

            if (!isDebug) {
                getByName("release") {
                    storeFile = file("../alimin_debug.jks")
                    keyAlias = "debug"
                    keyPassword = "123456"
                    storePassword = "123456"
                }
            }
        }
    }
}

dependencies {

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.lifecycle.runtime.ktx)
    implementation(libs.androidx.activity.compose)
    implementation(platform(libs.androidx.compose.bom))
    implementation(libs.androidx.ui)
    implementation(libs.androidx.ui.graphics)
    implementation(libs.androidx.ui.tooling.preview)
    implementation(libs.androidx.material3)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
    androidTestImplementation(platform(libs.androidx.compose.bom))
    androidTestImplementation(libs.androidx.ui.test.junit4)
    debugImplementation(libs.androidx.ui.tooling)
    debugImplementation(libs.androidx.ui.test.manifest)
    testImplementation("junit:junit:4.12")
    androidTestImplementation("com.android.support.test:runner:1.0.2")
    androidTestImplementation("com.android.support.test.espresso:espresso-core:3.0.2")
    implementation("com.tencent.bugly:crashreport:4.0.4")
    implementation("pub.devrel:easypermissions:3.0.0")
    implementation("com.microsoft.fluentui:FluentUIAndroid:0.2.1")
    implementation("com.microsoft.design:fluent-system-icons:1.1.164@aar")
    implementation(project(":fkandroid"))
    implementation(project(":hwcommon"))
    implementation(project(":hwmvp"))
}