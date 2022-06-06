#!/bin/bash
chmod +x test-res.sh
gcc -pthread p3190212-p3190064-p3190076-res.c -o main && ./main 100 1000
