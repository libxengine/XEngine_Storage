[中文](README.md) ||  [English](README.en.md)  
# XEngine_Storage
This repository has a development and master branch. If you want to use it, please use the master branch  
As long as the repository is not in a suspended state, someone will maintain and develop it all the time, please use it with confidence  

## Introduction
c c++ 存储服务 c c++ 文件存储服务  
c c++ file storage service  
This is an api gateway for uploading and downloading.  
The service is for users who have business needs for upload and download services (such as developers who need to upload and download servers).  
This server is more inclined to storestorage transfer and API Manage interface,the service can be used upload and download management and network file shared and software update service.  
best storage service for http,more concise code. On going maintenance  

## Software feature
The purpose of development and implementation based on libXEngine is a cross-platform network storage service  

feature list:  
1. support file http upload and download(use post and get method)  
2. support http api notify of event and management api interface  
3. support http encrypt and cert data transimission  
4. support database(mysql) record infomation.  
5. support load banace  
6. supprot http auth  
7. support log and config  
8. Support multi-threaded file upload and download
9. supprot task start and end proxy forwarding  
10. support p2p  
11. bt protocol  
12. data distributed  
13. support second pass and Resumable   
14. support upload and download Redirect  
15. support full speed and limit speed
16. permission control
17. upload file rewrite
18. support convert record(service upload and download)
19. webdav protocol download and upload file support
20. preview file create

## install

#### XEngine Evn
you must install XEngine,need V9.x or above,install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/libxengine/libxengine  
GITHUB:https://github.com/libxengine/libxengine

##### fast to deployment 
git clone https://gitee.com/libxengine/libxengine.git or git clone https://github.com/libxengine/libxengine.git  
window Exection XEngine_WINEnv.bat   
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3

#### sub module
Due to the dependent sub-modules, after you checkout the warehouse, execute the following command in the warehouse directory to pull the sub-modules  
git submodule init  
git submodule update  

#### Windows
use vs open and compile,suport windows 7sp1 and above  
Just Run it,use XEngine_AuthorizeApp  
if you need bt protocol support,you must install bt library by vcpkg:./vcpkg.exe install libtorrent

#### Linux
use makefile compile,UBUNTU22.04 or above  
Run it on the terminal  
if you need bt protocol support,you must install bt library:sudo apt install libtorrent-rasterbar-dev -y

#### Macos
use makefile compile,mac 13 and above  
Run it on the terminal  
if you need bt protocol support,you must install bt library:brew install libtorrent-rasterbar

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
- XEngine_APPClient client  code dir(APPClient_UPDate is software update example of client)  
- XEngine_SQLFile   database sql file

## Example
UPLoad File:   curl -d 'hello xengine' -X POST "http://192.168.1.8:5102/api?filename=hello.txt&storeagekey=storagekey1"  
Download File: curl -X GET "http://192.168.1.8:5101/storagekey1/hello.txt"  

## Installation Instructions
#### Install
Modify the configuration file to what you need, and then run the service directly  
Advanced functions require the support of MYSQL. After installing MYSQL, import XEngine_SQLFile/XStorage_Create.sql and then import XEngine_Storage.sql.
#### upgrade
If it is an upgrade installation, after downloading the new version and modifying the configuration file, it can be run directly  
If you use a database, you need to open the database matching fields and tables are consistent

## other problems   
You can refer to the document under the docment directory. It contains API protocol and service description.  
When the jsoncpp module is used, the compilation runtime library selection must be consistent  

## Test Service
Address:app.xyry.org  
Port:HTTP Center Port 5100,Download 5101,UPLoad 5102  

## Participate in contribution

1. Fork this code
2. Create new Feat_xxx branch
3. Submit the code
4. New Pull Request

## development plan
You can learn more about development content and plans at this address:https://github.com/libxengine/XEngine_Storage/issues

## Follow us
If you think this software is helpful to you, please give us a START  
and wechat qr scan to follow us 
![qrcode](https://www.xyry.org/qrcode.jpg)

## post issues

if you have eny quest.post issues...
