#!/bin/bash

RES_PATH=$1

echo "Resource path: " $1

mkdir Build
mkdir Build/Debug
mkdir Build/Debug/bin
mkdir Build/Release

cp -r exe/conf.template Build/Debug/bin
cp exe/conf_create.py Build/Debug/bin
cp exe/stop 	Build/Debug/bin
cp exe/start 	Build/Debug/bin
cp exe/status 	Build/Debug/bin

pushd .
cd Build/Debug/bin

ln -s ../../../exe/loginconf .
ln -s ../../../exe/dbconf . 
ln -s ../../../exe/gateconf . 
ln -s ../../../exe/gsconf . 
ln -s ../../../exe/audioconf . 
ln -s ../../../exe/versionconf . 

popd

# pushd .
# cd exe/gsconf
# ln -s $RES_PATH/XProject/Assets/Table table
# cd table
# ln -s $RES_PATH/XProject/Assets/Resources/Table/AITree .
# ln -s $RES_PATH/XProject/Assets/Resources/Table/Level .
# ln -s $RES_PATH/XProject/Assets/Resources/Table/SceneBlock .
# ln -s $RES_PATH/XProject/Assets/Resources/Table/WayPoint .
# ln -s $RES_PATH/XProject/Assets/Resources/Table/SuperRisk .
# cd ..

# ln -s $RES_PATH/XProject/Assets/Resources/CutScene . 
# ln -s $RES_PATH/XProject/Assets/Resources/SkillPackage . 
# ln -s $RES_PATH/XProject/Assets/Editor/EditorResources/Server/Curve . 

# popd

# pushd .
# cd Build/Debug
# cmake -DCMAKE_BUILD_TYPE=Debug ../..
# make -j4
# popd

pushd .
cd Build/Release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make -j1
popd

