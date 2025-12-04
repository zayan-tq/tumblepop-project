#!/bin/bash

# Check arguments
if [ "$1" == "--compile" ]; then
    echo "Compiling Tumble-POP..."
    g++ -c tumblepop.cpp
    g++  tumblepop.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
    echo "Compilation finished."
    ./sfml-app
    echo "Running"

fi