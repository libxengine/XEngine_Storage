#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_Center_HTTPThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
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
			//获取当前队列池中所有触发上传客户端
			RfcComponents_HttpServer_GetPoolEx(xhCenterHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					int nMsgLen = 10240;
					int nHdrCount = 0;
					CHAR* ptszMsgBuffer = NULL;
					CHAR** ppszListHdr = NULL;
					//获得指定上传客户端触发信息
					if (RfcComponents_HttpServer_GetMemoryEx(xhCenterHttp, ppSt_PKTClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpCenter(ppSt_PKTClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
					}
					BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&st_HTTPParam);
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

	LPCTSTR lpszMethodPost = _T("POST");
	LPCTSTR lpszMethodGet = _T("GET");
	LPCTSTR lpszMethodOption = _T("OPTIONS");

	if (0 == _tcsnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodPost)))
	{
		if (!XEngine_APPHelp_ProxyAuth(lpszClientAddr, lpszMethodPost, pSt_HTTPParam->tszHttpUri, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPCENTER))
		{
			return FALSE;
		}
		if (st_ServiceCfg.st_XProxy.st_XProxyAuth.bAuth)
		{
			st_HDRParam.bAuth = TRUE;
		}
		//使用重定向?
		if (APIHelp_Distributed_IsMode(st_LoadbalanceCfg.st_LoadBalance.pStl_ListUseMode, STORAGE_NETTYPE_HTTPCENTER))
		{
			TCHAR tszHdrBuffer[1024];
			TCHAR tszStorageAddr[128];

			memset(tszHdrBuffer, '\0', sizeof(tszHdrBuffer));
			memset(tszStorageAddr, '\0', sizeof(tszStorageAddr));

			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 302;

			APIHelp_Distributed_RandomAddr(st_LoadbalanceCfg.st_LoadBalance.pStl_ListCenter, tszStorageAddr);
			_stprintf(tszHdrBuffer, _T("Location: %s%s\r\n"), tszStorageAddr, pSt_HTTPParam->tszHttpUri);

			RfcComponents_HttpServer_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHdrBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求的函数被要求重定向到:%s%s"), lpszClientAddr, tszStorageAddr, pSt_HTTPParam->tszHttpUri);
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
		//通知类型.用于多服务器
		if (0 == _tcsnicmp(XENGINE_STORAGE_APP_TASK_PASS, tszAPIMethod, _tcslen(XENGINE_STORAGE_APP_TASK_PASS)))
		{
			XEngine_Task_Pass(tszAPIName, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount);
		}
		else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_TASK_EVENT, tszAPIMethod, _tcslen(XENGINE_STORAGE_APP_TASK_EVENT)))
		{
			XEngine_Task_Event(tszAPIName, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount);
		}
		else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_TASK_MANAGE, tszAPIMethod, _tcslen(XENGINE_STORAGE_APP_TASK_MANAGE)))
		{
			XEngine_Task_Manage(tszAPIName, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount);
		}
	}
	else if (0 == _tcsnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		if (!XEngine_APPHelp_ProxyAuth(lpszClientAddr, lpszMethodGet, pSt_HTTPParam->tszHttpUri, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPCENTER))
		{
			return FALSE;
		}
		if (st_ServiceCfg.st_XProxy.st_XProxyAuth.bAuth)
		{
			st_HDRParam.bAuth = TRUE;
		}
		XEngine_Task_P2PGet(pSt_HTTPParam->tszHttpUri + 1, lpszClientAddr, pSt_HTTPParam);
	}
	else if (0 == _tcsnicmp(lpszMethodOption, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodOption)))
	{
		//用于心跳
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 200;
		LPCTSTR lpszHdrBuffer = _T("Allow: POST OPTIONS\r\n");
		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, lpszHdrBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求OPTIONS心跳方法成功"), lpszClientAddr);
	}
	else
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 405;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,发送的方法不支持"), lpszClientAddr);
		return FALSE;
	}
	
	return TRUE;
}