#pragma once
/********************************************************************
//	Created:	2019/6/28   15:02
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Core\XStorageProtocol_Core.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Core
//	File Base:	XStorageProtocol_Core
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	存储服务协议解析类
//	History:
*********************************************************************/

class CXStorageProtocol_Core
{
public:
    CXStorageProtocol_Core();
    ~CXStorageProtocol_Core();
public:
    BOOL XStorageProtocol_Core_REQQueryFile(LPCTSTR lpszMsgBuffer, TCHAR *ptszTimeStart, TCHAR *ptszTimeEnd, TCHAR *ptszFileName = NULL, TCHAR * ptszFileHash = NULL);
    BOOL XStorageProtocol_Core_REPQueryFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int nListCount, LPCTSTR lpszRootDir, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
    BOOL XStorageProtocol_Core_ReportFileParse(LPCTSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int* pInt_ListCount);
    BOOL XStorageProtocol_Core_REPInfo(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, SESSION_STORAGEINFO*** pppSt_DLInfo, SESSION_STORAGEINFO*** pppSt_UPInfo, int nDLCount, int nUPCount);
public:
	BOOL XStorageProtocol_Core_REQDirOperator(LPCTSTR lpszMsgBuffer, TCHAR* ptszUserDir, int* pInt_Operator);
	BOOL XStorageProtocol_Core_REPDirOperator(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, TCHAR*** pppszListEnum, int nListCount);
public:
	BOOL XStorageProtocol_Core_REQUPEvent(LPCTSTR lpszMsgBuffer, LPCTSTR lpszBoundary, TCHAR* ptszFileName, TCHAR* ptszFilePath, TCHAR* ptszFileHash, __int64x* pInt_FileSize);
};
