#!/bin/sh
make
echo "testing before hook"
./test_write
echo "writed file is:"
cat test.txt
echo ""

echo "testing after hook"
bash -c 'export LD_RELOAD="./libruyk.so"; ./test_write'
echo "writed file is:"
cat test.txt
echo ""
rm test.txt
