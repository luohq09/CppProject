#!/bin/bash

cur_path=$(dirname $0)
cd $cur_path

PID=$(cat pid)
PORT=$(cat port)

if netstat -nlp | grep ${PORT}.*${PID}>/dev/null; then
    echo serving
elif ps -p ${PID} > /dev/null; then
    echo loading
else
    echo dead
fi