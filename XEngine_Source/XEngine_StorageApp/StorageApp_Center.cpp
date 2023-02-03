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
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			RfcComponents_HttpServer_GetPoolEx(xhCenterHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
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
					if (RfcComponents_HttpServer_GetMemoryEx(xhCenterHttp, ppSt_PKTClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpCenter(ppSt_PKTClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
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
	LPCTSTR lpszMethodOption = _T("OPTIONS");

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

			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,用户验证失败,错误:%lX"), lpszClientAddr, APIHelp_GetLastError());
			return FALSE;
		}
		if (_tcslen(st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy) > 0)
		{
			int nBLen = 0;
			int nResponseCode = 0;
			TCHAR* ptszBody = NULL;

			Protocol_StoragePacket_BasicAuth(pSt_HTTPParam->tszHttpMethod, pSt_HTTPParam->tszHttpUri, lpszClientAddr, tszUserName, tszUserPass, tszSDBuffer, &nSDLen);
			APIClient_Http_Request(_T("POST"), st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy, tszSDBuffer, &nResponseCode, &ptszBody, &nBLen);
			if (200 != nResponseCode)
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.bAuth = TRUE;
				st_HDRParam.nHttpCode = nResponseCode;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,用户验证失败,用户名:%s,密码:%s,错误码:%d,错误内容:%s"), tszUserName, tszUserPass, tszUserPass, nResponseCode, ptszBody);
			}
			BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszBody);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,代理服务:%s 验证通过,用户名:%s,密码:%s"), lpszClientAddr, st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy, tszUserName, tszUserPass);
		}
		else
		{
			if (!Session_User_Exist(tszUserName, tszUserPass))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.bAuth = TRUE;
				st_HDRParam.nHttpCode = 401;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,验证用户失败,无法继续"), lpszClientAddr);
				return FALSE;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,本地验证用户验证通过,用户名:%s,密码:%s"), lpszClientAddr, tszUserName, tszUserPass);
		}
		st_HDRParam.bAuth = TRUE;
	}

	if (0 == _tcsnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodPost)))
	{
		//使用重定向?
		if (st_LoadbalanceCfg.st_LBDistributed.nCenterMode > 0)
		{
			TCHAR tszHdrBuffer[1024];
			TCHAR tszStorageAddr[128];

			memset(tszHdrBuffer, '\0', sizeof(tszHdrBuffer));
			memset(tszStorageAddr, '\0', sizeof(tszStorageAddr));

			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 302;

			APIHelp_Distributed_RandomAddr(st_LoadbalanceCfg.st_LoadBalance.pStl_ListCenter, tszStorageAddr, st_LoadbalanceCfg.st_LBDistributed.nCenterMode);
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
		else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_TASK_MANAGE, tszAPIMethod, _tcslen(XENGINE_STORAGE_APP_TASK_MANAGE)))
		{
			XEngine_Task_Manage(tszAPIName, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount);
		}
	}
	else if (0 == _tcsnicmp(lpszMethodOption, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodOption)))
	{
		//用于心跳
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 200;
		LPCTSTR lpszHdrBuffer = _T("Allow: POST GET PUT OPTIONS\r\n");
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