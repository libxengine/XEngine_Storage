#include "StorageApp_Hdr.h"

BOOL CALLBACK XEngine_Callback_DownloadLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bDLEnable)
	{
		OPenSsl_Server_AcceptEx(xhDLSsl, hSocket, lpszClientAddr);
	}
	RfcComponents_HttpServer_CreateClientEx(xhDLHttp, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHBDownload, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_DownloadRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bDLEnable)
	{
		int nSLen = 0;
		TCHAR* ptszMsgBuffer = NULL;
		OPenSsl_Server_RecvMemoryEx(xhDLSsl, lpszClientAddr, &ptszMsgBuffer, &nSLen, lpszRecvMsg, nMsgLen);
		if (!RfcComponents_HttpServer_InserQueueEx(xhDLHttp, lpszClientAddr, ptszMsgBuffer, nSLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
			return;
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		if (!RfcComponents_HttpServer_InserQueueEx(xhDLHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
			return;
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBDownload, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("下载客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_DownloadLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_BYSELF, STORAGE_NETTYPE_HTTPDOWNLOAD);
}
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK XEngine_Callback_UPLoaderLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bUPEnable)
	{
		OPenSsl_Server_AcceptEx(xhUPSsl, hSocket, lpszClientAddr);
	}
	RfcComponents_HttpServer_CreateClientEx(xhUPHttp, lpszClientAddr, 0);
	RfcComponents_HttpServer_SetRecvModeEx(xhUPHttp, lpszClientAddr, 1);   //设置为文件接受模式
	SocketOpt_HeartBeat_InsertAddrEx(xhHBUPLoader, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_UPLoaderRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bUPEnable)
	{
		int nSLen = 0;
		TCHAR* ptszMsgBuffer = NULL;
		OPenSsl_Server_RecvMemoryEx(xhUPSsl, lpszClientAddr, &ptszMsgBuffer, &nSLen, lpszRecvMsg, nMsgLen);
		if (!RfcComponents_HttpServer_InserQueueEx(xhUPHttp, lpszClientAddr, ptszMsgBuffer, nSLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
			return;
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		if (!RfcComponents_HttpServer_InserQueueEx(xhUPHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
			return;
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBUPLoader, lpszClientAddr);

	int nCount = 0;
	__int64u nTimeWait = 0;
	Session_UPStorage_GetAll(NULL, &nCount);
	Algorithm_Calculation_SleepFlow(xhLimit, &nTimeWait, st_ServiceCfg.st_XLimit.nMaxUPLoader, nCount, nMsgLen);
	std::this_thread::sleep_for(std::chrono::microseconds(nTimeWait));
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("上传客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_UPLoaderLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_BYSELF, STORAGE_NETTYPE_HTTPUPLOADER);
}
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK XEngine_Callback_CenterLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bCHEnable)
	{
		OPenSsl_Server_AcceptEx(xhCHSsl, hSocket, lpszClientAddr);
	}
	RfcComponents_HttpServer_CreateClientEx(xhCenterHttp, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHBCenter, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_CenterRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bCHEnable)
	{
		int nSLen = 0;
		TCHAR* ptszMsgBuffer = NULL;
		OPenSsl_Server_RecvMemoryEx(xhCHSsl, lpszClientAddr, &ptszMsgBuffer, &nSLen, lpszRecvMsg, nMsgLen);
		if (!RfcComponents_HttpServer_InserQueueEx(xhCenterHttp, lpszClientAddr, ptszMsgBuffer, nSLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
			return;
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		if (!RfcComponents_HttpServer_InserQueueEx(xhCenterHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpServer_GetLastError());
			return;
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBCenter, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("业务客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_CenterLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_BYSELF, STORAGE_NETTYPE_HTTPCENTER);
}
//////////////////////////////////////////////////////////////////////////
void CALLBACK XEngine_Callback_HBDownload(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_HEARTBEAT, STORAGE_NETTYPE_HTTPDOWNLOAD);
}
void CALLBACK XEngine_Callback_HBUPLoader(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_HEARTBEAT, STORAGE_NETTYPE_HTTPUPLOADER);
}
void CALLBACK XEngine_Callback_HBCenter(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_HEARTBEAT, STORAGE_NETTYPE_HTTPCENTER);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Net_CloseClient(LPCTSTR lpszClientAddr, int nLeaveType, int nClientType)
{
	LPCTSTR lpszLeaveMsg;
	string m_StrClient;

	if (STORAGE_NETTYPE_HTTPUPLOADER == nClientType)
	{
		m_StrClient = _T("上传客户端");
		if (STORAGE_LEAVETYPE_HEARTBEAT == nLeaveType)
		{
			lpszLeaveMsg = _T("心跳超时");
			NetCore_TCPXCore_CloseForClientEx(xhNetUPLoader, lpszClientAddr);
		}
		else if (STORAGE_LEAVETYPE_BYSELF == nLeaveType)
		{
			lpszLeaveMsg = _T("被动断开");
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBUPLoader, lpszClientAddr);
		}
		else
		{
			lpszLeaveMsg = _T("主动关闭");
			NetCore_TCPXCore_CloseForClientEx(xhNetUPLoader, lpszClientAddr);
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBUPLoader, lpszClientAddr);
		}
		Session_UPStroage_Delete(lpszClientAddr);
		RfcComponents_HttpServer_CloseClinetEx(xhUPHttp, lpszClientAddr);
		OPenSsl_Server_CloseClientEx(xhUPSsl, lpszClientAddr);
	}
	else if (STORAGE_NETTYPE_HTTPDOWNLOAD == nClientType)
	{
		m_StrClient = _T("下载客户端");
		if (STORAGE_LEAVETYPE_HEARTBEAT == nLeaveType)
		{
			lpszLeaveMsg = _T("心跳超时");
			NetCore_TCPXCore_CloseForClientEx(xhNetDownload, lpszClientAddr);
		}
		else if (STORAGE_LEAVETYPE_BYSELF == nLeaveType)
		{
			lpszLeaveMsg = _T("被动断开");
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBDownload, lpszClientAddr);
		}
		else
		{
			lpszLeaveMsg = _T("主动关闭");
			NetCore_TCPXCore_CloseForClientEx(xhNetDownload, lpszClientAddr);
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBDownload, lpszClientAddr);
		}
		Session_DLStroage_Delete(lpszClientAddr);
		RfcComponents_HttpServer_CloseClinetEx(xhDLHttp, lpszClientAddr);
		OPenSsl_Server_CloseClientEx(xhDLSsl, lpszClientAddr);
	}
	else if (STORAGE_NETTYPE_HTTPCENTER == nClientType)
	{
		m_StrClient = _T("业务客户端");
		if (STORAGE_LEAVETYPE_HEARTBEAT == nLeaveType)
		{
			lpszLeaveMsg = _T("心跳超时");
			NetCore_TCPXCore_CloseForClientEx(xhNetCenter, lpszClientAddr);
		}
		else if (STORAGE_LEAVETYPE_BYSELF == nLeaveType)
		{
			lpszLeaveMsg = _T("被动断开");
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBCenter, lpszClientAddr);
		}
		else
		{
			lpszLeaveMsg = _T("主动关闭");

			NetCore_TCPXCore_CloseForClientEx(xhNetCenter, lpszClientAddr);
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBCenter, lpszClientAddr);
		}
		RfcComponents_HttpServer_CloseClinetEx(xhCenterHttp, lpszClientAddr);
		OPenSsl_Server_CloseClientEx(xhCHSsl, lpszClientAddr);
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("%s：%s，与服务器断开，原因：%s"), m_StrClient.c_str(), lpszClientAddr, lpszLeaveMsg);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
BOOL XEngine_Net_SendMsg(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nType)
{
	BOOL bRet = FALSE;

	if (STORAGE_NETTYPE_HTTPDOWNLOAD == nType)
	{
		if (st_ServiceCfg.st_XCert.bDLEnable)
		{
			int nSLen = 0;
			TCHAR *ptszMsgBuffer = NULL;

			OPenSsl_Server_SendMemoryEx(xhDLSsl, lpszClientAddr, lpszMsgBuffer, nMsgLen, &ptszMsgBuffer, &nSLen);
			bRet = NetCore_TCPXCore_SendEx(xhNetDownload, lpszClientAddr, ptszMsgBuffer, nSLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		else
		{
			bRet = NetCore_TCPXCore_SendEx(xhNetDownload, lpszClientAddr, lpszMsgBuffer, nMsgLen, 0, 10);
		}
		if (bRet && st_ServiceCfg.st_XTime.bHBTime)
		{
			SocketOpt_HeartBeat_ActiveAddrEx(xhHBDownload, lpszClientAddr);
		}
	}
	else if(STORAGE_NETTYPE_HTTPUPLOADER == nType)
	{
		if (st_ServiceCfg.st_XCert.bUPEnable)
		{
			int nSLen = 0;
			TCHAR* ptszMsgBuffer = NULL;

			OPenSsl_Server_SendMemoryEx(xhUPSsl, lpszClientAddr, lpszMsgBuffer, nMsgLen, &ptszMsgBuffer, &nSLen);
			bRet = NetCore_TCPXCore_SendEx(xhNetUPLoader, lpszClientAddr, ptszMsgBuffer, nSLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		else
		{
			bRet = NetCore_TCPXCore_SendEx(xhNetUPLoader, lpszClientAddr, lpszMsgBuffer, nMsgLen);
		}
		if (bRet && st_ServiceCfg.st_XTime.bHBTime)
		{
			SocketOpt_HeartBeat_ActiveAddrEx(xhHBUPLoader, lpszClientAddr);
		}
	}
	else if (STORAGE_NETTYPE_HTTPCENTER == nType)
	{
		if (st_ServiceCfg.st_XCert.bCHEnable)
		{
			int nSLen = 0;
			TCHAR* ptszMsgBuffer = NULL;

			OPenSsl_Server_SendMemoryEx(xhCHSsl, lpszClientAddr, lpszMsgBuffer, nMsgLen, &ptszMsgBuffer, &nSLen);
			bRet = NetCore_TCPXCore_SendEx(xhNetCenter, lpszClientAddr, ptszMsgBuffer, nSLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		else
		{
			bRet = NetCore_TCPXCore_SendEx(xhNetCenter, lpszClientAddr, lpszMsgBuffer, nMsgLen);
		}
		if (bRet && st_ServiceCfg.st_XTime.bHBTime)
		{
			SocketOpt_HeartBeat_ActiveAddrEx(xhHBCenter, lpszClientAddr);
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
