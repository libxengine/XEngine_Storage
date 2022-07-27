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
//////////////////////////////////////////////////////////////////////////
//                         协议结构体
//////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
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
//IP地址信息
typedef struct
{
	TCHAR tszIPStart[128];
	TCHAR tszIPEnd[128];
	TCHAR tszIPAddr[128];       //IP地址
	TCHAR tszIPCountry[128];    //国家/地区
	TCHAR tszIPProvince[128];   //省/自治区
	TCHAR tszIPCity[128];       //市
	TCHAR tszIPCounty[128];     //县
	TCHAR tszIPAddress[128];    //详细地址
	TCHAR tszIPISP[128];        //运营商
	TCHAR tszIPTime[128];       //数据库日期
}XENGINE_IPADDRINFO;
#pragma pack(pop)