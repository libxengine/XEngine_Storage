#pragma once
/********************************************************************
//	Created:	2012/5/21  17:01
//	File Name: 	J:\U_DISK_Path\NetSocketEngine\NetEngine_Client\NetClient_P2p\P2PPeerManage_Define.h
//	File Path:	J:\U_DISK_Path\NetSocketEngine\NetEngine_Client\NetClient_P2p
//	File Base:	P2PPeerManage_Define
//	File Ext:	h
//  Project:    NetSocketEngine(网络通信引擎)
//	Author:		dowflyon
//	Purpose:	P2P节点管理器导出函数
//	History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
///                          回调函数定义
//////////////////////////////////////////////////////////////////////////
//客户端数据回调函数-如果是TCP第一个参数是 一个客户端句柄,UDP客户端发送过来的地址
typedef void(CALLBACK* CALLBACK_XENGINE_P2XP_DISTRIBUTION_IOCLIENT_EVENTS)(LPCSTR lpszAddr, LPCSTR lpszMsgBuffer, int nLen, BOOL bIsTcp, LPVOID lParam);
//服务器数据回调函数,如果第二个参数为NULL,那么第三个参数0 表示客户端离开,1表示客户端进入,UDP没有客户离开和进入事件
typedef void(CALLBACK* CALLBACK_XENGINE_P2XP_DISTRIBUTION_IOSERVICE_EVENTS)(LPCSTR lpszAddr, LPCSTR lpszMsgBuffer, int nLen, BOOL bIsTcp, LPVOID lParam);
//////////////////////////////////////////////////////////////////////////
//                     导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD P2XPClient_GetLastError(int *pInt_Sys = NULL);
/************************************************************************/
/*                     P2P传输管理器导出函数                            */
/************************************************************************/
