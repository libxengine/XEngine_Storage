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
					//获得指定上传客户端触发信息
					if (RfcComponents_HttpServer_GetClientEx(xhCenterHttp, ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_HTTPParam))
					{
						XEngine_Task_HttpCenter(ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen, &st_HTTPParam);
					}
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}
//POST /api/query/file
BOOL XEngine_Task_HttpCenter_APIList(LPCTSTR lpszUrlName, TCHAR* ptszAPIVersion, TCHAR* ptszAPIMethod, TCHAR* ptszAPIName)
{
	LPCTSTR lpszTokChar = _T("/");
	TCHAR tszHTTPUrl[MAX_PATH];
	memset(tszHTTPUrl, '\0', MAX_PATH);

	_tcscpy(tszHTTPUrl, lpszUrlName);
	TCHAR* ptszTokStr = _tcstok(tszHTTPUrl, lpszTokChar);
	if (NULL == ptszTokStr)
	{
		return FALSE;
	}
	_tcscpy(ptszAPIVersion, ptszTokStr);
	ptszTokStr = _tcstok(NULL, lpszTokChar);
	if (NULL == ptszTokStr)
	{
		return FALSE;
	}
	_tcscpy(ptszAPIMethod, ptszTokStr);
	ptszTokStr = _tcstok(NULL, lpszTokChar);
	if (NULL == ptszTokStr)
	{
		return FALSE;
	}
	_tcscpy(ptszAPIName, ptszTokStr);
	return TRUE;
}
BOOL XEngine_Task_HttpCenter(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam)
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
	if (!XEngine_Task_HttpCenter_APIList(pSt_HTTPParam->tszHttpUri, tszAPIVersion, tszAPIMethod, tszAPIName))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 404;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求的API不支持"), lpszClientAddr);
		return FALSE;
	}

	LPCTSTR lpszQuery = _T("query");
	LPCTSTR lpszQueryFile = _T("file");
	if (0 == _tcsncmp(lpszQuery, tszAPIMethod, _tcslen(lpszQuery)))
	{
		if (0 == _tcsncmp(lpszQueryFile, tszAPIName, _tcslen(lpszQueryFile)))
		{
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
	return TRUE;
}