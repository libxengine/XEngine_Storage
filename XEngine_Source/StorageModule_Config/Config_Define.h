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
    TCHAR tszIPAddr[128];
    BOOL bDeamon;
    BOOL bReuseraddr;
    int nCenterPort;
    int nStorageDLPort;
    int nStorageUPPort;
    struct 
    {
        BOOL bReload;
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
        BOOL bHBTime;
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
        TCHAR tszSQLAddr[128];
        TCHAR tszSQLUser[128];
        TCHAR tszSQLPass[128];
        TCHAR tszDBName[128];                                         
        int nSQLPort;
        BOOL bEnable;
        TCHAR tszSQLFile[MAX_PATH];
    }st_XSql;
    struct  
	{
        BOOL bResumable;
        BOOL bUPHash;
        int nHashMode;
    }st_XStorage;
    struct  
    {
        struct  
        {
            BOOL bAuth;
            TCHAR tszAuthProxy[MAX_PATH];
            TCHAR tszUserList[MAX_PATH];
        }st_XProxyAuth;
        struct  
        {
            BOOL bUPPass;
            BOOL bDLPass;
            TCHAR tszUPPass[MAX_PATH];
            TCHAR tszDLPass[MAX_PATH];
        }st_XProxyPass;
    }st_XProxy;
	struct
	{
        int nDLTry;
        __int64x nMaxUPLoader;
        __int64x nMaxDNLoader;
	}st_XLimit;
    struct  
    {
        int nTime;
        int nSDPort;
        int nRVPort;
    }st_P2xp;
    struct  
    {
        TCHAR tszCertChain[MAX_PATH];
        TCHAR tszCertKey[MAX_PATH];
        int nSslType;
        BOOL bDLEnable;
        BOOL bUPEnable;
        BOOL bCHEnable;
    }st_XCert;
    struct
    {
        list<string> *pStl_ListStorage;
    }st_XVer;
}XENGINE_SERVERCONFIG;

typedef struct  
{
    TCHAR tszFilePath[MAX_PATH];
    TCHAR tszFileName[MAX_PATH];
    TCHAR tszBuckKey[128];
    TCHAR tszBuckSize[64];
    int nLevel;
    BOOL bEnable;
}XENGINE_STORAGEBUCKET;
typedef struct
{
	BOOL bDistributed;
    struct  
    {
        int nCenterMode;
        int nUPLoadMode;
        int nDownldMode;
        int nStorageMode;
    }st_LBDistributed;
	struct
	{
		int nCenterMode;
		int nUPLoadMode;
		int nDownldMode;
		int nStorageMode;
	}st_LBLocation;
	struct
	{
        list<string>* pStl_ListCenter;
		list<string>* pStl_ListDownload;
		list<string>* pStl_ListUPLoader;
        list<XENGINE_STORAGEBUCKET>* pStl_ListBucket;
	}st_LoadBalance;
}XENGINE_LBCONFIG;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int *pInt_ErrorCode = NULL);
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
extern "C" BOOL Config_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVERCONFIG * pSt_ServerConfig);
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
extern "C" BOOL Config_Json_LoadBalance(LPCTSTR lpszConfigFile, XENGINE_LBCONFIG * pSt_ServerConfig);