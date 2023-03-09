# Pre-requisites

## CMake

`sudo apt install cmake`

## GCC compiler (+ build essentials)

`sudo apt install build-essential`

## Protobuf compiler

1. `sudo apt install protobuf-compiler`
2. Generate protocol implementation files:
    - navigate to the protobuf definition folder: `cd /home/lgh Server/Server_Tools/protobuf`
    - generate files using the installed protobuf compiler (create output directory if not exist):
        - `protoc --cpp_out=../../protocol/pb enum.proto`
        - `protoc --cpp_out=../../protocol/pb project.proto`
        - `protoc --cpp_out=../../protocol/pb project_client.proto`

## CURL developer libraries

`sudo apt install libcurl4-openssl-dev`

## MySQL developer libraries

`sudo apt install libmysqlclient-dev`