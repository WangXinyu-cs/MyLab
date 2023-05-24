#!/bin/bash

DIR=$BASH_SOURCE
if [ -z "$DIR" ]; then
    DIR=$0
fi

CURRENT_DIR=$(cd `dirname "$DIR"`; pwd)
echo add $CURRENT_DIR/bin to '$LD_LIBRARY_PATH' 

LD_LIBRARY_PATH=$CURRENT_DIR/bin:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
