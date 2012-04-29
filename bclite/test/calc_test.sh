#!/bin/sh

N=3

for ((i=1; $i < $N + 1; i= $i + 1))
do
	cat ./test_$i.txt | ../traverse_test 2>/dev/null| diff - ./answer_$i.txt 
done

