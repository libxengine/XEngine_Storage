#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_UPLoader_HTTPThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (RfcComponents_HttpServer_EventWaitEx(xhUPHttp, nThreadPos))
		{
			int nListCount = 0;
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			RfcComponents_HttpServer_GetPoolEx(xhUPHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
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
					//获得指定上传客户端触发信息
					if (RfcComponents_HttpServer_GetMemoryEx(xhUPHttp, ppSt_PKTClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpUPLoader(ppSt_PKTClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
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
BOOL XEngine_Task_HttpUPLoader(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	TCHAR tszFileDir[1024];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszFileDir, '\0', sizeof(tszFileDir));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	LPCTSTR lpszMethodGet = _T("PUT");

	if (0 != _tcsncmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 405;

		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParam->tszHttpMethod);
		return FALSE;
	}
	if (!XEngine_APPHelp_ProxyAuth(lpszClientAddr, lpszMethodGet, pSt_HTTPParam->tszHttpUri, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPUPLOADER))
	{
		return FALSE;
	}
	if (st_ServiceCfg.st_XProxy.st_XProxyAuth.bAuth)
	{
		st_HDRParam.bAuth = TRUE;
	}
	//使用重定向?
	if (APIHelp_Distributed_IsMode(st_LoadbalanceCfg.st_LoadBalance.pStl_ListUseMode, STORAGE_NETTYPE_HTTPUPLOADER))
	{
		TCHAR tszHdrBuffer[1024];
		TCHAR tszStorageAddr[128];

		memset(tszHdrBuffer, '\0', sizeof(tszHdrBuffer));
		memset(tszStorageAddr, '\0', sizeof(tszStorageAddr));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 302;

		APIHelp_Distributed_RandomAddr(st_LoadbalanceCfg.st_LoadBalance.pStl_ListUPLoader, tszStorageAddr);
		_stprintf(tszHdrBuffer, _T("Location: %s%s\r\n"), tszStorageAddr, pSt_HTTPParam->tszHttpUri);

		RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHdrBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求的文件被要求重定向到:%s%s"), lpszClientAddr, tszStorageAddr, pSt_HTTPParam->tszHttpUri);
		return TRUE;
	}

	int nRVMode = 0;
	int nRVCount = 0;
	int nHDSize = 0;
	XENGINE_STORAGEBUCKET st_StorageBucket;
	memset(&st_StorageBucket, '\0', sizeof(XENGINE_STORAGEBUCKET));

	if (!APIHelp_Distributed_UPStorage(st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,请求上传文件失败,可能BUCKET:% 不正确,错误：%lX"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, APIHelp_GetLastError());
		return FALSE;
	}
	_stprintf(tszFileDir, _T("%s%s"), st_StorageBucket.tszFilePath, pSt_HTTPParam->tszHttpUri);

	if (!Session_UPStroage_Exist(lpszClientAddr))
	{
		int nPosStart = 0;
		int nPosEnd = 0;
		__int64x nPosCount = 0;
		//得到长度大小
		if (XEngine_APPHelp_RangeFile(lpszClientAddr, &nPosStart, &nPosEnd, &nPosCount, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPUPLOADER))
		{
			//是新的还是断点续传的
			if ((0 != nPosStart) || (0 != nPosEnd))
			{
				//是否启用了断点续传
				if (!st_ServiceCfg.st_XStorage.bResumable)
				{
					st_HDRParam.bIsClose = TRUE;
					st_HDRParam.nHttpCode = 416;

					RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
					XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,请求断点续传上传文件失败,服务端关闭了此功能,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
					return FALSE;
				}
			}
		}
		RfcComponents_HttpServer_GetRecvModeEx(xhUPHttp, lpszClientAddr, &nRVMode, &nRVCount, &nHDSize);
		if (!Session_UPStroage_Insert(lpszClientAddr, tszFileDir, nPosCount, nRVCount, nPosStart, nPosEnd))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 404;

			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,插入用户请求失败,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
			return FALSE;
		}
	}
	if (nMsgLen <= 0)
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 411;
		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,用户数据大小为0?"), lpszClientAddr);
		return FALSE;
	}
	Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nMsgLen);
	RfcComponents_HttpServer_GetRecvModeEx(xhUPHttp, lpszClientAddr, &nRVMode, &nRVCount, &nHDSize);
	if (nHDSize >= nRVCount)
	{
		SESSION_STORAGEINFO st_StorageInfo;
		memset(&st_StorageInfo, '\0', sizeof(SESSION_STORAGEINFO));

		Session_UPStroage_Close(lpszClientAddr);
		Session_UPStroage_GetInfo(lpszClientAddr, &st_StorageInfo);
		//大小是否足够
		if (st_StorageInfo.ullCount != st_StorageInfo.ullFSize)
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 200;
			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("上传客户端:%s,请求上传文件成功,文件名:%s,总大小:%lld,写入大小:%lld,文件不完整,需要等待断点续传完毕"), lpszClientAddr, tszFileDir, st_StorageInfo.ullCount, st_StorageInfo.ullFSize);
			return TRUE;
		}
		int nHashLen = 0;
		UCHAR tszHashStr[MAX_PATH];
		XSTORAGECORE_DBFILE st_ProtocolFile;

		memset(tszHashStr, '\0', MAX_PATH);
		memset(&st_ProtocolFile, '\0', sizeof(XSTORAGECORE_DBFILE));

		_tcscpy(st_ProtocolFile.st_ProtocolFile.tszFilePath, st_StorageBucket.tszFilePath);
		_tcscpy(st_ProtocolFile.st_ProtocolFile.tszFileName, pSt_HTTPParam->tszHttpUri + 1);
		st_ProtocolFile.st_ProtocolFile.nFileSize = nRVCount;

		OPenSsl_Api_Digest(tszFileDir, tszHashStr, &nHashLen, TRUE, st_ServiceCfg.st_XStorage.nHashMode);
		BaseLib_OperatorString_StrToHex((char*)tszHashStr, nHashLen, st_ProtocolFile.st_ProtocolFile.tszFileHash);
		//处理结果
		if (!XEngine_APPHelp_VerHash(lpszClientAddr, tszFileDir, st_ProtocolFile.st_ProtocolFile.tszFileHash, pptszListHdr, nHdrCount))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 403;
			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			return FALSE;
		}
		BOOL bRet = TRUE;
		if (0 != st_ServiceCfg.st_XSql.nSQLType)
		{
			_tcscpy(st_ProtocolFile.tszBuckKey, st_StorageBucket.tszBuckKey);
			if (1 == st_ServiceCfg.st_XSql.nSQLType)
			{
				bRet = XStorage_MySql_FileInsert(&st_ProtocolFile);
			}
			else
			{
				bRet = XStorage_SQLite_FileInsert(&st_ProtocolFile);
			}
			if (bRet)
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 200;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求上传文件成功,文件名:%s,大小:%d"), lpszClientAddr, tszFileDir, nRVCount);
			}
			else
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 403;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求上传文件失败,插入数据库失败:%s,错误:%lX"), lpszClientAddr, tszFileDir, XStorageDB_GetLastError());
			}
		}
		else
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 200;
			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("上传客户端:%s,请求上传文件成功,文件名:%s,大小:%d,数据库没有启用,不插入数据库"), lpszClientAddr, tszFileDir, nRVCount);
		}
		//PASS代理
		if (st_ServiceCfg.st_XProxy.st_XProxyPass.bUPPass && bRet)
		{
			int nPLen = MAX_PATH;
			int nHttpCode = 0;
			TCHAR tszProxyStr[MAX_PATH];
			SESSION_STORAGEINFO st_StorageInfo;

			memset(tszProxyStr, '\0', MAX_PATH);
			memset(&st_StorageInfo, '\0', sizeof(SESSION_STORAGEINFO));

			Session_UPStroage_GetInfo(lpszClientAddr, &st_StorageInfo);
			Protocol_StoragePacket_UPDown(st_StorageInfo.tszFileDir, st_StorageInfo.tszClientAddr, st_StorageInfo.ullRWCount, tszProxyStr, &nPLen, st_ProtocolFile.st_ProtocolFile.tszFileHash);
			if (APIHelp_HttpRequest_Post(st_ServiceCfg.st_XProxy.st_XProxyPass.tszUPPass, tszProxyStr, &nHttpCode))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求完成通知返回值:%d,文件:%s,地址:%s"), lpszClientAddr, nHttpCode, st_StorageInfo.tszFileDir, st_ServiceCfg.st_XProxy.st_XProxyPass.tszUPPass);
			}
			else
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,请求完成通知失败,可能服务器不正确:文件:%s,地址:%s"), lpszClientAddr, st_StorageInfo.tszFileDir, st_ServiceCfg.st_XProxy.st_XProxyPass.tszUPPass);
			}
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("上传客户端:%s,请求上传文件中,文件名:%s,大小:%d"), lpszClientAddr, tszFileDir, nMsgLen);
	}
	
	return TRUE;
}