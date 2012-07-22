#!/bin/sh
make
./strings.py ./rfc1166.txt res.txt
./string_test

