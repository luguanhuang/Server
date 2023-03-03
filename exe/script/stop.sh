#!/bin/bash


###判断参数是否正确###
if [ $# != 1 -a $# != 2 ] ; then
echo "USAGE:参数数量为1或2"
echo "e.g.: $0 loginserver | dbserver | gateserver id | gameserver id | masterserver | audioserver | gmserver | worldserver | routerserver id | fmserver | centerserver"
exit 1;
fi


if [ $1 = "gameserver" -o $1 = "gateserver" -o $1 = "routerserver" ]; then
	if [ $# -ne 2 ]; then
		echo "gameserver/gateserver/routerserver 需要2个参数"
		echo "e.g.: $0 gameserver id | gateserver id | routerserver id"
		exit 1;
	fi
fi


pid_file="$1.pid"
server_name=$1
if [ $# -eq 1 ];then
	echo "Kill server: "$1
else
	pid_file="$1_$2.pid"
	server_name="$1_$2"
	echo "Kill server: "$1"_"$2
fi


script_file=$0
work_dir="../"
pid_dir="pid"

cd $work_dir


# StopServer
# $1: xserver
function StopServer()
{
	files=`find $pid_dir/ -name "$pid_file"`
	if [ -z "$files" ]; then
		echo $pid_file" is not exist"
		exit 1
	fi

	
	server_pid=`cat $pid_dir/$pid_file`
	if [ -z "$server_pid" ]; then
		echo "empty pid file ["$pid_file"], delete it"
		rm -rf $pid_dir/$pid_file
		exit 1
	fi

	#echo "file: "$pid_file" [server: "$1" ] [pid: "$server_pid"]"

	check_pid=`ps aux | awk -F ' ' '{print $2}' | grep $server_pid -w | grep -v "grep"`
	if [ -z "$check_pid" ]; then
		echo $server_name" is not running"
	else
		kill -2 $server_pid
		echo "stopping "$server_name" ["$server_pid"] ..."

		until [ -z "$check_pid" ]
		do
			sleep 2
			echo "waitting ..."
			check_pid=`ps aux | awk -F ' ' '{print $2}' | grep $server_pid -w | grep -v "grep"`
		done

		sleep 2
		echo $server_name " stopped"
	fi
	echo ""
	rm -rf $pid_dir/$pid_file
}


StopServer $1
