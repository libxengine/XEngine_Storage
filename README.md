# XEngine_Storage

## 介绍
c c++ 存储服务 c c++ 文件存储服务
c c++ file storage service

## 软件架构
基于libXEngine开发并实现的一套简洁高性能跨平台网络存储服务  
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
软件特性:  
1. 支持HTTP协议上传和下载(采用GET和POST)
2. 支持二进制私有协议传送数据(规划中)
3. 支持加密传输(规划中)
4. 采用MYSQL记录信息
5. 支持分布式(规划中)
6. 支持HTTP和私有验证(规划中)
7. 支持详尽的日志和配置信息

## 安装教程

#### Windows
使用VS2019 打开并且编译.支持WIN7-WIN10  
你需要按照下面的方式配置环境,不然你可能需要自己在项目中设置库目录
##### 三方库环境
需要使用VCPKG安装环境.代码地址:https://github.com/microsoft/vcpkg  
安装好后你可能需要把它集成到你的VS中  
然后命令安装环境: vcpkg.exe install jsoncpp   
集成VS环境: vcpkg integrate install  
##### XEngine环境
XEngine可以直接下载,下载完毕后添加用户环境变量,需要下面两个  
- XEngine_Include 头文件目录地址
- XEngine_Library 库文件目录地址

#### Linux
Linux使用Makefile编译  
Linux支持UBUNTU20.04 以上版本
##### 三方库环境
基于不同系统,可能命令不同,比如在Ubuntu下面  
sudo apt install libjsoncpp-devel
##### XEngine环境
XEngine可以通过脚本文件安装sudo XEngine_RunEnv.sh -c 3

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

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
