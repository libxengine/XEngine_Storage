[中文](README.md) ||  [English](README.en.md)  
# XEngine_Storage
This repository has a development and master branch. If you want to use it, please use the master branch  
As long as the repository is not in a suspended state, someone will maintain and develop it all the time, please use it with confidence  

## Introduction
c c++ 存储服务 c c++ 文件存储服务  
c c++ file storage service  
This is an api gateway for uploading and downloading.  
The service is for users who have business needs for upload and download services (such as developers who need to upload and download servers).  
This server is more inclined to storestorage transfer and API Manage interface  
best storage service for http,batter than nginx and more convenient to manage,more concise code. On going maintenance  

## Software feature
The purpose of development and implementation based on libXEngine is a cross-platform network storage service  

feature list:  
1. support file http upload and download(use put and get method)  
2. support http api notify of event and management api interface  
3. support http encrypt and cert data transimission  
4. support database(mysql and sqlite) record infomation.  
5. support load banace  
6. supprot http auth  
7. support log and config  
8. supprot speeds contral  
9. supprot task start and end proxy forwarding  
10. support p2p  
11. bt(planning)  
12. data distributed  
13. support second pass and Resumable   
14. support upload and download Redirect  
15. support full speed and limit speed

## install

#### XEngine Evn
you must install XEngine,need V7.38 or above,install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

##### fast to deployment 
git clone https://gitee.com/xyry/libxengine.git or git clone https://github.com/libxengine/xengine.git  
window Exection XEngine_WINEnv.bat   
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3

#### Windows
use vs open and compile,suport windows 7sp1 and above  
Just Run it,use XEngine_AuthorizeApp

#### Linux
use makefile compile,UBUNTU20.04 x64 or CENTOS8 x64  
Run it on the terminal

#### Macos
use makefile compile,mac 12 and above  
Run it on the terminal

##### complie
execute command in XEngine_Source path  
make complie  
make FLAGS=InstallAll install  
make FLAGS=CleanAll clear  

#### use

1.  Switch to the MASTER branch
2.  download code
3.  complie
4.  install
5.  run
6.  use curl or postman test upload and download  

## second pass
The Second pass is not realized by the server, it is by the client  
upload file second pass is first check the HASH file is on the server, if has file on the server, it will directly prompt the upload is successful.  
The realization of downloading second transmission is to first query the local file save path through HASH, and download it directly if it exists.

## about P2P
P2P distributed download has been supported, but currently only in the lan,cross-network segment is not supported for the time being, you need to wait for the development to be completed  
P2P distributed download is the same as the hyper-threaded download of other download tools. The principle is to use the HTTP RANGE field. You can implement this function through libraries such as libcurl.  

## directory struct
- XEngine_Docment   docment directory
- XEngine_Release   install directory
- XEngine_Source    code    directory  
- XEngine_APPClient client  code dir  
- XEngine_SQLFile   database sql file

## now task
P2P WAN file search and download support  

## other problems   
You can refer to the document under the docment directory. It contains API protocol and service description.  
When the jsoncpp module is used, the compilation runtime library selection must be consistent  

## Test Service
Address:app.xyry.org or 159.75.200.173  
Port:HTTP Center Port 5100,Download 5101,UPLoad 5102  

## Participate in contribution

1. Fork this code
2. Create new Feat_xxx branch
3. Submit the code
4. New Pull Request

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...
