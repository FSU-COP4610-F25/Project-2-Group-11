#!/bin/bash
set -e 

gcc -o empty empty.c
gcc -o part1 part1.c

strace -o empty.trace ./empty > /dev/null 2>&1
strace -o part1.trace ./part1 > /dev/null 2>&1

empty_calls=$(grep -cE '^[a-zA-Z_]+\(' empty.trace)
part1_calls=$(grep -cE '^[a-zA-Z_]+\(' part1.trace)

echo "System calls in empty:  $empty_calls"
echo "System calls in part1:  $part1_calls"
