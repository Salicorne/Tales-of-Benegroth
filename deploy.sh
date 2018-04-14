#!/bin/bash
echo " > cleaning..."
touch app && rm app && rm -rf Build/*
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
