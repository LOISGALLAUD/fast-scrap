#!/bin/bash
if [ ! -d "bin" ]; then
    mkdir bin
fi
g++ proxy-test.cpp -o bin/proxy-test -lcpr
g++ fast-scrap.cpp -o bin/fast-scrap -lcpr
