#!/bin/bash

cd ~/dist

#cp ~/server/bin/*.xml .
cp ~/code/server/lib/libnetwork.so .
cp ~/code/server/Build/Debug/bin/*server .

for sv in logconf dbconf gsconf gateconf loginconf gmconf webconf
do
	if [ ! -L $sv ] ; then
		ln -s ~/code/server/exe/$sv $sv
	fi
done

#cp /usr/local/lib/libprofiler.so.0 .
#cp /usr/local/lib/libtcmalloc.so.4 .

cd ~
if [ -a dn_*.tar ] 
then
	rm dn_*.tar
fi

cd ~
tar -hzcvf dn_`date +%Y%m%d`.tar.gz --exclude=*.svn  dist
