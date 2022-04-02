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

	LPCTSTR lpszMethodPut = _T("PUT");
	LPCTSTR lpszMethodPost = _T("POST");
	if ((0 != _tcsncmp(lpszMethodPut, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodPut))) && (0 != _tcsncmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodPost))))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 405;

		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParam->tszHttpMethod);
		return FALSE;
	}
	//用户验证
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

			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,用户验证失败,错误:%lX"), lpszClientAddr, APIHelp_GetLastError());
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

				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,用户验证失败,用户名:%s,密码:%s,错误码:%d,错误内容:%s"), tszUserName, tszUserPass, tszUserPass, nResponseCode, ptszBody);
			}
			BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszBody);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,代理服务:%s 验证通过,用户名:%s,密码:%s"), lpszClientAddr, st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy, tszUserName, tszUserPass);
		}
		else
		{
			if (!Session_User_Exist(tszUserName, tszUserPass))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.bAuth = TRUE;
				st_HDRParam.nHttpCode = 401;

				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,验证用户失败,无法继续"), lpszClientAddr);
				return FALSE;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,本地验证用户验证通过,用户名:%s,密码:%s"), lpszClientAddr, tszUserName, tszUserPass);
		}
		st_HDRParam.bAuth = TRUE;
	}
	//使用重定向?
	if (st_LoadbalanceCfg.st_LBDistributed.nUPLoadMode > 0)
	{
		TCHAR tszHdrBuffer[1024];
		TCHAR tszStorageAddr[128];

		memset(tszHdrBuffer, '\0', sizeof(tszHdrBuffer));
		memset(tszStorageAddr, '\0', sizeof(tszStorageAddr));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 302;

		APIHelp_Distributed_RandomAddr(st_LoadbalanceCfg.st_LoadBalance.pStl_ListUPLoader, tszStorageAddr, st_LoadbalanceCfg.st_LBDistributed.nUPLoadMode);
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

	if (!APIHelp_Distributed_UPStorage(pSt_HTTPParam->tszHttpUri, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket, st_LoadbalanceCfg.st_LBLocation.nUPLoadMode))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 413;

		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,请求上传文件失败,可能BUCKET:% 不正确,错误：%lX"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, APIHelp_GetLastError());
		return FALSE;
	}
	_stprintf(tszFileDir, _T("%s%s"), st_StorageBucket.tszFilePath, pSt_HTTPParam->tszHttpUri);

	if (!Session_UPStroage_Exist(lpszClientAddr))
	{
		int nPosStart = 0;
		int nPosEnd = 0;
		__int64x nPosCount = 0;
		//得到断点续传大小
		if (!APIHelp_Api_RangeFile(&nPosStart, &nPosEnd, &nPosCount, pptszListHdr, nHdrCount))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 411;
			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,没有用户数据大小字段,无法继续"), lpszClientAddr);
			return FALSE;
		}
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
		if (!Session_UPStroage_Insert(lpszClientAddr, st_StorageBucket.tszBuckKey, tszFileDir, nPosCount, nRVCount, nPosStart, nPosEnd))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 404;

			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,插入用户请求失败,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
			return FALSE;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求设置了数据范围:%d - %d/%lld"), lpszClientAddr, nPosStart, nPosEnd, nPosCount);
	}
	Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nMsgLen);
	RfcComponents_HttpServer_GetRecvModeEx(xhUPHttp, lpszClientAddr, &nRVMode, &nRVCount, &nHDSize);
	if (nHDSize >= nRVCount)
	{
		int nPLen = MAX_PATH;
		TCHAR tszPassNotify[MAX_PATH];
		SESSION_STORAGEINFO st_StorageInfo;

		memset(tszPassNotify, '\0', MAX_PATH);
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
		st_ProtocolFile.st_ProtocolFile.nFileSize = st_StorageInfo.ullFSize;

		OPenSsl_Api_Digest(tszFileDir, tszHashStr, &nHashLen, TRUE, st_ServiceCfg.st_XStorage.nHashMode);
		BaseLib_OperatorString_StrToHex((char*)tszHashStr, nHashLen, st_ProtocolFile.st_ProtocolFile.tszFileHash);
		//处理结果
		if (st_ServiceCfg.st_XStorage.bUPHash)
		{
			if (APIHelp_Api_VerHash(st_ProtocolFile.st_ProtocolFile.tszFileHash, pptszListHdr, nHdrCount))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,上传的文件信息HASH效验成功,文件:%s HASH:%s"), lpszClientAddr, tszFileDir, st_ProtocolFile.st_ProtocolFile.tszFileHash);
			}
			else
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 403;
				_tremove(tszFileDir);
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,上传的文件信息HASH校验失败,无法继续,文件:%s 已被删除,HASH:%s,错误:%lX"), lpszClientAddr, tszFileDir, st_ProtocolFile.st_ProtocolFile.tszFileHash, APIHelp_GetLastError());
				return FALSE;
			}
		}
		
		BOOL bRet = TRUE;
		Protocol_StoragePacket_UPDown(tszPassNotify, &nPLen, st_StorageInfo.tszBuckKey, st_StorageInfo.tszFileDir, st_StorageInfo.tszClientAddr, st_StorageInfo.ullRWCount, FALSE, st_ProtocolFile.st_ProtocolFile.tszFileHash);
		if (st_ServiceCfg.st_XSql.bEnable)
		{
			_tcscpy(st_ProtocolFile.tszBuckKey, st_StorageBucket.tszBuckKey);
			if (Database_File_FileInsert(&st_ProtocolFile))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 200;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPassNotify, nPLen);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求上传文件成功,文件名:%s,大小:%d"), lpszClientAddr, tszFileDir, nRVCount);
			}
			else
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 403;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPassNotify, nPLen);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求上传文件失败,插入数据库失败:%s,错误:%lX"), lpszClientAddr, tszFileDir, Database_GetLastError());
			}
		}
		else
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 200;
			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPassNotify, nPLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("上传客户端:%s,请求上传文件成功,文件名:%s,大小:%d,数据库没有启用,不插入数据库"), lpszClientAddr, tszFileDir, nRVCount);
		}
		//PASS代理
		if (st_ServiceCfg.st_XProxy.st_XProxyPass.bUPPass && bRet)
		{
			int nHttpCode = 0;
			if (APIHelp_HttpRequest_Post(st_ServiceCfg.st_XProxy.st_XProxyPass.tszUPPass, tszPassNotify, &nHttpCode))
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