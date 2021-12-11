#pragma once
/********************************************************************
//    Created:     2021/07/19  13:41:43
//    File Name:   D:\XEngine_Storage\XEngine_Source\XStorage_Protocol.h
//    File Path:   D:\XEngine_Storage\XEngine_Source
//    File Base:   XStorage_Protocol
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储服务-标准协议头文件
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                         协议定义
//////////////////////////////////////////////////////////////////////////
//存储服务
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQQUERY 0x4001     //查询请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPQUERY 0x4002     //查询回复
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQREPORT 0x4003    //报告查询结果
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPREPORT 0x4004    //确认查询结果
//P2XP协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLOGIN 0x6001        //登录
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLOGIN 0x6002
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLANLIST 0x6003      //同步列表协议,同步本地局域网IP中的客户端
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLANLIST 0x6004      //同步列表回复确认协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQCONNECT 0x6005      //请求用户连接到网络服务
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPCONNECT 0x6006      //确认连接协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQUSERQUERY 0x6007    //用户查询
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPUSERQUERY 0x6008    //用户查询确认
//////////////////////////////////////////////////////////////////////////
//                         协议结构体
//////////////////////////////////////////////////////////////////////////
//文件传输头文件,可用于P2XP文件传输和组包管理模块文件传输
typedef struct tag_XEngine_Protocol_File
{
	CHAR tszFilePath[MAX_PATH];                                           //文件路径
	CHAR tszFileName[MAX_PATH];                                           //文件名称,文件传输的时候才需要,其他时候不需要
	CHAR tszFileHash[MAX_PATH];                                           //文件HASH值,MD5或者HASH1 256等...不能超过260字节大小
	CHAR tszFileUser[128];                                                //文件所属用户
	CHAR tszFileTime[64];                                                 //文件创建时间，如果这个参数不填,那么服务器将会设置为接受到的文件时间
	__int64x nFileSize;                                                   //文件大小
}XENGINE_PROTOCOL_FILE, * LPXENGINE_PROTOCOL_FILE;
//连接信息
typedef struct tag_XEngine_P2XPPeer_Protocol
{
	CHAR tszUserName[64];                                                 //用户信息
	CHAR tszPrivateAddr[32];                                              //私有本地地址，内网地址
	CHAR tszPublicAddr[32];                                               //外网地址
	CHAR tszConnectAddr[32];                                              //链接地址
	CHAR tszUserLocation[32];                                             //位置信息
	CHAR tszUserISP[20];                                                  //用户ISP
	WORD dwConnectType;                                                   //连接类型
	WORD dwPeerType;                                                      //节点类型
}XENGINE_P2XPPEER_PROTOCOL, * LPXENGINE_P2XPPEER_PROTOCOL;
//P2XP通道连接命令
typedef struct tag_XEngine_P2XPIO_Protocol
{
	CHAR tszSourceUser[32];                                               //请求连接的用户
	CHAR tszDestUser[32];                                                 //要连接的用户
	CHAR tszConnectAddr[32];                                              //连接的IP地址
	int nDestPort;                                                        //要连接的端口
	BOOL bIsTcp;                                                          //连接类型TCP,否则为UDP
}XENGINE_P2XPIO_PROTOCOL, * LPXENGINE_P2XPIO_PROTOCOL;