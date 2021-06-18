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
    BOOL XStorageProtocol_Core_REPQueryFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int nListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
    BOOL XStorageProtocol_Core_REPFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XNETHANDLE xhToken = 0, BOOL bUPFile = TRUE, int nCode = 0, LPCTSTR lpszCodeMsg = NULL, XENGINE_PROTOCOLFILE *pSt_ProtcolFile = NULL);
public:
	BOOL XStorageProtocol_Core_REQUPEvent(LPCTSTR lpszMsgBuffer, LPCTSTR lpszBoundary, TCHAR* ptszFileName, TCHAR* ptszFilePath, TCHAR* ptszFileHash, __int64x* pInt_FileSize);
public:
	BOOL XStorageProtocol_Core_REQCreateDir(LPCTSTR lpszMsgBuffer, TCHAR* ptszUserDir);
	BOOL XStorageProtocol_Core_REPQueryDir(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, TCHAR*** pppszListEnum, int nListCount);
public:
    BOOL XStorageProtocol_Core_REQUserReg(LPCTSTR lpszMsgBuffer, XSTORAGECORE_USERINFO* pSt_ProtocolRegister);
public:
    BOOL XStorageProtocol_Core_REQQueryUser(LPCTSTR lpszMsgBuffer, TCHAR* ptszUserName, BOOL *pbKeyQuery);
    BOOL XStorageProtocol_Core_REPQueryUser(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPVOID lParam, BOOL bKeyInfo = FALSE);
};