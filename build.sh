#! /bin/bash

chmod a+x ./build.sh

cd ./Build
echo -e "\033[42;31mstart building\033[0m"
make clean
make

cd ..
chmod a+x ./Release/game

echo -e "\033[42;31mDONE!          \033[0m"