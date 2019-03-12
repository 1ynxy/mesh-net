#!/bin/bash

if [ $1 == "clean" ]; then
    rm -R build/*
fi

cd build

cmake ..

if [ ! -f ./Makefile ]; then
    echo Makefile not created correctly
    exit
fi

make

if [ ! -f ./lynxgame ]; then
    echo "executable not built correctly"
    exit
fi

cd ..

chmod u+x build/lynxgame

if [ ! -d release ]; then
    mkdir -p release
fi

mv build/lynxgame release/lynxgame

rsync release/lynxgame lynxy@192.168.0.15:/home/lynxy/lynxgame/lynxgame
