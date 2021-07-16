#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_UPLoader_HTTPThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	TCHAR tszMsgBuffer[10240];
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (RfcComponents_HttpServer_EventWaitEx(xhUPHttp, nThreadPos))
		{
			int nListCount = 0;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParam;
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;

			memset(&st_HTTPParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			//获取当前队列池中所有触发上传客户端
			RfcComponents_HttpServer_GetPoolEx(xhUPHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					int nMsgLen = 10240;
					int nHdrCount = 0;
					CHAR** ppszListHdr = NULL;
					//获得指定上传客户端触发信息
					if (RfcComponents_HttpServer_GetClientEx(xhUPHttp, ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpUPLoader(ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
					}
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
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,发送的方法不支持"), lpszClientAddr);
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
	if (!Session_UPStroage_Exist(lpszClientAddr))
	{
		int nPosStart = 0;
		int nPosEnd = 0;
		XEngine_APPHelp_RangeFile(lpszClientAddr, &nPosStart, &nPosEnd, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPUPLOADER);
		RfcComponents_HttpServer_GetRecvModeEx(xhUPHttp, lpszClientAddr, &nRVMode, &nRVCount, &nHDSize);

		_stprintf(tszFileDir, _T("%s%s"), st_ServiceCfg.st_XStorage.tszFileDir, pSt_HTTPParam->tszHttpUri);
		if (!Session_UPStroage_Insert(lpszClientAddr, tszFileDir, nRVCount, nRVCount, nPosStart, nPosEnd))
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
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,用户数据大小为0?"), lpszClientAddr);
		return TRUE;
	}
	Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nMsgLen);
	RfcComponents_HttpServer_GetRecvModeEx(xhUPHttp, lpszClientAddr, &nRVMode, &nRVCount, &nHDSize);
	if (nHDSize >= nRVCount)
	{
		int nHashLen = 0;
		UCHAR tszHashStr[MAX_PATH];
		XSTORAGECORE_DBFILE st_ProtocolFile;

		memset(tszHashStr, '\0', MAX_PATH);
		memset(&st_ProtocolFile, '\0', sizeof(XSTORAGECORE_DBFILE));

		_stprintf(tszFileDir, _T("%s%s"), st_ServiceCfg.st_XStorage.tszFileDir, pSt_HTTPParam->tszHttpUri);
		_tcscpy(st_ProtocolFile.st_ProtocolFile.tszFilePath, st_ServiceCfg.st_XStorage.tszFileDir);
		_tcscpy(st_ProtocolFile.st_ProtocolFile.tszFileName, pSt_HTTPParam->tszHttpUri + 1);
		st_ProtocolFile.st_ProtocolFile.nFileSize = nRVCount;

		OPenSsl_Api_Digest(tszFileDir, tszHashStr, &nHashLen, TRUE, st_ServiceCfg.st_XStorage.nHashMode);
		BaseLib_OperatorString_StrToHex((char*)tszHashStr, nHashLen, st_ProtocolFile.st_ProtocolFile.tszFileHash);
		//验证HASH值
		if (XEngine_APPHelp_VerHash(lpszClientAddr, tszFileDir, st_ProtocolFile.st_ProtocolFile.tszFileHash, pptszListHdr, nHdrCount))
		{
			if (0 != st_ServiceCfg.st_XSql.nSQLType)
			{
				BOOL bRet = FALSE;
				if (1 == st_ServiceCfg.st_XSql.nSQLType)
				{
					bRet = XStorageSQL_File_FileInsert(&st_ProtocolFile);
				}
				else
				{
					bRet = XStorage_SQLite_FileInsert(&st_ProtocolFile);
				}
				if (bRet)
				{
					if (st_ServiceCfg.st_XProxy.st_XProxyPass.bUPPass)
					{
						int nPLen = MAX_PATH;
						int nHttpCode = 0;
						TCHAR tszProxyStr[MAX_PATH];
						SESSION_STORAGEINFO st_StorageInfo;

						memset(tszProxyStr, '\0', MAX_PATH);
						memset(&st_StorageInfo, '\0', sizeof(SESSION_STORAGEINFO));

						Session_UPStroage_GetInfo(lpszClientAddr, &st_StorageInfo);
						XStorageProtocol_Proxy_PacketUPDown(st_StorageInfo.tszFileDir, st_StorageInfo.tszClientAddr, st_StorageInfo.ullRWCount, tszProxyStr, &nPLen, st_ProtocolFile.st_ProtocolFile.tszFileHash);
						APIHelp_HttpRequest_Post(st_ServiceCfg.st_XProxy.st_XProxyPass.tszUPPass, tszProxyStr, &nHttpCode);
						XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_NOTICE, _T("上传客户端:%s,请求完成通知返回值:%d,文件:%s,地址:%s"), lpszClientAddr, nHttpCode, st_StorageInfo.tszFileDir, st_ServiceCfg.st_XProxy.st_XProxyPass.tszUPPass);
					}
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
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("上传客户端:%s,请求上传文件成功,文件名:%s,大小:%d,数据库没有启用,不插入数据库"), lpszClientAddr, tszFileDir, nRVCount);
			}
		}
		Session_UPStroage_Delete(lpszClientAddr);
	}
	else
	{
		_stprintf(tszFileDir, _T("%s%s"), st_ServiceCfg.st_XStorage.tszFileDir, pSt_HTTPParam->tszHttpUri);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求上传文件中,文件名:%s,大小:%d"), lpszClientAddr, tszFileDir, nMsgLen);
	}
	
	return TRUE;
}