[中文](README.md) ||  [English](README.en.md)  
# XEngine_Storage
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
只要仓库不是在暂停状态,那么就会有人一直维护和开发,请放心使用  

## 介绍
c c++ 存储服务 c c++ 文件存储服务  
c c++ file storage service  
这是一个上传下载服务API网关,不是文件同步服务,也不是云存储.  
这个服务的主要目的是为了给有上传下载服务业务需求的用户使用(比如需要开发上传下载服务的开发人员).  
此服务器更偏向于存储传输与API接口管理.  
目前最好的HTTP存储服务,比NGINX更优秀,更方便管理,代码更简洁.持续维护中...  

## 软件特性
基于libXEngine开发并实现的一套简洁高性能跨平台网络存储服务  
软件特性:  
1. 支持HTTP协议上传和下载(采用PUT和GET)  
2. 支持HTTP API接口事件通知与管理  
3. 支持HTTP加密与证书  
4. 采用数据库(MYSQL和SQLITE)记录信息  
5. 支持分布式  
6. 支持HTTP验证  
7. 支持详尽的日志和配置信息  
8. 支持传输速率控制
9. 支持任务开始结束代理转发  
10. 支持P2P  
11. BT(规划中)  
12. 数据分发  
13. 支持秒传和断点续传  
14. 支持上传和下载重定向  
15. 支持全速模式和限速模式

## 安装教程

#### XEngine环境
必须安装XEngine,版本需要V7.38或者以上版本,安装XEngine可以参考其Readme文档  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

###### 快速部署
git clone https://gitee.com/xyry/libxengine.git 或者 git clone https://github.com/libxengine/xengine.git  
window执行XEngine_WINEnv.bat 脚本.  
Linux执行:sudo ./XEngine_LINEnv.sh -i 3  
macos执行:./XEngine_LINEnv.sh -i 3  

#### Windows
使用VS打开并且编译,支持WINDOWS 7SP1以上系统  
直接运行即可,使用XEngine_AuthorizeApp

#### Linux
Linux使用Makefile编译,UBUNTU20.04 x64或者CENTOS8 x64  
在控制台运行

#### Macos
使用makefile编译,控制台运行,需要mac 12以及以上版本  
在控制台运行

##### 编译命令
在XEngine_Source目录下执行命令  
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译  

#### 使用说明

1.  切换到MASTER分支
2.  下载代码
3.  编译
4.  安装
5.  运行
6.  通过curl或postman 等三方工具测试上传下载

## 目录结构
- XEngine_Docment    文档目录结构  
- XEngine_Release    安装目录结构  
- XEngine_Source     源代目录结构  
- XEngine_APPClient  客户端演示代码  
- XEngine_SQLFile    数据库脚本文件
  
## 示例
上传文件: curl -d 'hello xengine' -X POST "http://192.168.1.8:5102/api?filename=hello.txt&storeagekey=storagekey1"  
下载文件: curl -X GET "http://192.168.1.8:5101/storagekey1/hello.txt"  

## 秒传实现  
秒传的实现不是靠服务器实现的,而是靠客户端实现的.  
上传秒传的实现是先通过HASH查询文件是否在服务器,如果存在就不上传直接提示客户端上传成功.  
下载秒传的实现是先通过HASH查询本地文件保存路径,如果存在就直接下载完成.  

## 关于P2P
P2P分布式下载已经支持,不过目前只能在局域网中,暂时不支持跨网段,需要等待开发完毕  
P2P分布式下载与其他下载工具的超线程下载一样,原理是使用HTTP RANGE字段实现.各位可以通过libcurl等库实现此功能.  

## 当前任务
本地网络接口删除  
上传目录控制,防止超过权限的上传动作  
客户端连接数限制  
P2P广域网文件查找与下载支持  
分布式数据库存储

## 其他问题  
你可以参考docment目录下的文档.里面包含了API协议和服务说明.  
jsoncpp模块在使用的时候编译运行时库选择必须一致.否则会崩溃.  

## 测试服务器
地址:app.xyry.org 或者 159.75.200.173  
端口:HTTP业务端口 5100,下载 5101,上传 5102  

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request  

## 关注我们
如果你觉得这个软件对你有帮助,请你给我们一个START吧

## 提交问题

如果你有问题或者发现程序有BUG和意见,可以在issues中提交  