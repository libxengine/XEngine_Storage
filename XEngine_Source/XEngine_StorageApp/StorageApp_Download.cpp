#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_Download_HTTPThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	TCHAR tszClientAddr[128];
	TCHAR tszMsgBuffer[4096];
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (RfcComponents_HttpServer_EventWaitEx(xhDLHttp, nThreadPos))
		{
			int nListCount = 0;
			int nMsgLen = 0;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParam;
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;

			memset(&st_HTTPParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
			memset(tszClientAddr, '\0', sizeof(tszClientAddr));
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			//获取当前队列池中所有触发下载客户端
			RfcComponents_HttpServer_GetPoolEx(xhDLHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					int nHdrCount = 0;
					CHAR** ppszListHdr = NULL;
					//获得指定下载客户端触发信息
					if (RfcComponents_HttpServer_GetClientEx(xhDLHttp, ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpDownload(ppSt_PKTClient[i]->tszClientAddr, tszClientAddr, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
					}
					BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListHdr, nHdrCount);
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}
XHTHREAD CALLBACK XEngine_Download_SendThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	TCHAR tszClientAddr[128];
	TCHAR tszMsgBuffer[4096];

	while (bIsRun)
	{
		int nListCount = 0;
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Session_DLStroage_GetCount(nThreadPos, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nMsgLen = 4096;
			if (!Session_DLStroage_GetList(nThreadPos, i, tszClientAddr, tszMsgBuffer, &nMsgLen))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,获取用户对应文件内容失败,错误：%lX"), tszClientAddr, Session_GetLastError());
				continue;
			}
			if (nMsgLen <= 0)
			{
				if (st_ServiceCfg.st_XProxy.st_XProxyPass.bDLPass)
				{
					int nPLen = MAX_PATH;
					int nHttpCode = 0;
					UCHAR tszHashKey[MAX_PATH];
					TCHAR tszHashStr[MAX_PATH];
					TCHAR tszProxyStr[MAX_PATH];
					SESSION_STORAGEINFO st_StorageInfo;

					memset(tszHashKey, '\0', MAX_PATH);
					memset(tszHashStr, '\0', MAX_PATH);
					memset(tszProxyStr, '\0', MAX_PATH);
					memset(&st_StorageInfo, '\0', sizeof(SESSION_STORAGEINFO));

					OPenSsl_Api_Digest(st_StorageInfo.tszFileDir, tszHashKey, NULL, TRUE, st_ServiceCfg.st_XStorage.nHashMode);
					BaseLib_OperatorString_StrToHex((char*)tszHashKey, 20, tszHashStr);
					Session_DLStroage_GetInfo(nThreadPos, i, &st_StorageInfo);

					XStorageProtocol_Proxy_PacketUPDown(st_StorageInfo.tszFileDir, st_StorageInfo.tszClientAddr, st_StorageInfo.ullRWCount, tszProxyStr, &nPLen, tszHashStr);
					APIHelp_HttpRequest_Post(st_ServiceCfg.st_XProxy.st_XProxyPass.tszDLPass, tszProxyStr, &nHttpCode);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_NOTICE, _T("下载客户端:%s,请求完成通知返回值:%d,文件:%s,地址:%s"), tszClientAddr, nHttpCode, st_StorageInfo.tszFileDir, st_ServiceCfg.st_XProxy.st_XProxyPass.tszDLPass);
				}
				Session_DLStroage_Delete(tszClientAddr);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_NOTICE, _T("下载客户端:%s,文件已经发送完毕,用户已经被移除发送列表"), tszClientAddr);
				continue;
			}
			XEngine_Task_SendDownload(tszClientAddr, tszMsgBuffer, nMsgLen);
		}
		int nTimeWait = 1;
#ifdef _WINDOWS
		st_ServiceCfg.st_XLimit.nMaxDNLoader = st_ServiceCfg.st_XLimit.nMaxDNLoader * 10;
#endif
		Algorithm_Calculation_SleepFlow(&nTimeWait, st_ServiceCfg.st_XLimit.nMaxDNLoader, nListCount, 4096);
		std::this_thread::sleep_for(std::chrono::microseconds(nTimeWait));
	}
	return 0;
}

