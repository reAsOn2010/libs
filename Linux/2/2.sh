#!/bin/bash
if [ $# -gt 1 ]; then
	echo 'too many parameters'
	exit
fi
if [ -d $1 ]; then
	echo "remove the directory $1"
	rm -rf $1
	exit
fi
if [ -f $1 ]; then
	echo "remove the file $1"
	rm -f $1
	exit
fi
echo 'no such file or directory'
