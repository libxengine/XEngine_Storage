[中文](README.md) ||  [English](README.en.md)  
# XEngine_Storage
This repository has a development and master branch. If you want to use it, please use the master branch  

## Introduction
c c++ 存储服务 c c++ 文件存储服务  
c c++ file storage service  
This is an api gateway for uploading and downloading.  
The service is for users who have business needs for upload and download services (such as developers who need to upload and download servers).  
This server is more inclined to store the API interface direction  
best storage service for http,batter than nginx and more convenient to manage. On going maintenance  

## Software feature
The purpose of development and implementation based on libXEngine is a cross-platform network storage service  

feature list:  
1. support file http upload and download(use put and get method)  
2. support http api notify of event and management api interface  
3. support http encrypt and cert data transimission(planning)  
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
14. support nginx upload module proxy_pass  
15. support upload and download Redirect  
16. dynamic rate of the download

## install

#### Requirements  
support system above windows 7sp1 and linux(ubuntu20.04,centos8)  
XEngine need V7.18 or above  
vcpkg need 2021.05.11 or above  

#### Windows
use vs2019 x86(debug or release) x64(only release) open and complie  
You need to configure the environment in the following way, otherwise you may need to set the library directory in the project yourself  

##### install Dependent library
vcpkg:https://github.com/microsoft/vcpkg   
vcpkg.exe install jsoncpp  
vcpkg integrate install  
you can not use vcpkg and config jsoncpp development for youself   

##### XEngine
download xengine through git.read xengine project readme file to configure your env  

#### Linux
Linux use Makefile to complie  
supproted ubuntu x64 or centos x64     

##### install Dependent library
install jsoncpp to your system  
ubuntu20.04  
sudo apt install libjsoncpp-devel  
Centos8.x  
sudo dnf install jsoncpp-devel  

##### XEngine Install
you can install xengine env to your system by shell   
like this:sudo XEngine_LINEnv.sh -i 3  
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
Download speed limit optimization  

## other problems   
You can refer to the document under the docment directory. It contains API protocol and service description.  

## Participate in contribution

1. Fork this code
2. Create new Feat_xxx branch
3. Submit the code
4. New Pull Request

## Clean up statement
the software will not generate any files to other directories in your system.  
the software can achieve complete cleanup by directly delete the directory.  

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...
