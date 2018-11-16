#!/bin/bash


echo Compilando...

export SYSTEMC_HOME=/home/confuzo/systemc-2.3.1

g++ -I. -std=c++11 -o processador src/main.cpp -I include/  -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 -lsystemc -lm


