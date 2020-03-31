#!/usr/bin/env bash

adb logcat | grep -G "#[0-9]\+ pc"
