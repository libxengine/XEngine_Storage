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
    int nP2XPPort;
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
        int nP2XPThread;
    }st_XMax;
    struct
    {
        BOOL bHBTime;
        int nDBMonth;
        int nTimeCheck;
        int nCenterTimeOut;
        int nStorageTimeOut;
        int nP2XPTimeOut;
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
        int nSQLType;
        TCHAR tszSQLFile[MAX_PATH];
    }st_XSql;
    struct  
	{
        BOOL bRename;
        int nSendMode;
        int nHashMode;
		TCHAR tszFileDir[MAX_PATH];
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
        int nMode;
        int nTime;
        int nSDPort;
        int nRVPort;
        TCHAR tszQQWryFile[MAX_PATH];
    }st_P2xp;
    struct
    {
        list<tstring> *pStl_ListStorage;
    }st_XVer;
}XENGINE_SERVERCONFIG;
typedef struct
{
	TCHAR tszIPAddr[128];
	BOOL bDistributed;
    struct  
    {
        int nServerMode;
    }st_LBConfig;
	struct
	{
        list<int>* pStl_ListUseMode;
        list<tstring>* pStl_ListCenter;
		list<tstring>* pStl_ListDownload;
		list<tstring>* pStl_ListUPLoader;
	}st_LoadBalance;
}XENGINE_LBCONFIG;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                        文件配置读取                                  */
/************************************************************************/
extern "C" BOOL Config_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVERCONFIG * pSt_ServerConfig);
extern "C" BOOL Config_Json_LoadBalance(LPCTSTR lpszConfigFile, XENGINE_LBCONFIG * pSt_ServerConfig);