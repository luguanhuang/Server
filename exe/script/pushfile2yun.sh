#!/bin/bash

if [ $# -lt 2 ] ; then
echo "参数不足，需要指定文件 和 目标机器IP"
echo "USAGE: "$0" file destIP"
exit 1;
fi

echo "Begin transmission"

ftp -n<<!
open $2
user user00 lzg
binary
cd ftp_dir
prompt
tick
put $1
close
bye
!

echo "Completed"
