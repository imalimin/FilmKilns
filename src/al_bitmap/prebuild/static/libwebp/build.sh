#!/usr/bin/env bash
ANDROID_SDK=/Users/lmy/Library/Android/sdk
ANDROID_NDK=/Users/lmy/Library/Android/android-ndk-r14b

build() {
    ARCH=$1
    ABI=
    if [[ "$ARCH" = "armv7a" ]]; then
        echo "+--------------------------+"
        echo "|----webp build armv7a-----|"
        echo "+--------------------------+"
        ABI="armeabi-v7a"
    elif [[ "$ARCH" = "arm64" ]]; then
        echo "+--------------------------+"
        echo "|----webp build arm64 -----|"
        echo "+--------------------------+"
        ABI="arm64-v8a"
    elif [[ "$ARCH" = "x86" ]]; then
        echo "+--------------------------+"
        echo "|---- webp build x86  -----|"
        echo "+--------------------------+"
        ABI="x86"
    else
        echo "Need a arch param"
        exit 1
    fi
    ${ANDROID_SDK}/cmake/3.6.4111459/bin/cmake -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=${ABI} \
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="./product/${ABI}" \
    -DCMAKE_BUILD_TYPE="Release" \
    -DANDROID_PLATFORM="android-21" \
    -DANDROID_TOOLCHAIN="clang" \
    -DWEBP_ENABLE_SIMD="ON" \
    -DWEBP_BUILD_ANIM_UTILS="OFF" \
    -DWEBP_BUILD_CWEBP="OFF" \
    -DWEBP_BUILD_DWEBP="OFF" \
    -DWEBP_BUILD_GIF2WEBP="OFF" \
    -DWEBP_BUILD_IMG2WEBP="OFF" \
    -DWEBP_BUILD_VWEBP="OFF" \
    -DWEBP_BUILD_WEBPINFO="OFF" \
    -DWEBP_BUILD_WEBPMUX="OFF" \
    -DWEBP_BUILD_EXTRAS="OFF" \
    -DWEBP_BUILD_WEBP_JS="OFF" \
    -DWEBP_ENABLE_SWAP_16BIT_CSP="OFF" \
    -DWEBP_ENABLE_SWAP_16BIT_CSP="OFF"

    make -j4
}

build $1