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
    BOOL XStorageProtocol_Client_REQQueryFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL);
    BOOL XStorageProtocol_Client_REQDelete(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL);
public:
    BOOL XStorageProtocol_Client_REQDirOperator(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUserDir, int nOperator);
};
