#!/bin/bash

echo '指定测试用例如..test_data/sample_1.tsp '
read  example
read  -p 'resule file name:' out
./LKSolver < $example >  $out
#./LKSolver < $1 > $2
