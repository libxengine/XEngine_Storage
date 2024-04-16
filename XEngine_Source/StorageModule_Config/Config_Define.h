#pragma once
/********************************************************************
//    Created:     2021/06/02  15:00:09
//    File Name:   D:\XEngine_Storage\StorageModule_Config\Config_Define.h
//    File Path:   D:\XEngine_Storage\StorageModule_Config
//    File Base:   Config_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出的定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct tag_XEngine_ServerConfig
{
    XCHAR tszIPAddr[128];
    bool bDeamon;
    bool bReuseraddr;
    int nCenterPort;
    int nStorageDLPort;
    int nStorageUPPort;
    struct 
    {
        bool bReload;
    }st_Memory;
    struct
    {
        int nMaxClient;
        int nMaxQueue;
        int nIOThread;
        int nCenterThread;
        int nStorageDLThread;
        int nStorageUPThread;
    }st_XMax;
    struct
    {
        bool bHBTime;
        int nDBMonth;
        int nTimeCheck;
        int nCenterTimeOut;
        int nStorageTimeOut;
    }st_XTime;
    struct
    {
        int nMaxSize;
        int nMaxCount;
        int nLogLeave;
    }st_XLog;
    struct
    {
        XCHAR tszSQLAddr[128];
        XCHAR tszSQLUser[128];
        XCHAR tszSQLPass[128];
        XCHAR tszDBName[128];                                         
        int nSQLPort;
        bool bEnable;
    }st_XSql;
    struct  
	{
        bool bResumable;
        bool bUPHash;
        int nHashMode;
    }st_XStorage;
	struct
	{
		bool bUPAuth;
        bool bDLAuth;
        bool bCHAuth;
		XCHAR tszUserList[MAX_PATH];
	}st_XAuth;
    struct  
    {
		bool bUPPass;
		bool bDLPass;
        bool bAuthPass;
		XCHAR tszUPPass[MAX_PATH];
		XCHAR tszDLPass[MAX_PATH];
        XCHAR tszAuthPass[MAX_PATH];
    }st_XProxy;
	struct
	{
		bool bLimitMode;
        __int64x nMaxUPLoader;
        __int64x nMaxDNLoader;
        int nMaxUPConnect;
        int nMaxDNConnect;
	}st_XLimit;
    struct  
    {
        bool bEnable;
        int nTime;
        int nSDPort;
        int nRVPort;
    }st_P2xp;
    struct  
    {
        XCHAR tszCertChain[MAX_PATH];
        XCHAR tszCertServer[MAX_PATH];
        XCHAR tszCertKey[MAX_PATH];
        bool bDLEnable;
        bool bUPEnable;
        bool bCHEnable;
    }st_XCert;
    struct  
    {
        XCHAR tszAPIUrl[MAX_PATH];
        bool bEnable;
    }st_XReport;
    struct
    {
        list<string> *pStl_ListStorage;
    }st_XVer;
}XENGINE_SERVERCONFIG;

typedef struct  
{
    XCHAR tszFilePath[MAX_PATH];
    XCHAR tszFileName[MAX_PATH];
    XCHAR tszBuckKey[128];
    XCHAR tszBuckSize[64];
    int nLevel;
    bool bEnable;
    struct 
    {
        bool bCreateDir;
        bool bRewrite;
        bool bUPLimit;
    }st_PermissionFlags;
}XENGINE_STORAGEBUCKET;
typedef struct
{
    struct  
    {
        int nUPLoadMode;
        int nDownldMode;
        int nStorageMode;
    }st_LBDistributed;
	struct
	{
		int nUPLoadMode;
		int nDownldMode;
		int nStorageMode;
	}st_LBLocation;
	struct
	{
		list<string>* pStl_ListDownload;
		list<string>* pStl_ListUPLoader;
        list<XENGINE_STORAGEBUCKET>* pStl_ListBucket;
	}st_LoadBalance;
}XENGINE_LBCONFIG;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Config_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                        文件配置读取                                  */
/************************************************************************/
/********************************************************************
函数名称：Config_Json_File
函数功能：读取基本配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Config_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVERCONFIG * pSt_ServerConfig);
/********************************************************************
函数名称：Config_Json_LoadBalance
函数功能：读取负载均衡配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Config_Json_LoadBalance(LPCXSTR lpszConfigFile, XENGINE_LBCONFIG * pSt_ServerConfig);