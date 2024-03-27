plugins {
    id("com.android.library")
//    id("kotlin-parcelize")
    alias(libs.plugins.jetbrainsKotlinAndroid)
}

android {
    namespace = "com.lmy.common"
    compileSdk = Integer.parseInt(libs.versions.targetVersion.get())

    defaultConfig {
        minSdk = Integer.parseInt(libs.versions.minVersion.get())
        targetSdk = Integer.parseInt(libs.versions.targetVersion.get())

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        vectorDrawables {
            useSupportLibrary = true
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
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
}

dependencies {

    implementation(libs.androidx.core.ktx)
    implementation(libs.google.material)
    implementation(libs.facebook.fresco)
//    implementation "org.jetbrains.kotlin:kotlin-stdlib-jdk7:$kotlin_version"
//    implementation "org.jetbrains.kotlin:kotlin-reflect:$kotlin_version"
//    implementation "androidx.appcompat:appcompat:$androidx_version"
//    implementation "androidx.core:core-ktx:$androidx_version"
//    implementation "com.google.android.material:material:$material_version"
//    implementation 'com.facebook.fresco:fresco:2.6.0'
//    implementation 'androidx.coordinatorlayout:coordinatorlayout:1.1.0'
//    implementation 'androidx.recyclerview:recyclerview:1.1.0'
}