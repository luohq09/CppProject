#!/bin/bash

cur_path=$(dirname $0)
cd $cur_path


if [[ "$@" == "--version" ]]; then
    ./CannonServer --version
    exit 0
fi

nohup ./CannonServer --log_dir=../logs --logbufsecs=0 $1 $2 $3 2>&1 1>../logs/std.log &

echo $! > pid

PORT=9090
for i in "$@"
do
case $i in
    -port=*|--port=*)
    PORT="${i#*=}"
    shift # past argument=value
    ;;
    *)
            # unknown option
    ;;
esac
done

echo ${PORT} > port
