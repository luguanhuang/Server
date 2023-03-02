#!/bin/bash

for file in `find .`
do 
	if [ -h "$file"]; then
		echo $file
	fi
done
