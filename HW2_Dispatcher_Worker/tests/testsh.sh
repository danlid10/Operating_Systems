#!/bin/bash

echo "should run for 1 seconds"
time ./hw2 test1.txt 1 1 0
echo "should run for 1 seconds"
time ./hw2 test1.txt 1 1 1
echo "should run for 30 seconds"
time ./hw2 test2.txt 1 1 0
echo "should run for 20 seconds"
time ./hw2 test2.txt 2 1 0
echo "should run for 13 seconds"
time ./hw2 test3.txt 2 1 0
echo "should run for 20 seconds"
time ./hw2 test4.txt 1 1 0
echo "should run for 15 seconds"
time ./hw2 test4.txt 2 1 0
echo "should run for 20 seconds"
time ./hw2 test5.txt 10 10 0
echo "should count 2"
time ./hw2 test7.txt 10 10 0
echo "should count to 10000"
time ./hw2 test8.txt 10 10 0
echo "should count to 5000"
time ./hw2 test9.txt 10 10 0