#!/bin/bash

./start.sh  worldserver conf/w9_world_conf.xml
./start.sh  routerserver conf/w9_router1_conf.xml 1
./start.sh  gameserver conf/w9_cross_gs1_conf.xml 1
./start.sh  gameserver conf/w9_cross_gs2_conf.xml 2

