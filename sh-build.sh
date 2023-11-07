#!/bin/bash
if [ ! -d "bin" ]; then
    mkdir bin
fi
g++ 1-proxy-test.cpp -o bin/proxy-test -lcpr
g++ 2-dl-im.cpp -o bin/dl-im -lcurl
