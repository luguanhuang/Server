cd /home/publish/resource
git pull

cd /home/publish/code
git pull

cd server
touch CMakeLists.txt

cd Build/Debug

make

cd ~
./kill.py server
