#!/bin/bash

set -euxo pipefail

gcc main.c -Wall -m64 -std=c99 -march=x86-64 -static -o main

docker build -t fhrnet/memalloc .
