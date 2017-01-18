# 编译环境docker镜像

`docker run -itd --name=cannon-dev --net=none -v /root/.ssh:/root/.ssh registry.4paradigm.com/cannon-build /usr/sbin/sshd -D`

`sudo pipework br0 cannon-dev 172.27.4.19/20@172.27.0.1`

`ssh root@172.27.4.19`

# 软件版本依赖
- thrift-0.9.3
- glog-0.3.4
- gtest-0.7.0
- gflag-2.1.2
- boost-1.62

## flags字段
- port
CannonServer监听端口号
- kv_path
待加载kv数据文件
- kv_schema_path
待加载kv的schema的路径（json格式）

# 编译CannonServer
make all

# 部署CannonServer
tar -zxvf ./release/CannonServer-xxxx.tar.gz -C ${deploy-dir}/

# 启动Server
```
cd ${deploy-dir}
sh start.sh --prot=xxxx --kv_path=xxxx --kv_schema_path=xxxx
```

# Performance
https://wiki.4paradigm.com/pages/viewpage.action?pageId=11568323
