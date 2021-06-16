#include "StorageApp_Hdr.h"

BOOL CALLBACK XEngine_Callback_DownloadLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	RfcComponents_HttpServer_CreateClientEx(xhDLHttp, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHBDownload, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_DownloadRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!RfcComponents_HttpServer_InserQueueEx(xhDLHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBDownload, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("下载客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_DownloadLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, FALSE, STORAGE_NETTYPE_HTTPDOWNLOAD);
}
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK XEngine_Callback_UPLoaderLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	RfcComponents_HttpServer_CreateClientEx(xhUPHttp, lpszClientAddr, 0);
	RfcComponents_HttpServer_SetRecvModeEx(xhUPHttp, lpszClientAddr, 1);   //设置为文件接受模式
	SocketOpt_HeartBeat_InsertAddrEx(xhHBUPLoader, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_UPLoaderRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!RfcComponents_HttpServer_InserQueueEx(xhUPHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBUPLoader, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("上传客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_UPLoaderLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, FALSE, STORAGE_NETTYPE_HTTPUPLOADER);
}
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK XEngine_Callback_CenterLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	RfcComponents_HttpServer_CreateClientEx(xhUPHttp, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_CenterRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!RfcComponents_HttpServer_InserQueueEx(xhCenterHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
		return;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("业务客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_CenterLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, FALSE, STORAGE_NETTYPE_HTTPCENTER);
}
//////////////////////////////////////////////////////////////////////////
void CALLBACK XEngine_Callback_HBDownload(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, TRUE, STORAGE_NETTYPE_HTTPDOWNLOAD);
}
void CALLBACK XEngine_Callback_HBUPLoader(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, TRUE, STORAGE_NETTYPE_HTTPUPLOADER);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Net_CloseClient(LPCTSTR lpszClientAddr, BOOL bHBLeave, int nType)
{
	LPCTSTR lpszLeaveMsg = bHBLeave ? _T("心跳超时") : _T("主动断开");
	tstring m_StrClient;

	if (STORAGE_NETTYPE_HTTPUPLOADER == nType)
	{
		m_StrClient = _T("上传客户端");
	}
	else if (STORAGE_NETTYPE_HTTPDOWNLOAD == nType)
	{
		m_StrClient = _T("下载客户端");
	}
	else
	{
		m_StrClient = _T("业务客户端");
	}
	
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
	Session_UPStroage_Delete(lpszClientAddr);
	Session_DLStroage_Delete(lpszClientAddr);
	RfcComponents_HttpServer_CloseClinetEx(xhUPHttp, lpszClientAddr);
	RfcComponents_HttpServer_CloseClinetEx(xhDLHttp, lpszClientAddr);
	RfcComponents_HttpServer_CloseClinetEx(xhCenterHttp, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("%s：%s，与服务器断开，原因：%s"), m_StrClient.c_str(), lpszClientAddr, lpszLeaveMsg);
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
