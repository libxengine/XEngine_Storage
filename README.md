# XEngine_Storage

## 介绍
c c++ 存储服务 c c++ 文件存储服务  
c c++ file storage service  
这是一个上传下载服务API网关,不是文件同步服务,也不是云存储.  
这个服务的主要目的是为了给有上传下载服务业务需求的用户使用(比如需要开发上传下载服务的开发人员).  

## 软件特性
基于libXEngine开发并实现的一套简洁高性能跨平台网络存储服务  
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
软件特性:  
1. 支持HTTP协议上传和下载(采用PUT和GET)
2. 支持HTTP API接口事件通知与管理(规划中)
3. 支持加密传输(规划中)
4. 采用MYSQL记录信息
5. 支持分布式(规划中)
6. 支持HTTP和私有验证(规划中)
7. 支持详尽的日志和配置信息
8. 支持传输速率控制
9. 支持任务开始结束代理转发(规划中)  
10.支持P2P,BT,数据分发(规划中)  
11.支持秒传  
12.支持NGINX 下载透传代理(规划中)  
13.支持NGINX UPLOAD MODULE上传代理  

## 安装教程

#### 版本需求
支持WINDOWS 7SP1和LINUX(UBUNT20.04,CENTOS8)以上系统  
XEngine版本需要V7.14或者以上版本  
vcpkg 需要2021.05.11以上版本  

#### Windows
使用VS2019 x86(debug release) x64(release)打开并且编译
你需要按照下面的方式配置环境,不然你可能需要自己在项目中设置库目录
##### 三方库环境
需要使用VCPKG安装环境.代码地址:https://github.com/microsoft/vcpkg  
安装好后你可能需要把它集成到你的VS中  
然后命令安装环境: vcpkg.exe install jsoncpp   
集成VS环境: vcpkg integrate install  
vcpkg 主要为了方便安装jsoncpp,如果你想自己配置jsoncpp的环境,可以不使用vcpkg...  

##### XEngine环境
XEngine可以直接下载,下载完毕后添加用户环境变量,需要下面两个  
- XEngine_Include 头文件目录地址
- XEngine_Library 库文件目录地址

#### Linux
Linux使用Makefile编译  
UBUNTU20.04 x64或者CENTOS8 x64均可  

##### 三方库环境
基于不同系统,可能命令不同,比如在Ubuntu下面  
sudo apt install libjsoncpp-devel  
Centos8.x  
sudo dnf install jsoncpp-devel  

##### XEngine环境
XEngine可以通过脚本文件安装sudo XEngine_RunEnv.sh -c 3
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
- XEngine_Docment  文档目录结构  
- XEngine_Release  安装目录结构  
- XEngine_Source   源代目录结构  

## API列表
POST方法在本服务中用作于API.  
API用于管理服务器.API列表格式使用URL来识别,比如,POST /api/query/file  
三段格式是固定的,第一个api固定值,第二个query表示API类型,第三个file表示api名  
支持的API列表参考:apilist.txt  

## 秒传实现  
秒传的实现不是靠服务器实现的,而是靠客户端实现的.  
上传秒传的实现是先通过HASH查询文件是否在服务器,如果存在就不上传直接提示客户端上传成功.  
下载秒传的实现是先通过HASH查询本地文件保存路径,如果存在就直接下载完成.  

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request  

## 提交问题

如果你有问题,可以在issues中提交
