# Raspberry-Pi-Monitoring-system
![](https://img.shields.io/badge/Powered%20Py-Linux-bule.svg) &ensp;&ensp;&ensp;
![](https://img.shields.io/badge/Language-C%2B%2B%2FShell-blue.svg)

这是一个大型(/滑稽）树莓派集群系统的监控系统


由Master端和Client端组成：
## Master
开启多进程，每5s收集所有Client的数据。
## Client
开启多线程调用shell ， 每5s更新一次本机信息。
## config

### client.conf
```
name=pi7                 //名字  
port=8850                //开启端口  
Master=192.168.1.47      //本机IP地址  
Master_data_port=8851    //Master的端口  
readpath=../log/         //log所在路径  
```
### master.conf
```
name=Master;             //名字  
port=8850				         //开启端口  
writepath=../log/		     // 写入log路径  
client_num=8			       //Client的数量  
client1=192.168.1.40     //client所在的IP地址  
client2=192.168.1.41  
client3=192.168.1.42  
client4=192.168.1.43  
client5=192.168.1.44  
client6=192.168.1.45  
client7=192.168.1.46  
client8=192.168.1.47  




