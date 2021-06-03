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
        TCHAR tszSQLAddr[64];
        TCHAR tszSQLUser[64];
        TCHAR tszSQLPass[64];
        int nSQLPort;
    }st_XSql;
    struct  
	{
        int nUseMode;
        TCHAR tszHttpAddr[MAX_PATH];
        TCHAR tszNginAddr[MAX_PATH];
		TCHAR tszFileDir[MAX_PATH];
    }st_XStorage;
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
