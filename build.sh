#!/bin/bash

[[ -d dir ]] || mkdir -p build

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

[[ -d release ]] || mkdir -p release

mv build/lynxgame release/lynxgame.out
