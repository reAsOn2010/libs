#!/bin/bash
if [ $# -ne 1 ];then
   echo 'too many parameters'
   exit
fi
if [ -f $1 ]; then
   ls -l $1|awk '{print $3,$6,$7,$8}'
   exit
fi
echo 'no such file'
