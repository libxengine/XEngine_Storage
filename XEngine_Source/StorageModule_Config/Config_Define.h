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
    int nCenterPort;
    int nStorageDLPort;
    int nStorageUPPort;
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
    }st_XSql;
    struct  
	{
        int nUseMode;
        int nHashMode;
        TCHAR tszHttpAddr[MAX_PATH];
        TCHAR tszNginAddr[MAX_PATH];
		TCHAR tszFileDir[MAX_PATH];
    }st_XStorage;
	struct
	{
        __int64x nMaxUPLoader;
        __int64x nMaxDNLoader;
	}st_XLimit;
    struct
    {
        list<tstring> *pStl_ListStorage;
    }st_XVer;
}XENGINE_SERVERCONFIG;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                        文件配置读取                                  */
/************************************************************************/
extern "C" BOOL Config_Json_File(LPCTSTR lpszConfigFile,XENGINE_SERVERCONFIG *pSt_ServerConfig);
