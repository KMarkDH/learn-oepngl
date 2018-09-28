#! /bin/bash

chmod a+x ./run.sh

cd ./Build
echo $pwd
make clean
make

cd ..
./Release/game