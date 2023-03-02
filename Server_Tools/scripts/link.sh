#!/bin/bash

RES_PATH=$1

echo "Resource path: " $1

if [ ! -d "Build" ]; then
	mkdir Build
fi
if [ ! -d "Build/Debug" ]; then
	mkdir Build/Debug
fi
if [ ! -d "Build/Debug/bin" ]; then
	mkdir Build/Debug/bin
fi
if [ ! -d "Build/Release" ]; then
	mkdir Build/Release
fi

cp exe/*.xml Build/Debug/bin

pushd .
cd Build/Debug/bin

ln -s ../../../exe/logconf .
ln -s ../../../exe/loginconf .
ln -s ../../../exe/dbconf .
ln -s ../../../exe/gateconf .
ln -s ../../../exe/gsconf .

popd

pushd .
cd exe/gsconf
ln -s $RES_PATH/XProject/Assets/Resources/Table table
ln -s $RES_PATH/XProject/Assets/Resources/SkillPackage .
ln -s $RES_PATH/XProject/Assets/Resources/CutScene .
if [ ! -d "Server" ]; then
	mkdir Server
fi
cd Server
ln -s $RES_PATH/XProject/Assets/Editor/EditorResources/Server/Curve .
popd
