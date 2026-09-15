#pragma once
/********************************************************************
//    Created:     2024/08/05  16:14:56
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_FTPContral\Session_FTPContral.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_FTPContral
//    File Base:   Session_FTPContral
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     FTP会话
//    History:
*********************************************************************/
typedef struct  
{
	XCHAR tszFileName[XPATH_MAX];
	XCHAR tszFilePath[XPATH_MAX];
    XCHAR tszAlisPath[XPATH_MAX];
    __int64u nPos;
    time_t nTimeStart;
    int nPort;
	bool bUPLoader;
	XHANDLE xhToken;
}SESSION_FTPCONTRAL;

class CSession_FTPContral
{
public:
    CSession_FTPContral();
    ~CSession_FTPContral();
public:
    bool Session_FTP_Insert(LPCXSTR lpszClientAddr, bool bUPLoader);
    bool Session_FTP_Set(LPCXSTR lpszClientAddr, LPCXSTR lpszFileName = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszAlisName = NULL);
    bool Session_FTP_Get(LPCXSTR lpszClientAddr, XCHAR* ptszFileName = NULL, XCHAR* ptszFilePath = NULL, XCHAR* ptszAlisPath = NULL);
	bool Session_FTP_SetSocket(LPCXSTR lpszClientAddr, int nPort, XHANDLE xhToken);
	XHANDLE Session_FTP_GetSocket(LPCXSTR lpszClientAddr, int* pInt_Port = NULL);
    bool Session_FTP_SetRetr(LPCXSTR lpszClientAddr, __int64u nPos);
    bool Session_FTP_GetRetr(LPCXSTR lpszClientAddr, __int64u* pInt_Pos);
    bool Session_FTP_Delete(LPCXSTR lpszClientAddr);
private:
    shared_mutex st_Locker;
private:
    unordered_map<xstring, SESSION_FTPCONTRAL> stl_MapAction;
};