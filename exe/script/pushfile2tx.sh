#!/bin/bash

if [ $# -lt 1 ] ; then
echo "参数不足，需要指定文件"
echo "USAGE: "$0" file"
exit 1;
fi

echo "Begin transmission"

lftp ftp.swift.qq.com -u fworks_889 -p 9052 --password NE@d216drAg <<!
cd server/LIVE
put $1
exit
!

echo "Completed"
