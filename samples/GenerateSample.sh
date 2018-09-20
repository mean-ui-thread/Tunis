#!/bin/bash

if [ "$1" != "" ]; then
    cmake -DSAMPLE_NAME=$1 -P GenerateSample.cmake
else
    echo "You must provide a sample name."
fi
