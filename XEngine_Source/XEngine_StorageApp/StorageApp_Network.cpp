#include "StorageApp_Hdr.h"

bool CALLBACK XEngine_Callback_DownloadLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bDLEnable)
	{
		OPenSsl_Server_AcceptEx(xhDLSsl, hSocket, lpszClientAddr);
	}
	HttpProtocol_Server_CreateClientEx(xhDLHttp, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHBDownload, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("下载客户端：%s，进入了服务器"), lpszClientAddr);
	return true;
}
void CALLBACK XEngine_Callback_DownloadRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bDLEnable)
	{
		int nSLen = 0;
		XCHAR* ptszMsgBuffer = NULL;
		OPenSsl_Server_RecvMemoryEx(xhDLSsl, lpszClientAddr, &ptszMsgBuffer, &nSLen, lpszRecvMsg, nMsgLen);
		if (!HttpProtocol_Server_InserQueueEx(xhDLHttp, lpszClientAddr, ptszMsgBuffer, nSLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("下载客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpProtocol_GetLastError());
			return;
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		if (!HttpProtocol_Server_InserQueueEx(xhDLHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("下载客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpProtocol_GetLastError());
			return;
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBDownload, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("下载客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_DownloadLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_BYSELF, STORAGE_NETTYPE_HTTPDOWNLOAD);
}
//////////////////////////////////////////////////////////////////////////
bool CALLBACK XEngine_Callback_UPLoaderLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bUPEnable)
	{
		OPenSsl_Server_AcceptEx(xhUPSsl, hSocket, lpszClientAddr);
	}
	HttpProtocol_Server_CreateClientEx(xhUPHttp, lpszClientAddr, 0);
	HttpProtocol_Server_SetRecvModeEx(xhUPHttp, lpszClientAddr, 1);   //设置为文件接受模式
	SocketOpt_HeartBeat_InsertAddrEx(xhHBUPLoader, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端：%s，进入了服务器"), lpszClientAddr);
	return true;
}
void CALLBACK XEngine_Callback_UPLoaderRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bUPEnable)
	{
		int nSLen = 0;
		XCHAR* ptszMsgBuffer = NULL;
		OPenSsl_Server_RecvMemoryEx(xhUPSsl, lpszClientAddr, &ptszMsgBuffer, &nSLen, lpszRecvMsg, nMsgLen);
		if (!HttpProtocol_Server_InserQueueEx(xhUPHttp, lpszClientAddr, ptszMsgBuffer, nSLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpProtocol_GetLastError());
			return;
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		if (!HttpProtocol_Server_InserQueueEx(xhUPHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpProtocol_GetLastError());
			return;
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBUPLoader, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("上传客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_UPLoaderLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_BYSELF, STORAGE_NETTYPE_HTTPUPLOADER);
}
//////////////////////////////////////////////////////////////////////////
bool CALLBACK XEngine_Callback_CenterLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bCHEnable)
	{
		OPenSsl_Server_AcceptEx(xhCHSsl, hSocket, lpszClientAddr);
	}
	HttpProtocol_Server_CreateClientEx(xhCenterHttp, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHBCenter, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端：%s，进入了服务器"), lpszClientAddr);
	return true;
}
void CALLBACK XEngine_Callback_CenterRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bCHEnable)
	{
		int nSLen = 0;
		XCHAR* ptszMsgBuffer = NULL;
		OPenSsl_Server_RecvMemoryEx(xhCHSsl, lpszClientAddr, &ptszMsgBuffer, &nSLen, lpszRecvMsg, nMsgLen);
		if (!HttpProtocol_Server_InserQueueEx(xhCenterHttp, lpszClientAddr, ptszMsgBuffer, nSLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpProtocol_GetLastError());
			return;
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		if (!HttpProtocol_Server_InserQueueEx(xhCenterHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpProtocol_GetLastError());
			return;
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBCenter, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("业务客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_CenterLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_BYSELF, STORAGE_NETTYPE_HTTPCENTER);
}
//////////////////////////////////////////////////////////////////////////
bool CALLBACK XEngine_Callback_WebdavLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bWDEnable)
	{
		OPenSsl_Server_AcceptEx(xhWDSsl, hSocket, lpszClientAddr);
	}
	HttpProtocol_Server_CreateClientEx(xhWebdavHttp, lpszClientAddr, 0);
	SocketOpt_HeartBeat_InsertAddrEx(xhHBWebdav, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端：%s，进入了服务器"), lpszClientAddr);
	return true;
}
void CALLBACK XEngine_Callback_WebdavRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (st_ServiceCfg.st_XCert.bWDEnable)
	{
		int nSLen = 0;
		XCHAR* ptszMsgBuffer = NULL;
		OPenSsl_Server_RecvMemoryEx(xhWDSsl, lpszClientAddr, &ptszMsgBuffer, &nSLen, lpszRecvMsg, nMsgLen);
		if (!HttpProtocol_Server_InserQueueEx(xhWebdavHttp, lpszClientAddr, ptszMsgBuffer, nSLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpProtocol_GetLastError());
			return;
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	else
	{
		if (!HttpProtocol_Server_InserQueueEx(xhWebdavHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端：%s，投递数据失败,大小:%d,错误;%lX"), lpszClientAddr, nMsgLen, HttpProtocol_GetLastError());
			return;
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHBWebdav, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("WEBDAV客户端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_WebdavLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_BYSELF, STORAGE_NETTYPE_HTTPWEBDAV);
}
//////////////////////////////////////////////////////////////////////////
void CALLBACK XEngine_Callback_HBDownload(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_HEARTBEAT, STORAGE_NETTYPE_HTTPDOWNLOAD);
}
void CALLBACK XEngine_Callback_HBUPLoader(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_HEARTBEAT, STORAGE_NETTYPE_HTTPUPLOADER);
}
void CALLBACK XEngine_Callback_HBCenter(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_HEARTBEAT, STORAGE_NETTYPE_HTTPCENTER);
}
void CALLBACK XEngine_Callback_HBWebdav(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_HEARTBEAT, STORAGE_NETTYPE_HTTPWEBDAV);
}
//////////////////////////////////////////////////////////////////////////
bool XEngine_Net_CloseClient(LPCXSTR lpszClientAddr, int nLeaveType, int nClientType)
{
	string m_StrLeaveMsg;
	string m_StrClient;

	if (STORAGE_NETTYPE_HTTPUPLOADER == nClientType)
	{
		m_StrClient = _X("上传客户端");
		if (STORAGE_LEAVETYPE_HEARTBEAT == nLeaveType)
		{
			m_StrLeaveMsg = _X("心跳超时");
			NetCore_TCPXCore_CloseForClientEx(xhNetUPLoader, lpszClientAddr);
		}
		else if (STORAGE_LEAVETYPE_BYSELF == nLeaveType)
		{
			m_StrLeaveMsg = _X("被动断开");
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBUPLoader, lpszClientAddr);
		}
		else
		{
			m_StrLeaveMsg = _X("主动关闭");
			NetCore_TCPXCore_CloseForClientEx(xhNetUPLoader, lpszClientAddr);
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBUPLoader, lpszClientAddr);
		}
		Algorithm_Calculation_PassiveClose(Session_UPStroage_GetSpeed(lpszClientAddr), true);
		Algorithm_Calculation_Close(Session_UPStroage_GetSpeed(lpszClientAddr));
		Session_UPStroage_Delete(lpszClientAddr);
		HttpProtocol_Server_CloseClinetEx(xhUPHttp, lpszClientAddr);
		OPenSsl_Server_CloseClientEx(xhUPSsl, lpszClientAddr);
	}
	else if (STORAGE_NETTYPE_HTTPDOWNLOAD == nClientType)
	{
		m_StrClient = _X("下载客户端");
		if (STORAGE_LEAVETYPE_HEARTBEAT == nLeaveType)
		{
			m_StrLeaveMsg = _X("心跳超时");
			NetCore_TCPXCore_CloseForClientEx(xhNetDownload, lpszClientAddr);
		}
		else if (STORAGE_LEAVETYPE_BYSELF == nLeaveType)
		{
			m_StrLeaveMsg = _X("被动断开");
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBDownload, lpszClientAddr);
		}
		else
		{
			m_StrLeaveMsg = _X("主动关闭");
			NetCore_TCPXCore_CloseForClientEx(xhNetDownload, lpszClientAddr);
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBDownload, lpszClientAddr);
		}
		SESSION_STORAGEINFO st_StorageInfo;
		memset(&st_StorageInfo, '\0', sizeof(SESSION_STORAGEINFO));

		if (Session_DLStroage_GetInfo(lpszClientAddr, &st_StorageInfo))
		{
			Algorithm_Calculation_Close(st_StorageInfo.xhToken);
			Session_DLStroage_Delete(lpszClientAddr);
		}
		
		HttpProtocol_Server_CloseClinetEx(xhDLHttp, lpszClientAddr);
		OPenSsl_Server_CloseClientEx(xhDLSsl, lpszClientAddr);
	}
	else if (STORAGE_NETTYPE_HTTPCENTER == nClientType)
	{
		m_StrClient = _X("业务客户端");
		if (STORAGE_LEAVETYPE_HEARTBEAT == nLeaveType)
		{
			m_StrLeaveMsg = _X("心跳超时");
			NetCore_TCPXCore_CloseForClientEx(xhNetCenter, lpszClientAddr);
		}
		else if (STORAGE_LEAVETYPE_BYSELF == nLeaveType)
		{
			m_StrLeaveMsg = _X("被动断开");
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBCenter, lpszClientAddr);
		}
		else
		{
			m_StrLeaveMsg = _X("主动关闭");

			NetCore_TCPXCore_CloseForClientEx(xhNetCenter, lpszClientAddr);
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBCenter, lpszClientAddr);
		}
		HttpProtocol_Server_CloseClinetEx(xhCenterHttp, lpszClientAddr);
		OPenSsl_Server_CloseClientEx(xhCHSsl, lpszClientAddr);
	}
	else if (STORAGE_NETTYPE_HTTPWEBDAV == nClientType)
	{
		m_StrClient = _X("WEBDAV客户端");
		if (STORAGE_LEAVETYPE_HEARTBEAT == nLeaveType)
		{
			m_StrLeaveMsg = _X("心跳超时");
			NetCore_TCPXCore_CloseForClientEx(xhNetWebdav, lpszClientAddr);
		}
		else if (STORAGE_LEAVETYPE_BYSELF == nLeaveType)
		{
			m_StrLeaveMsg = _X("被动断开");
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBWebdav, lpszClientAddr);
		}
		else
		{
			m_StrLeaveMsg = _X("主动关闭");

			NetCore_TCPXCore_CloseForClientEx(xhNetWebdav, lpszClientAddr);
			SocketOpt_HeartBeat_DeleteAddrEx(xhHBWebdav, lpszClientAddr);
		}
		HttpProtocol_Server_CloseClinetEx(xhWebdavHttp, lpszClientAddr);
		OPenSsl_Server_CloseClientEx(xhWDSsl, lpszClientAddr);
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("%s：%s，与服务器断开，原因：%s"), m_StrClient.c_str(), lpszClientAddr, m_StrLeaveMsg.c_str());
	return true;
}
///////////////////////////////////////////////////////////////////////////////////
bool XEngine_Net_SendMsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nType)
{
	bool bRet = false;

	if (STORAGE_NETTYPE_HTTPDOWNLOAD == nType)
	{
		if (st_ServiceCfg.st_XCert.bDLEnable)
		{
			int nSLen = 0;
			XCHAR *ptszMsgBuffer = NULL;

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
			XCHAR* ptszMsgBuffer = NULL;

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
			XCHAR* ptszMsgBuffer = NULL;

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
	else if (STORAGE_NETTYPE_HTTPWEBDAV == nType)
	{
		if (st_ServiceCfg.st_XCert.bWDEnable)
		{
			int nSLen = 0;
			XCHAR* ptszMsgBuffer = NULL;

			OPenSsl_Server_SendMemoryEx(xhWDSsl, lpszClientAddr, lpszMsgBuffer, nMsgLen, &ptszMsgBuffer, &nSLen);
			bRet = NetCore_TCPXCore_SendEx(xhNetWebdav, lpszClientAddr, ptszMsgBuffer, nSLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		else
		{
			bRet = NetCore_TCPXCore_SendEx(xhNetWebdav, lpszClientAddr, lpszMsgBuffer, nMsgLen);
		}
		if (bRet && st_ServiceCfg.st_XTime.bHBTime)
		{
			SocketOpt_HeartBeat_ActiveAddrEx(xhHBWebdav, lpszClientAddr);
		}
	}
	if (!bRet)
	{
		XLONG dwRet = NetCore_GetLastError();
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("客户端：%s，网络类型:%d,发送数据失败，发送大小：%d，错误：%lX,%d"), lpszClientAddr, nType, nMsgLen, dwRet, errno);
		return false;
	}
	return true;
}
