@echo off

rem create deploy directory
mkdir deploy
cd deploy
mkdir client
mkdir server
cd ..
echo Created deploy directory

rem build client app
cd RPS-client
mkdir build
cd build
cmake ..
cmake --build .
cd ../..
xcopy "RPS-client/build" "deploy/client" /E /I
echo Client app built in deploy

rem copy additional sources for game
cd deploy
mkdir saved
cd ..
xcopy "RPS-client/saved" "deploy/saved" /E /I
copy "RPS-client/config.xml" "deploy"

rem build and upload server app
cd RPS-server
platformio run --target upload
cd ..
xcopy "RPS-server/.pio/build" "deploy/server" /E /I
echo Server app built in deploy and uploaded to ESP

rem run tests
cd RPS-client/build
ctest -C Debug
cd ../..
echo Tests completed

rem clean build directories from roots
cd RPS-client
rmdir /S /Q build
cd ../RPS-server/.pio
rmdir /S /Q build
cd ../..
del config.xml
echo Clean useless build directories