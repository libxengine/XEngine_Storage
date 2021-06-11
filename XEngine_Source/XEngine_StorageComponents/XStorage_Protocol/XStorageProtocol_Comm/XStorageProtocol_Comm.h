#pragma once
/********************************************************************
//	Created:	2019/6/28   15:19
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Comm\XStorageProtocol_Comm.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Comm
//	File Base:	XStorageProtocol_Comm
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	存储协议公用类
//	History:
*********************************************************************/

class CXStorageProtocol_Comm
{
public:
    CXStorageProtocol_Comm();
    ~CXStorageProtocol_Comm();
public:
    BOOL XStorageProtocol_Comm_ParseFile(LPCTSTR lpszMsgBuffer, int nMsgLen, BYTE byVersion, XENGINE_PROTOCOLFILE *pSt_ProtocolFile);
    BOOL XStorageProtocol_Comm_Response(XENGINE_PROTOCOLHDR *pSt_ProtocolHdr, TCHAR *ptszMsgBuffer, int *pInt_MsgLen, int nMsgCode = 0, LPCTSTR lpszMsgInfo = NULL);
    BOOL XStorageProtocol_Comm_ParseClient(LPCTSTR lpszMsgBuffer, TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XENGINE_PROTOCOLHDR *pSt_ProtocolHdr = NULL);
protected:
private:
};
