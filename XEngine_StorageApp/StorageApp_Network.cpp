﻿#include "StorageApp_Hdr.h"

BOOL CALLBACK XEngine_Callback_DownloadLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	RfcComponents_HttpServer_CreateClientEx(xhDLHttp, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHBDownload, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_DownloadRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!RfcComponents_HttpServer_InserQueueEx(xhDLHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBDownload, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_DownloadLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, FALSE);
}
BOOL CALLBACK XEngine_Callback_UPLoaderLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	RfcComponents_HttpServer_CreateClientEx(xhUPHttp, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHBUPLoader, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_UPLoaderRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!RfcComponents_HttpServer_InserQueueEx(xhUPHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBUPLoader, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_UPLoaderLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, FALSE);
}
//////////////////////////////////////////////////////////////////////////
void CALLBACK XEngine_Callback_HBDownload(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, TRUE);
}
void CALLBACK XEngine_Callback_HBUPLoader(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, TRUE);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Net_CloseClient(LPCTSTR lpszClientAddr, BOOL bHBLeave)
{
	LPCTSTR lpszLeaveMsg = bHBLeave ? _T("心跳超时") : _T("主动断开");

	if (bHBLeave)
	{
		NetCore_TCPXCore_CloseForClientEx(xhNetDownload, lpszClientAddr);
		NetCore_TCPXCore_CloseForClientEx(xhNetUPLoader, lpszClientAddr);
	}
	else
	{
		SocketOpt_HeartBeat_DeleteAddrEx(xhHBDownload, lpszClientAddr);
		SocketOpt_HeartBeat_DeleteAddrEx(xhHBUPLoader, lpszClientAddr);
	}
	Session_DLStroage_Delete(lpszClientAddr);
	RfcComponents_HttpServer_CloseClinetEx(xhUPHttp, lpszClientAddr);
	RfcComponents_HttpServer_CloseClinetEx(xhDLHttp, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%s，与服务器断开，原因：%s"), lpszClientAddr, lpszLeaveMsg);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
BOOL XEngine_Net_SendMsg(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nType)
{
	BOOL bRet = FALSE;

	if (STORAGE_NETTYPE_HTTPDOWNLOAD == nType)
	{
		bRet = NetCore_TCPXCore_SendEx(xhNetDownload, lpszClientAddr, lpszMsgBuffer, nMsgLen);
		if (bRet)
		{
			SocketOpt_HeartBeat_ActiveAddrEx(xhHBDownload, lpszClientAddr);
		}
	}
	else
	{
		bRet = NetCore_TCPXCore_SendEx(xhNetUPLoader, lpszClientAddr, lpszMsgBuffer, nMsgLen);
		if (bRet)
		{
			SocketOpt_HeartBeat_ActiveAddrEx(xhHBUPLoader, lpszClientAddr);
		}
	}

	if (!bRet)
	{
		DWORD dwRet = NetCore_GetLastError();
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端：%s，网络类型:%d,发送数据失败，发送大小：%d，错误：%lX,%d"), lpszClientAddr, nType, nMsgLen, dwRet, errno);
		return FALSE;
	}
	return TRUE;
}
