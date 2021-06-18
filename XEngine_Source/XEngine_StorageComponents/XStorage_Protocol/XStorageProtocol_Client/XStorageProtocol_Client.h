#pragma once
/********************************************************************
//	Created:	2019/6/28   16:14
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Client\XStorageProtocol_Client.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Client
//	File Base:	XStorageProtocol_Client
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	存储客户端协议类
//	History:
*********************************************************************/

class CXStorageProtocol_Client
{
public:
    CXStorageProtocol_Client();
    ~CXStorageProtocol_Client();
public:
    BOOL XStorageProtocol_Client_REQQueryFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileMD5 = NULL);
    BOOL XStorageProtocol_Client_REQQueryUser(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUserName, BOOL bQueryKey = FALSE);
    BOOL XStorageProtocol_Client_REQDelete(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileMD5 = NULL);
    BOOL XStorageProtocol_Client_REQLogin(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XENGINE_PROTOCOL_USERAUTH *pSt_ProtocolAuth);
    BOOL XStorageProtocol_Client_REQFile(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, LPCTSTR lpszFileMD5 = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFilePath = NULL, BOOL bUPFile = TRUE);
public:
    BOOL XStorageProtocol_Client_REQDirOperator(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszUserDir, BOOL bCreate = TRUE);
    BOOL XStorageProtocol_Client_REQDirQuery(TCHAR *ptszMsgBuffer, int *pInt_MsgLen);
public:
    BOOL XStorageProtocol_Client_REQRegister(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUser, LPCTSTR lpszPass, LPCTSTR lpszEMailAddr, __int64x nPhoneNumber, __int64x nIDNumber, int nPerimission = 0);
};
