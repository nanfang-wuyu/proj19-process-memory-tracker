#!/bin/bash 

mode=$1
if [ $mode == leak ]
then
    echo input sort order "(tid or size)"
    read order
    echo -n $order > "mode.txt"
    cd src/leakfinder/
    ./LeakMain
    export LD_PRELOAD=./leakfinder.so
    cd ~/project/
    echo You can quit the program and input "'cd src/leakfinder;export LD_PRELOAD=./leakfinder.so;cd ~/project'" to use terminal
    echo You can also press enter to stay here and use simulate terminal
    read key
    while :
    do
        echo -n `pwd`
        echo -n "> "
        read input
        $input
    done
else
    src/processFile/process
fi