BOOL XEngine_Task_HttpDownload(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	__int64x ullCount = 0;     //总大小
	__int64x ullSize = 0;      //需要下载大小
	TCHAR tszSDBuffer[2048];
	TCHAR tszFileDir[1024];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszFileDir, '\0', sizeof(tszFileDir));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	LPCTSTR lpszMethodGet = _T("GET");
	if (0 != _tcsncmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 405;

		RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,发送的方法不支持"), lpszClientAddr);
		return FALSE;
	}
	if (!XEngine_APPHelp_ProxyAuth(lpszClientAddr, lpszMethodGet, pSt_HTTPParam->tszHttpUri, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPDOWNLOAD))
	{
		return FALSE;
	}
	if (st_ServiceCfg.st_XProxy.st_XProxyAuth.bAuth)
	{
		st_HDRParam.bAuth = TRUE;
	}
	//使用重定向,这是分布式重定向实现
	if (APIHelp_Distributed_IsMode(st_LoadbalanceCfg.st_LoadBalance.pStl_ListUseMode, STORAGE_NETTYPE_HTTPDOWNLOAD))
	{
		TCHAR tszHdrBuffer[1024];
		TCHAR tszStorageAddr[128];

		memset(tszHdrBuffer, '\0', sizeof(tszHdrBuffer));
		memset(tszStorageAddr, '\0', sizeof(tszStorageAddr));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 302;
		
		APIHelp_Distributed_RandomAddr(st_LoadbalanceCfg.st_LoadBalance.pStl_ListDownload, tszStorageAddr);
		_stprintf(tszHdrBuffer, _T("Location: %s%s\r\n"), tszStorageAddr, pSt_HTTPParam->tszHttpUri);

		RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHdrBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,请求的文件被要求重定向到:%s%s"), lpszClientAddr, tszStorageAddr, pSt_HTTPParam->tszHttpUri);
		return TRUE;
	}
	
	BOOL bRange = FALSE;
	int nPosStart = 0;
	int nPosEnd = 0;
	if (XEngine_APPHelp_RangeFile(lpszClientAddr, &nPosStart, &nPosEnd, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPDOWNLOAD))
	{
		bRange = TRUE;
	}

	_stprintf(tszFileDir, _T("%s%s"), st_ServiceCfg.st_XStorage.tszFileDir, pSt_HTTPParam->tszHttpUri);
	if (!Session_DLStroage_Insert(lpszClientAddr, tszFileDir, &ullCount, &ullSize, nPosStart, nPosEnd))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 404;

		RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,插入用户请求失败,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
		return FALSE;
	}

	if (bRange)
	{
		st_HDRParam.st_Range.nPosStart = nPosStart;
		st_HDRParam.st_Range.nPosEnd = nPosEnd;
		st_HDRParam.st_Range.nPosCount = ullCount;
		st_HDRParam.nHttpCode = 206;
		st_HDRParam.bIsClose = TRUE;
	}
	else
	{
		st_HDRParam.nHttpCode = 200;
		st_HDRParam.bIsClose = TRUE;
	}
	_tcscpy(st_HDRParam.tszMimeType, _T("bin"));
	RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, ullSize);
	XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,请求下载文件成功,文件名:%s,总大小:%llu,发送大小:%llu,范围:%d - %d"), lpszClientAddr, tszFileDir, ullCount, ullSize, nPosStart, nPosEnd);
	return TRUE;
}
BOOL XEngine_Task_SendDownload(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	if (XEngine_Net_SendMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen, STORAGE_NETTYPE_HTTPDOWNLOAD))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("下载客户端:%s,正在发送文件数据,大小:%d"), lpszClientAddr, nMsgLen);
	}
	else
	{
		if (Session_DLStorage_SetSeek(lpszClientAddr, -nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("下载客户端:%s,正在发送文件数据,发送失败,移动指针:%d"), lpszClientAddr, -nMsgLen);
		}
		else
		{
			XEngine_Net_CloseClient(lpszClientAddr, STORAGE_LEAVETYPE_CLOSE, STORAGE_NETTYPE_HTTPDOWNLOAD);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,正在发送文件数据,大小:%d,发送超过重试次数,无法继续,移除发送队列"), lpszClientAddr, nMsgLen);
		}
	}
	return TRUE;
}
