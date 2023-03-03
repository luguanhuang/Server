#!/bin/bash


###判断参数是否正确###
echo "Kill all server"
script_file=$0

work_dir="../"
pid_dir="pid"

cd $work_dir


# StopServer
# $1: xserver
function StopServer()
{
	files=`find $pid_dir/ -name "$1.pid"`
	if [ $1 = "gameserver" -o $1 = "gateserver" ]; then
		files=`find $pid_dir/ -name "$1_*.pid"`
	fi

	for filename in $files
	do
		server_pid=`cat $filename`
		if [ -z "$server_pid" ]; then
			echo "empty pid file ["$1"], delete it"
			rm -rf $filename
			continue
		fi

		server_name=`basename $filename | awk -F '.' '{print $1}'`
		#echo "file: "$filename" [server: "$server_name" ] [pid: "$server_pid"]"

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
		rm -rf $filename
	done
}


StopServer gateserver
StopServer gameserver
StopServer audioserver
StopServer masterserver
StopServer gmserver
StopServer loginserver
StopServer controlserver
StopServer dbserver
StopServer centerserver
StopServer fmserver
StopServer routerserver
StopServer worldserver
StopServer versionserver
StopServer teamserver