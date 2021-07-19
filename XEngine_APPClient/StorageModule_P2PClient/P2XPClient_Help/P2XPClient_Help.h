#pragma once
/********************************************************************
//	Created:	2014/6/27  14:55
//	File Name: 	F:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_P2xp\NetEngine_P2XPClient\P2XPClient_Help\P2XPClient_Help.h
//	File Path:	F:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_P2xp\NetEngine_P2XPClient\P2XPClient_Help
//	File Base:	P2XPClient_Help
//	File Ext:	h
//  Project:    XyRySoft_NetEngine(网络通信引擎)
//	Author:		qyt 486179@qq.com
//	Purpose:	P2XP客户端帮助类
//	History:
*********************************************************************/
class CP2XPClient_Help
{
public:
    CP2XPClient_Help();
    ~CP2XPClient_Help();
public:
    BOOL P2XPClinet_Help_GetInfo(XENGINE_P2XPPEER_PROTOCOL *pSt_ClientInfo);
    BOOL P2XPClinet_Help_GetType(WORD *pdwPeerType);
    BOOL P2XPClient_Help_UDPHold(LPCTSTR lpszAddr, int nPort);
protected:
    BOOL P2XPClinet_Help_GetIPAddr(LPCTSTR lpszNetAddr,TCHAR *ptszNetAddr);
};
