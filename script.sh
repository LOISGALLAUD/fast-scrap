#!/bin/sh
python free-proxies.py
./bin/proxy-test
./bin/loop-dl.sh "one-piece" "1" "0"
