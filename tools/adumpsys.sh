#!/bin/bash

pkt=$1
if [ "$pkt" = "" ]; then
  echo "Input a package path pls."
  exit 1
fi

while [ 0 -le 1 ]
do
 adb shell dumpsys meminfo $1 | grep -E "Java Heap:|Native Heap:|Code:|Graphics:|Private Other:|System:|TOTAL:"
 echo "---END---"
 sleep 3
done
