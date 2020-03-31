#!/usr/bin/env bash

parse() {
	project=$1
    content=$2
    p=`echo $content | sed 's/.*#//g;s/.so.*//g'`
    array=(${p/// })
    idx=${array[0]}
    addr=${array[2]}
    name=${array[3]}
    so=`echo $name | sed 's/.*lib//g'`
    so="lib${so}.so"

    len=
    arm="armeabi-v7a"
    if [ ${#addr} -gt 8 ];then
    	arm="arm64-v8a"
    fi

    result=''

    dirs=`ls $project`
    for i in $dirs
    do
    	dir="${project}/${i}"
    	if [ -d $dir ];then
    		so_file="${dir}/build/intermediates/cmake/debug/obj/${arm}/${so}"
    		if [ -f $so_file ];then
    			result=$(parse_addr $so_file $addr)
    			date=$(date "+%Y-%m-%d %H:%M:%S")
    			echo "${date} #${idx} ${addr} at ${so}:"
    			echo -e "\033[33m ${result} \033[0m"
    		fi
    	fi
    done

    # result=
}

parse_addr() {
	# echo "$1, $2"
	echo `aarch64-linux-android-addr2line -e $1 $2`
	return $?
}

show_help() {
	echo "-p: Parse native track stack."
	echo "-h: For help."
}

while read line
do
	parse $1 "${line}"
done
# if [ "$1" == "-h" ];then
# 	show_help
# elif [ "$1" == "-p" ];then
# 	while read line
# 	do
# 		parse $2 "${line}"
# 	done
# else
# 	show_help
# fi


