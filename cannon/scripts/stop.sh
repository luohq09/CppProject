#!/bin/bash

cur_path=$(dirname $0)
cd $cur_path
cat pid | xargs kill