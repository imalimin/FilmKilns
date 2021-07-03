#!/bin/bash
ADB_CMD='adb'
RESULT=`uname -a | tr 'A-Z' 'a-z' | grep 'microsoft'`
if [[ $RESULT != '' ]];then
	ADB_CMD='adb.exe'
	echo 'microsoft'
fi
${ADB_CMD} logcat | grep -G "F DEBUG"
