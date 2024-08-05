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
	XCHAR tszFilePath[MAX_PATH];                                           //文件路径
	XCHAR tszFileName[MAX_PATH];                                           //文件名称,文件传输的时候才需要,其他时候不需要
	XCHAR tszFileHash[MAX_PATH];                                           //文件HASH值,MD5或者HASH1 256等...不能超过260字节大小
	XCHAR tszFileUser[128];                                                //文件所属用户
	XCHAR tszFileTime[64];                                                 //文件创建时间，如果这个参数不填,那么服务器将会设置为接受到的文件时间
	__int64x nFileSize;                                                   //文件大小
}XENGINE_PROTOCOL_FILE, * LPXENGINE_PROTOCOL_FILE;
//连接信息
typedef struct tag_XEngine_P2XPPeer_Protocol
{
	XCHAR tszUserName[64];                                                 //用户信息
	XCHAR tszPrivateAddr[32];                                              //私有本地地址，内网地址
	XCHAR tszPublicAddr[32];                                               //外网地址
	XCHAR tszConnectAddr[32];                                              //链接地址
	XSHOT dwConnectType;                                                   //连接类型
	XSHOT dwPeerType;                                                      //节点类型
}XENGINE_P2XPPEER_PROTOCOL, * LPXENGINE_P2XPPEER_PROTOCOL;
//P2XP通道连接命令
typedef struct tag_XEngine_P2XPIO_Protocol
{
	XCHAR tszSourceUser[32];                                               //请求连接的用户
	XCHAR tszDestUser[32];                                                 //要连接的用户
	XCHAR tszConnectAddr[32];                                              //连接的IP地址
	int nDestPort;                                                        //要连接的端口
	bool bIsTcp;                                                          //连接类型TCP,否则为UDP
}XENGINE_P2XPIO_PROTOCOL, * LPXENGINE_P2XPIO_PROTOCOL;
//IP地址信息
typedef struct
{
	XCHAR tszIPStart[128];
	XCHAR tszIPEnd[128];
	XCHAR tszIPAddr[128];       //IP地址
	XCHAR tszIPCountry[128];    //国家/地区
	XCHAR tszIPProvince[128];   //省/自治区
	XCHAR tszIPCity[128];       //市
	XCHAR tszIPCounty[128];     //县
	XCHAR tszIPAddress[128];    //详细地址
	XCHAR tszIPISP[128];        //运营商
	XCHAR tszIPTime[128];       //数据库日期
}XENGINE_IPADDRINFO;
//服务器动作处理
typedef struct
{
	XCHAR tszFileUrl[MAX_PATH];      //文件的URL
	XCHAR tszFileName[MAX_PATH];     //本地文件路径
	XCHAR tszBucketStr[MAX_PATH];    //存储的KEY
	XBYTE byType;                    //操作类型,0 HTTP,1 FTP,2 BT
}XENGINE_ACTIONINFO;
//WEBDAV协议
typedef struct
{
	XCHAR tszToken[MAX_PATH];                 //令牌
	XCHAR tszOwner[128];                 //所属用户
	XCHAR tszTimeout[64];                //超时时间
	XBYTE byLockType;                    //0未知,1独占,2共享
	XBYTE byLockOP;                      //0未知,1写,2读
}XENGINE_WEBDAVLOCK;
#pragma pack(pop)