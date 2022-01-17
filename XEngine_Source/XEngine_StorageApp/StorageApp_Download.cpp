#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_Download_HTTPThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (RfcComponents_HttpServer_EventWaitEx(xhDLHttp, nThreadPos))
		{
			int nListCount = 0;
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;
			//获取当前队列池中所有触发下载客户端
			RfcComponents_HttpServer_GetPoolEx(xhDLHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					int nMsgLen = 0;
					int nHdrCount = 0;
					CHAR* ptszMsgBuffer = NULL;
					CHAR** ppszListHdr = NULL;
					RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParam;

					memset(&st_HTTPParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
					//获得指定下载客户端触发信息
					if (RfcComponents_HttpServer_GetMemoryEx(xhDLHttp, ppSt_PKTClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpDownload(ppSt_PKTClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
					}
					BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
					BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListHdr, nHdrCount);
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}

void CALLBACK XEngine_Download_CBSend(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	int nMsgLen = 4096;
	int nListCount = 0;
	__int64u nTimeWait = 0;
	TCHAR tszMsgBuffer[4096];

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (Session_DLStroage_GetBuffer(lpszClientAddr, tszMsgBuffer, &nMsgLen))
	{
		if (nMsgLen <= 0)
		{
			if (st_ServiceCfg.st_XProxy.st_XProxyPass.bDLPass)
			{
				int nPLen = MAX_PATH;
				int nHttpCode = 0;
				TCHAR tszProxyStr[MAX_PATH];
				SESSION_STORAGEINFO st_StorageInfo;

				memset(tszProxyStr, '\0', MAX_PATH);
				memset(&st_StorageInfo, '\0', sizeof(SESSION_STORAGEINFO));

				Session_DLStroage_GetInfo(lpszClientAddr, &st_StorageInfo);

				Protocol_StoragePacket_UPDown(tszProxyStr, &nPLen, st_StorageInfo.tszFileDir, st_StorageInfo.tszBuckKey, st_StorageInfo.tszClientAddr, st_StorageInfo.ullRWCount, st_StorageInfo.tszFileHash);
				if (APIHelp_HttpRequest_Post(st_ServiceCfg.st_XProxy.st_XProxyPass.tszDLPass, tszProxyStr, &nHttpCode))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,请求完成通知返回值:%d,文件:%s,地址:%s"), lpszClientAddr, nHttpCode, st_StorageInfo.tszFileDir, st_ServiceCfg.st_XProxy.st_XProxyPass.tszDLPass);
				}
				else
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,请求完成通知失败,可能对方服务没有开启,文件:%s,地址:%s"), lpszClientAddr, st_StorageInfo.tszFileDir, st_ServiceCfg.st_XProxy.st_XProxyPass.tszDLPass);
				}
			}
			NetCore_TCPXCore_CBSendEx(xhNetDownload, lpszClientAddr);
			Session_DLStroage_Delete(lpszClientAddr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,文件已经发送完毕,用户已经被移除发送列表"), lpszClientAddr);
		}
		else
		{
			XEngine_Task_SendDownload(lpszClientAddr, tszMsgBuffer, nMsgLen);
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,获取用户对应文件内容失败,错误：%lX"), lpszClientAddr, Session_GetLastError());
	}
	//限速
	Session_DLStroage_GetCount(&nListCount);
	Algorithm_Calculation_SleepFlow(xhLimit, &nTimeWait, st_ServiceCfg.st_XLimit.nMaxDNLoader, nListCount, 4096);
	//WINDOWS下sleep_for精度可能不准
	std::this_thread::sleep_for(std::chrono::microseconds(nTimeWait));
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
	
	if (st_ServiceCfg.st_XProxy.st_XProxyAuth.bAuth)
	{
		TCHAR tszUserName[64];
		TCHAR tszUserPass[64];

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(tszUserPass, '\0', sizeof(tszUserPass));
		if (!APIHelp_Api_ProxyAuth(tszUserName, tszUserPass, pptszListHdr, nHdrCount))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.bAuth = TRUE;
			st_HDRParam.nHttpCode = 401;

			RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,用户验证失败,错误:%lX"), lpszClientAddr, APIHelp_GetLastError());
			return FALSE;
		}
		if (_tcslen(st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy) > 0)
		{
			int nBLen = 0;
			int nResponseCode = 0;
			TCHAR* ptszBody = NULL;

			Protocol_StoragePacket_BasicAuth(pSt_HTTPParam->tszHttpMethod, pSt_HTTPParam->tszHttpUri, lpszClientAddr, tszUserName, tszUserPass, tszSDBuffer, &nSDLen);
			APIHelp_HttpRequest_Post(st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy, tszSDBuffer, &nResponseCode, &ptszBody, &nBLen);
			if (200 != nResponseCode)
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.bAuth = TRUE;
				st_HDRParam.nHttpCode = nResponseCode;

				RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,用户验证失败,用户名:%s,密码:%s,错误码:%d,错误内容:%s"), tszUserName, tszUserPass, tszUserPass, nResponseCode, ptszBody);
			}
			BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszBody);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,代理服务:%s 验证通过,用户名:%s,密码:%s"), lpszClientAddr, st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy, tszUserName, tszUserPass);
		}
		else
		{
			if (!Session_User_Exist(tszUserName, tszUserPass))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.bAuth = TRUE;
				st_HDRParam.nHttpCode = 401;

				RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,验证用户失败,无法继续"), lpszClientAddr);
				return FALSE;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,本地验证用户验证通过,用户名:%s,密码:%s"), lpszClientAddr, tszUserName, tszUserPass);
		}
		st_HDRParam.bAuth = TRUE;
	}
	//使用重定向,这是分布式重定向实现
	if (st_LoadbalanceCfg.st_LBConfig.nDownldMode > 0)
	{
		TCHAR tszHdrBuffer[1024];
		TCHAR tszStorageAddr[128];

		memset(tszHdrBuffer, '\0', sizeof(tszHdrBuffer));
		memset(tszStorageAddr, '\0', sizeof(tszStorageAddr));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 302;
		
		APIHelp_Distributed_RandomAddr(st_LoadbalanceCfg.st_LoadBalance.pStl_ListDownload, tszStorageAddr, st_LoadbalanceCfg.st_LBConfig.nDownldMode);
		_stprintf(tszHdrBuffer, _T("Location: %s%s\r\n"), tszStorageAddr, pSt_HTTPParam->tszHttpUri);

		RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHdrBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,请求的文件被要求重定向到:%s%s"), lpszClientAddr, tszStorageAddr, pSt_HTTPParam->tszHttpUri);
		return TRUE;
	}
	
	BOOL bRange = FALSE;
	int nPosStart = 0;
	int nPosEnd = 0;
	__int64x nPosCount = 0;
	if (APIHelp_Api_RangeFile(&nPosStart, &nPosEnd, &nPosCount, pptszListHdr, nHdrCount))
	{
		//是否启用了断点续传
		if (st_ServiceCfg.st_XStorage.bResumable)
		{
			bRange = TRUE;
		}
		else
		{
			bRange = FALSE;
			nPosStart = 0;
			nPosEnd = 0;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,客户端的请求设置了数据范围:%d - %d/%lld"), lpszClientAddr, nPosStart, nPosEnd, nPosCount);
	}
	XENGINE_STORAGEBUCKET st_StorageBucket;
	memset(&st_StorageBucket, '\0', sizeof(XENGINE_STORAGEBUCKET));
	//分布式存储
	if (!APIHelp_Distributed_DLStorage(pSt_HTTPParam->tszHttpUri, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,请求文件失败,可能BUCKET:% 不正确,错误：%lX"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, APIHelp_GetLastError());
		return FALSE;
	}
	_stprintf(tszFileDir, _T("%s%s"), st_StorageBucket.tszFilePath, st_StorageBucket.tszFileName);

	int nHashLen = 0;
	UCHAR tszHashKey[MAX_PATH];
	TCHAR tszFieldStr[MAX_PATH];
	TCHAR tszHashStr[128];
	memset(tszHashKey, '\0', MAX_PATH);
	memset(tszFieldStr, '\0', MAX_PATH);
	memset(tszHashStr, '\0', sizeof(tszHashStr));
	//得到文件HASH
	OPenSsl_Api_Digest(tszFileDir, tszHashKey, &nHashLen, TRUE, st_ServiceCfg.st_XStorage.nHashMode);
	BaseLib_OperatorString_StrToHex((char*)tszHashKey, nHashLen, tszHashStr);
	BaseLib_OperatorString_GetFileAndPath(tszFileDir, NULL, NULL, NULL, st_HDRParam.tszMimeType);
	//插入数据
	if (!Session_DLStroage_Insert(lpszClientAddr, st_StorageBucket.tszBuckKey, tszFileDir, &ullCount, &ullSize, nPosStart, nPosEnd, tszHashStr))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 404;

		RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,插入用户请求失败,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
		return FALSE;
	}
	//是否续传
	if (bRange)
	{
		st_HDRParam.st_Range.nPosStart = nPosStart;
		st_HDRParam.st_Range.nPosEnd = nPosEnd;
		st_HDRParam.st_Range.nPosCount = ullCount;
		st_HDRParam.nHttpCode = 206;
		st_HDRParam.bIsClose = TRUE;

		if (0 == st_HDRParam.st_Range.nPosEnd)
		{
			st_HDRParam.st_Range.nPosEnd = ullCount;
		}
	}
	else
	{
		st_HDRParam.nHttpCode = 200;
		st_HDRParam.bIsClose = TRUE;
	}
	_stprintf(tszFieldStr, _T("FileHash: %s\r\n"), tszHashStr);
	RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, ullSize, tszFieldStr);
	XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
	//不能在send之前调用
	if (!NetCore_TCPXCore_CBSendEx(xhNetDownload, lpszClientAddr, XEngine_Download_CBSend))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 404;

		RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("下载客户端:%s,设置回调下载失败,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
		return FALSE;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("下载客户端:%s,请求下载文件成功,文件名:%s,总大小:%llu,发送大小:%llu,范围:%d - %d"), lpszClientAddr, tszFileDir, ullCount, ullSize, nPosStart, nPosEnd);
	return TRUE;
}
BOOL XEngine_Task_SendDownload(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	if (!XEngine_Net_SendMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen, STORAGE_NETTYPE_HTTPDOWNLOAD))
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
		return FALSE;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("下载客户端:%s,正在发送文件数据,大小:%d"), lpszClientAddr, nMsgLen);
	return TRUE;
}
