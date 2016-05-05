# 软件版本依赖
- thrift-0.9.2
- glog-0.3.4
- gtest-0.7.0
- gflag-2.1.2
- sparsehash (google)

# 启动Server
```
cd build
./bin/CannonServer --flagfile=./flags
```
## flags字段
- log_dir
指定log路径
- model_path
指定模型文件路径

## 加载模型
```
cd build/script/
./ccli reload --model-path=./test
```


# Performance
## query qps
- 2w qps

## load time
- 1.3G txt file, ~90s, ~14MB/s
- 45w updates/s
