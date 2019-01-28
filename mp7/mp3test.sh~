#!/bin/sh
# mp3test.sh
# Zice Wei
# ECE 2230, Fall 2018

size=$1

echo "Performance Evaluation"
factor="0.5 0.75 0.9 0.95 0.99"
opt="rand seq fold worst"
trails="50000 100000"

for listsize in $trails ; do
    ./lab7 -t $listsize -e -m 65537 -h quad
done 


for listsize in $trails ; do
    ./lab7 -t $listsize -e -m 65537 -h double
done 



for listsize in $trails ; do
    ./lab7 -t $listsize -e -m 65537 -h linear
done 
