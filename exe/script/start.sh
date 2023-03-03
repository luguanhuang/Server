#!/bin/bash


###判断参数是否正确###
if [ $# != 2 -a $# != 3 ] ; then
echo "USAGE:参数数量为2或3"
echo "e.g.: $0 versionserver conf.xml | loginserver conf.xml | dbserver conf.xml | gateserver conf.xml id | gameserver conf.xml id | masterserver conf.xml | controlserver conf.xml | audioserver conf.xml | idipserver conf.xml | worldserver conf.xml | routerserver conf.xml id | fmserver conf.xml | centerserver conf.xml"
exit 1;
fi

if [ $1 = "gameserver" -o $1 = "gateserver" -o $1 = "routerserver" ]; then
	if [ $# -ne 3 ]; then
		echo "gameserver/gateserver/routerserver 需要3个参数"
		echo "e.g.: $0 gameserver conf.xml id | gateserver conf.xml id | routerserver conf.xml id"
		exit 1;
	fi
fi

script_file=$0

work_dir="../"
pid_dir="pid"

cd $work_dir
mkdir -p $pid_dir


# StartServer
# $1: xserver
# $2: conf.xml
# $3: lineID
function StartServer()
{
	multisvr="no"
	if [ $1 = "gameserver" -o $1 = "gateserver" -o $1 = "routerserver" ]; then
		multisvr="yes"
	fi

	pid=`ps aux | grep $2 | grep -v "grep" | grep -v $script_file | awk -F ' ' '{print $2}'`
	if [ -n "$pid" ]; then
		echo $1" is running, pid: "$pid
		exit 1
	fi

	#start
	echo "start "$1" ..."
	./$1 $2 1>/dev/null 2>&1 &
	#echo $?
	sleep 2

	#rm $1.pid -f
	if [ $multisvr = "yes" ]; then
		ps aux | grep $2 | grep -v "grep" | grep -v $script_file | awk -F ' ' '{print $2}' > $pid_dir/$1"_"$3.pid
		pid=`cat $pid_dir/$1_$3.pid`
	else
		ps aux | grep $2 | grep -v "grep" | grep -v $script_file | awk -F ' ' '{print $2}' > $pid_dir/$1.pid
		pid=`cat $pid_dir/$1.pid`
	fi

	if [ -z "$pid" ]; then
		echo "start "$1" failed"
		exit 1
	fi
	
	if [ $multisvr = "yes" ]; then
		echo "start "$1" successful. pid="$pid
	else
		echo "start "$1" "$3" successful. pid="$pid
	fi
}


# $1: xserver
# 返回参数个数，0表示出错
function CheckServer()
{
	if [ "$1" = "dbserver" ]; then
		return 1
	elif [ "$1" == "masterserver" ]; then
		return 1
	elif [ "$1" == "audioserver" ]; then
		return 1
	elif [ "$1" == "fmserver" ]; then
		return 1
	elif [ "$1" == "loginserver" ]; then
		return 1
	elif [ "$1" == "idipserver" ]; then
		return 1
	elif [ "$1" == "versionserver" ]; then
		return 1
	elif [ "$1" == "controlserver" ]; then
                return 1
	elif [ "$1" == "gameserver" ]; then
		return 2
	elif [ "$1" == "gateserver" ]; then
		return 2
	elif [ "$1" == "routerserver" ]; then
		return 2
	elif [ "$1" == "worldserver" ]; then
		return 1
	elif [ "$1" == "centerserver" ]; then
		return 1
	elif [ "$1" == "teamserver" ]; then
		return 1
	else
		return 0
	fi	
}


CheckServer $1
checkRet=$?
if [ $checkRet -eq 1 ];then
	StartServer $1 $2
elif [ $checkRet -eq 2 ];then
	StartServer $1 $2 $3
else
        echo "server name: "$1" error!"
        exit 1;
fi
