#!/bin/bash

if [ $# -lt 1 ] ; then
echo "参数不足，需要指定server根目录"
echo "USAGE:"
echo "Make full pack with all files:      "$0" [server_dir] [--all]"
echo "Make full pack with normal files:   "$0" [server_dir]"
echo "Make incremental pack:              "$0" [server_dir] [--inc] [compare_dir]"
exit 1;
fi


build_type="Debug"
#build_type="Release"

# check $1
if [ ! -d "$1/Build/"$build_type"/bin" ]; then
	echo "Error: $1/Build/"$build_type"/bin doesn't exist"
	exit 1
fi


# check $2
# check $3
svr_dir=$1
echo "Packing info:"
echo "[Build type: "$build_type"]"
echo "[Server dir: "$1"]"

inc_mode="false"
compare_dir=""
is_all="false"
pack_make_time=""

if [ $# -eq 1 ]; then
	is_all="false"
	echo "[Pack  mode: Full packing mode, with normal files]"
elif [ $# -eq 2 ]; then
	if [ $2 = "--all" ]; then
		echo "[Pack  mode: Full packing mode, with all files]"
		is_all="true"
	elif [ $2 = "--inc" ]; then
		echo "use --inc need param 3"
		exit 1
	else
		echo "Invalid param 2, should be [--all]"
		exit 1
	fi
elif [ $# -eq 3 ]; then
	if [ $2 = "--inc" ]; then
		echo "[Pack  mode: Incremental packing mode]"
		inc_mode="true"
	else
		echo "Invalid param 2, should be [--inc]"
		exit 1
	fi

	compare_dir=$3
	if [ ! -d $compare_dir ]; then
		echo "Error: dir ["$compare_dir"] doesn't exist"
		exit 1
	fi
else
	echo "Param count error"
	exit 1
fi


# sleep to show message
echo ""
sleep 5


#$1: dest dir
function CopyFileToDir()
{
	#server
	cp $svr_dir/Build/$build_type/bin/*server $1 -v
	cp $svr_dir/Build/$build_type/bin/gmtool $1 -v
	cp $svr_dir/Build/$build_type/bin/combinetool $1 -v

	for sv in csconf dbconf gsconf gateconf loginconf idipconf rsconf audioconf versionconf worldconf
	do
		cp $svr_dir/exe/$sv $1/$sv -rfLv
	done

	cp $svr_dir/exe/ms_reload.txt $1 -v
	cp $svr_dir/exe/world_reload.txt $1 -v

	#cp $svr_dir/exe/conf.template $1 -rv
	#cp $svr_dir/exe/script $1 -rv

	#rm files
	rm $1/loginconf/gateip.txt -f
	rm $1/loginconf/gateip.xml -f

	rm $1/dbconf/*.sql -f
	rm $1/csconf/dbinit.sql -f
	rm $1/loginconf/dbinit.sql -f
	rm $1/worldconf/dbinit.sql -f

if [ $is_all = "false" ];then
	rm $1/dbconf/LoginLimit.txt -f
	rm $1/loginconf/whitelist.txt -f
	rm $1/loginconf/ios_audit_version.txt -f

	rm $1/gsconf/serveronly/tlog_config.xml -f
	rm $1/gsconf/serveronly/tss_sdk_conf.xml -f
	rm $1/gsconf/serveronly/voipparam.xml -f
	rm $1/gsconf/serveronly/fmlist.txt -f
	rm $1/gsconf/serveronly/whitelist.txt -f
	rm $1/gsconf/serveronly/usewhitelist.txt -f
	rm $1/gsconf/serveronly/FunctionOpen.txt -f
	rm $1/gsconf/serveronly/queuing.txt -f
	rm $1/gsconf/serveronly/idipclosesystem.txt -f
	rm $1/gsconf/serveronly/channelclosesystem.txt -f
	rm $1/gsconf/serveronly/apptag.txt -f
	rm $1/loginconf/backflow.txt

	#rm $1/gsconf/serveronly/Version.txt -f
fi
}


#$1: files dir
function PackDir()
{
	pack_make_time=`date +%Y%m%d%H%M`;
	tar_file=lzgjxupdate.$pack_make_time.tar.gz
	tar -hzcvf $tar_file --exclude=*.svn  $1
	md5_str=`md5sum $tar_file | awk -F ' ' '{print $1}'`
	echo "" > $tar_file"."$md5_str
}

#$1: dist.new, new
#$2: dist, old
function Travel()
{
	for file in `ls $1`
	do
		local new_filePath=$1"/"$file
		local old_filePath=$2"/"$file

		if [ -f $new_filePath ];then
			local md5_new=`md5sum $new_filePath | awk -F ' ' '{print $1}'`
			if [ -f $old_filePath ]; then
				local md5_old=`md5sum $old_filePath | awk -F ' ' '{print $1}'`
				if [ $md5_new = $md5_old ]; then
					echo $new_filePath" : ["$md5_new"] ------ "$old_filePath": ["$md5_old"], remove it"
					rm $new_filePath
				else
					echo "keep file: "$new_filePath
					#cp $new_filePath $old_filePath -f
				fi
			fi

			continue
		fi

		if [ -d $new_filePath ];then
			echo "--------------dir: "$new_filePath
			if [ -d $old_filePath ]; then
				Travel $new_filePath $old_filePath
			else
				echo "keep dir: "$new_filePath
				#cp $new_filePath $old_filePath -rf
			fi
		fi
	done
}

#$1: dir
function RemoveEmptyDir()
{
	local hasFiles=0
	for file in `ls $1`
	do
		local filePath=$1"/"$file

		if [ -f $filePath ];then
			hasFiles=1
			continue
		fi

		if [ -d $filePath ];then
			RemoveEmptyDir $filePath
			local result=$?
			if [ $result -eq 0 ]; then
				rm $filePath -rf
				echo "remove empty dir ["$filePath"]"
			else
				hasFiles=1
			fi
		fi
	done
	return $hasFiles
}

if [ $inc_mode = "false" ];then
	rm dist.temp -rf
	mkdir -p dist.temp
	CopyFileToDir dist.temp

	rm lzgjx -rf
	mv dist.temp lzgjx
	PackDir lzgjx
	if [ "$pack_make_time" = "" ]; then
		echo "get pack_make_time failed, exit"
		exit 1
	fi

	mv lzgjx dist-$pack_make_time
else
	rm dist.new -rf
	mkdir -p dist.new
	CopyFileToDir dist.new
	
	rm dist.temp -rf
	cp dist.new dist.temp -rf

	echo ""
	echo "Copy file finished"
	Travel dist.new $compare_dir
	echo "---------------------"
	echo "Travel 1 finished"
	RemoveEmptyDir dist.new

	lNewFileCount=`ls dist.new | wc -l`
	if [ $lNewFileCount -eq 0 ]; then
		rm dist.new -rf
		rm dist.temp -rf
		echo "============ No any file changed, don't generate pack files ============="
	else
		rm lzgjx -rf
		mv dist.new lzgjx
		PackDir lzgjx
		if [ "$pack_make_time" = "" ]; then
			echo "get pack_make_time failed, exit"
			exit 1
		fi

		rm lzgjx -rf
		mv dist.temp dist-$pack_make_time
	fi
fi


echo "All Finished"
