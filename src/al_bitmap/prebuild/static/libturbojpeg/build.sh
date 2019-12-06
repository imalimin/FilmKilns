#!/usr/bin/env bash
ANDROID_SDK=/Users/lmy/Library/Android/sdk
ANDROID_NDK=/Users/lmy/Library/Android/android-ndk-r14b

build() {
    ARCH=$1
    ABI=
    if [[ "$ARCH" = "armv7a" ]]; then
        echo "+--------------------------------+"
        echo "|----jpeg-turbo build armv7a-----|"
        echo "+--------------------------------+"
        ABI="armeabi-v7a"
    elif [[ "$ARCH" = "arm64" ]]; then
        echo "+--------------------------------+"
        echo "|----jpeg-turbo build arm64 -----|"
        echo "+--------------------------------+"
        ABI=arm64-v8a
    elif [[ "$ARCH" = "x86" ]]; then
        echo "+--------------------------------+"
        echo "|---- jpeg-turbo build x86  -----|"
        echo "+--------------------------------+"
        ABI=x86
    else
        echo "Need a arch param"
        exit 1
    fi
    ${ANDROID_SDK}/cmake/3.6.4111459/bin/cmake -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=$ABI \
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="./product/${ABI}" \
    -DCMAKE_BUILD_TYPE="Release" \
    -DANDROID_PLATFORM="android-21" \
    -DANDROID_TOOLCHAIN="clang" \
    -DENABLE_STATIC="TRUE" \
    -DENABLE_SHARED="FALSE"

    make -j4
}

build $1