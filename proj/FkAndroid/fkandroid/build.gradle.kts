plugins {
    id("com.android.library")
    alias(libs.plugins.jetbrainsKotlinAndroid)
}

android {
    namespace = "com.alimin.fk"
    compileSdk = Integer.parseInt(libs.versions.targetVersion.get())

    defaultConfig {
        minSdk = Integer.parseInt(libs.versions.minVersion.get())
        targetSdk = Integer.parseInt(libs.versions.targetVersion.get())

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        vectorDrawables {
            useSupportLibrary = true
        }
        externalNativeBuild {
            cmake {
                arguments(
                    "-DANDROID_ARM_NEON=TRUE", "-DANDROID_CPP_FEATURES=rtti exceptions",
                    "-DENABLE_FK=TRUE",
                    "-DOPTIMIZE_CLASS_TYPE=TRUE",
                    "-DANDROID_ARM_NEON=ON"
                )
            }
        }
        ndk {
            stl = "libstdc++"
            abiFilters.add("arm64-v8a")
//            abiFilters "armeabi-v7a", "arm64-v8a"
        }
        sourceSets {
            getByName("main").jniLibs.srcDirs(
                "../../../src/third_party/protobuf/shared",
                "../../../src/third_party/skia/shared"
            )
        }
        buildConfigField("boolean", "ENABLE_UNIT_TEST", "false")
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            buildConfigField("boolean", "ENABLE_UNIT_TEST", "false")
            externalNativeBuild {
                cmake {
//                    cppFlags "-fprofile-arcs -ftest-coverage --coverage -fprofile-instr-generate"
                    arguments.add("-DENABLE_G_TEST=FALSE")
                }
            }
        }
        debug {
            buildConfigField("boolean", "ENABLE_UNIT_TEST", "true")
            externalNativeBuild {
                cmake {
//                    cppFlags "-fprofile-arcs -ftest-coverage --coverage -fprofile-instr-generate"
                    arguments.add("-DENABLE_G_TEST=TRUE")
                }
            }
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
        buildConfig = true
    }
    composeOptions {
        kotlinCompilerExtensionVersion = "1.5.1"
    }
    packaging {
        resources {
            excludes += "/META-INF/{AL2.0,LGPL2.1}"
        }
    }

    externalNativeBuild {
        cmake {
            path = file("CMakeLists.txt")
//            version "3.10.2.4988404"
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
    api(libs.google.protobuf)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
    androidTestImplementation(platform(libs.androidx.compose.bom))
    androidTestImplementation(libs.androidx.ui.test.junit4)
    debugImplementation(libs.androidx.ui.tooling)
    debugImplementation(libs.androidx.ui.test.manifest)
}