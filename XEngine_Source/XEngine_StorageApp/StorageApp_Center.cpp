#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_Center_HTTPThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	TCHAR tszMsgBuffer[10240];
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (RfcComponents_HttpServer_EventWaitEx(xhCenterHttp, nThreadPos))
		{
			int nListCount = 0;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParam;
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;

			memset(&st_HTTPParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			//获取当前队列池中所有触发上传客户端
			RfcComponents_HttpServer_GetPoolEx(xhCenterHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					int nMsgLen = 10240;
					int nHdrCount = 0;
					CHAR** ppszListHdr = NULL;
					//获得指定上传客户端触发信息
					if (RfcComponents_HttpServer_GetClientEx(xhCenterHttp, ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpCenter(ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
					}
					BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListHdr, nHdrCount);
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}
//POST /api/query/file
BOOL XEngine_Task_HttpCenter(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	TCHAR tszAPIVersion[64];
	TCHAR tszAPIMethod[64];
	TCHAR tszAPIName[64];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszAPIVersion, '\0', sizeof(tszAPIVersion));
	memset(tszAPIMethod, '\0', sizeof(tszAPIMethod));
	memset(tszAPIName, '\0', sizeof(tszAPIName));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	LPCTSTR lpszMethodName = _T("POST");
	if (0 != _tcsncmp(lpszMethodName, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodName)))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 405;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,发送的方法不支持"), lpszClientAddr);
		return FALSE;
	}
	if (!XEngine_Task_ProxyAuth(lpszClientAddr, pSt_HTTPParam->tszHttpUri, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPCENTER))
	{
		return FALSE;
	}
	if (st_ServiceCfg.st_XProxy.st_XProxyAuth.bAuth)
	{
		st_HDRParam.bAuth = TRUE;
	}
	//使用重定向?
	if ((3 == st_ServiceCfg.st_XStorage.nUseMode) || (4 == st_ServiceCfg.st_XStorage.nUseMode))
	{
		TCHAR tszHdrBuffer[1024];
		memset(tszHdrBuffer, '\0', sizeof(tszHdrBuffer));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 302;

		_stprintf(tszHdrBuffer, _T("Location: %s%s\r\n"), st_ServiceCfg.st_XStorage.tszHttpAddr, pSt_HTTPParam->tszHttpUri);

		RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHdrBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求的函数被要求重定向到:%s%s"), lpszClientAddr, st_ServiceCfg.st_XStorage.tszHttpAddr, pSt_HTTPParam->tszHttpUri);
		return TRUE;
	}

	if (!RfcComponents_HttpHelp_GetUrlApi(pSt_HTTPParam->tszHttpUri, tszAPIVersion, tszAPIMethod, tszAPIName))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 404;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求的API不支持"), lpszClientAddr);
		return FALSE;
	}

	LPCTSTR lpszEvent = _T("Event");
	LPCTSTR lpszQuery = _T("Query");
	LPCTSTR lpszNotify = _T("Notify");

	LPCTSTR lpszUPFile = _T("UPFile");
	LPCTSTR lpszDLFile = _T("DLFile");
	LPCTSTR lpszFile = _T("File");

	//通知类型.用于多服务器
	if (0 == _tcsnicmp(lpszNotify, tszAPIMethod, _tcslen(lpszNotify)))
	{
		//下载
		if (0 == _tcsnicmp(lpszDLFile, tszAPIName, _tcslen(lpszDLFile)))
		{
			__int64x nFileSize = 0;
			TCHAR tszFileName[MAX_PATH];
			TCHAR tszFileHash[MAX_PATH];
			TCHAR tszClientAddr[128];

			memset(tszFileName, '\0', MAX_PATH);
			memset(tszFileHash, '\0', MAX_PATH);
			memset(tszClientAddr, '\0', sizeof(tszClientAddr));

			XStorageProtocol_Proxy_ParseNotify(lpszMsgBuffer, nMsgLen, tszClientAddr, tszFileName, tszFileHash, &nFileSize);

			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 200;

			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求的下载文件通知协议成功,文件:%s,大小:%lld"), lpszClientAddr, tszFileName, nFileSize);
		}
	}
	else if (0 == _tcsnicmp(lpszQuery, tszAPIMethod, _tcslen(lpszQuery)))
	{
		//文件查询类型
		if (0 == _tcsnicmp(lpszFile, tszAPIName, _tcslen(lpszFile)))
		{
			//查询文件列表
			int nMsgLen = 10240;
			TCHAR tszFileName[MAX_PATH];
			TCHAR tszFileHash[MAX_PATH];
			TCHAR tszTimeStart[128];
			TCHAR tszTimeEnd[128];
			TCHAR tszMsgBuffer[10240];

			memset(tszFileName, '\0', MAX_PATH);
			memset(tszFileHash, '\0', MAX_PATH);
			memset(tszTimeStart, '\0', sizeof(tszTimeStart));
			memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			int nListCount = 0;
			XSTORAGECORE_DBFILE** ppSt_ListFile;
			XStorageProtocol_Core_REQQueryFile(lpszMsgBuffer, tszTimeStart, tszTimeEnd, tszFileHash);
			XStorageSQL_File_FileQuery(&ppSt_ListFile, &nListCount, tszTimeStart, tszTimeEnd, tszFileName, tszFileHash);

			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 200;
			
			XStorageProtocol_Core_REPQueryFile(tszMsgBuffer, &nMsgLen, &ppSt_ListFile, nListCount, tszTimeStart, tszTimeEnd);
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszMsgBuffer, nMsgLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求查询文件列表成功,列表个数:%d"), lpszClientAddr, nListCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListFile, nListCount);
		}
	}
	else if (0 == _tcsnicmp(lpszEvent, tszAPIMethod, _tcslen(lpszEvent)))
	{
		//事件处理类型,上传事件,用于NGINX
		if (0 == _tcsnicmp(lpszUPFile, tszAPIName, _tcslen(lpszUPFile)))
		{
			LPCTSTR lpszContentType = _T("Content-Type");
			LPCTSTR lpszBoundaryStr = _T("boundary=");
			TCHAR tszContentStr[MAX_PATH];
			TCHAR tszBoundarTmp[MAX_PATH];
			TCHAR tszFileDir[MAX_PATH];
			TCHAR tszBoundarStr[512];
			XSTORAGECORE_DBFILE st_DBFile;

			memset(tszContentStr, '\0', MAX_PATH);
			memset(tszBoundarTmp, '\0', MAX_PATH);
			memset(tszContentStr, '\0', MAX_PATH);
			memset(tszFileDir, '\0', MAX_PATH);
			memset(tszBoundarStr, '\0', sizeof(tszBoundarStr));
			memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

			if (!RfcComponents_HttpHelp_GetField(&pptszListHdr, nHdrCount, lpszContentType, tszContentStr))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 400;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求的HTTP头内容不正确,没有类型字段"), lpszClientAddr);
				return FALSE;
			}
			if (!BaseLib_OperatorString_FromStrGetKeyValue(tszContentStr, lpszBoundaryStr, NULL, tszBoundarTmp))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 400;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求的HTTP头内容不正确,没有Boundary值"), lpszClientAddr);
				return FALSE;
			}
			_stprintf(tszBoundarStr, _T("--%s\r\n"), tszBoundarTmp);
			XStorageProtocol_Core_REQUPEvent(lpszMsgBuffer, tszBoundarStr, st_DBFile.st_ProtocolFile.tszFileName, tszFileDir, st_DBFile.st_ProtocolFile.tszFileHash, &st_DBFile.st_ProtocolFile.nFileSize);
			
			if (st_ServiceCfg.st_XStorage.bRename)
			{
				memset(st_DBFile.st_ProtocolFile.tszFilePath, '\0', MAX_PATH);
				_tcscpy(st_DBFile.st_ProtocolFile.tszFilePath, st_ServiceCfg.st_XStorage.tszFileDir);
			}
			else
			{
				memset(st_DBFile.st_ProtocolFile.tszFileName, '\0', MAX_PATH);
				BaseLib_OperatorString_GetFileAndPath(tszFileDir, st_DBFile.st_ProtocolFile.tszFilePath, st_DBFile.st_ProtocolFile.tszFileName);
				st_DBFile.st_ProtocolFile.tszFilePath[_tcslen(st_DBFile.st_ProtocolFile.tszFilePath) - 1] = '\0';
			}
			if (XStorageSQL_File_FileInsert(&st_DBFile))
			{
				st_HDRParam.nHttpCode = 200;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,处理NGINX代理上传文件成功,文件名:%s,大小:%lld"), lpszClientAddr, tszFileDir, st_DBFile.st_ProtocolFile.nFileSize);
			}
			else
			{
				st_HDRParam.nHttpCode = 403;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,处理NGINX代理上传文件失败,插入数据库失败:%s,错误:%lX"), lpszClientAddr, tszFileDir, XStorageDB_GetLastError());
			}
			if (st_ServiceCfg.st_XStorage.bRename)
			{
				TCHAR tszFileTmp[1024];
				memset(tszFileTmp, '\0', sizeof(tszFileTmp));

				_stprintf(tszFileTmp, _T("%s/%s"), st_DBFile.st_ProtocolFile.tszFilePath, st_DBFile.st_ProtocolFile.tszFileName);
				_trename(tszFileDir, tszFileTmp);
			}
		}
	}
	return TRUE;
}