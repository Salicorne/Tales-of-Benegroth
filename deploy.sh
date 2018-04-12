#!/bin/bash
echo " > cleaning..."
touch app && rm app
cd ./Build
echo " > executing CMake..."
cmake ..
echo " > executing make..."
make clean
make
echo " > starting game !"
cp app ..
cd ..
./app
