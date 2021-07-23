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
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_TRANSMISSION 0x4000 //文件传输
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQUPDATE 0x4001    //上传文件请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPUPDATE 0x4002    //上传文件请求确认
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQDOWN 0x4003      //下载文件请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDOWN 0x4004      //下载文件确认
//P2XP协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLANLIST 0x6001      //同步列表协议,同步本地局域网IP中的客户端
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLANLIST 0x6002      //同步列表回复确认协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQWLANLIST 0x6003     //同步列表协议,同步同一外网IP中局域网
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPWLANLIST 0x6004     //同步万维网IP确认协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQCONNECT 0x6005      //请求用户连接到网络服务
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPCONNECT 0x6006      //确认连接协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQUSERQUERY 0x6007    //用户查询
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPUSERQUERY 0x6008    //用户查询确认
//////////////////////////////////////////////////////////////////////////
//                         协议结构体
//////////////////////////////////////////////////////////////////////////
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