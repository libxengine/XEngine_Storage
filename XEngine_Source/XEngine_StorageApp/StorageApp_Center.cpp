#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_Center_HTTPThread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (HttpProtocol_Server_EventWaitEx(xhCenterHttp, nThreadPos))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			HttpProtocol_Server_GetPoolEx(xhCenterHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					int nMsgLen = 0;
					int nHdrCount = 0;
					XCHAR* ptszMsgBuffer = NULL;
					XCHAR** ppszListHdr = NULL;
					RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParam;

					memset(&st_HTTPParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
					//获得指定上传客户端触发信息
					if (HttpProtocol_Server_GetMemoryEx(xhCenterHttp, ppSt_PKTClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpCenter(ppSt_PKTClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
					}
					BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
					BaseLib_Memory_Free((XPPPMEM)&ppszListHdr, nHdrCount);
				}
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}
//POST /api/query/file
bool XEngine_Task_HttpCenter(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	int nRVLen = 2048;
	XCHAR tszSDBuffer[2048] = {};
	XCHAR tszRVBuffer[2048] = {};
	XCHAR tszStrKey[MAX_PATH] = {};
	XCHAR tszStrVlu[MAX_PATH] = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodGet = _X("GET");

	XCHAR** pptszUrlList;
	XCHAR tszUrlName[128];
	int nUrlCount = 0;
	//得到URL参数个数
	HttpProtocol_ServerHelp_GetParament(pSt_HTTPParam->tszHttpUri, &pptszUrlList, &nUrlCount, tszUrlName);
	if (nUrlCount < 1)
	{
		st_HDRParam.nHttpCode = 404;
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen);
		BaseLib_Memory_Free((XPPPMEM)&pptszUrlList, nUrlCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	st_HDRParam.bIsClose = true;
	st_HDRParam.nHttpCode = 200;
	if (st_ServiceCfg.st_XProxy.bAuthPass)
	{
		XCHAR tszUserName[64];
		XCHAR tszUserPass[64];

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(tszUserPass, '\0', sizeof(tszUserPass));
		if (!APIHelp_Api_ProxyAuth(tszUserName, tszUserPass, pptszListHdr, nHdrCount))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.bAuth = true;
			st_HDRParam.nHttpCode = 401;

			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,用户验证失败,错误:%lX"), lpszClientAddr, StorageHelp_GetLastError());
			return false;
		}
		int nResponseCode = 0;
		XCHAR* ptszBody = NULL;

		Protocol_StoragePacket_BasicAuth(pSt_HTTPParam->tszHttpMethod, pSt_HTTPParam->tszHttpUri, lpszClientAddr, tszUserName, tszUserPass, tszSDBuffer, &nSDLen);
		APIClient_Http_Request(_X("POST"), st_ServiceCfg.st_XProxy.tszAuthPass, tszSDBuffer, &nResponseCode, &ptszBody, &nSDLen);
		if (200 != nResponseCode)
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.bAuth = true;
			st_HDRParam.nHttpCode = nResponseCode;

			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,用户验证失败,用户名:%s,密码:%s,错误码:%d,错误内容:%s"), tszUserName, tszUserPass, tszUserPass, nResponseCode, ptszBody);
		}
		BaseLib_Memory_FreeCStyle((VOID**)&ptszBody);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,代理服务:%s 验证通过,用户名:%s,密码:%s"), lpszClientAddr, st_ServiceCfg.st_XProxy.tszAuthPass, tszUserName, tszUserPass);
		st_HDRParam.bAuth = true;
	}

	if (0 == _tcsxnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPost)))
	{
		LPCXSTR lpszAPIPass = _X("Pass");
		LPCXSTR lpszAPIManage = _X("Manage");
		LPCXSTR lpszAPIAction = _X("Action");

		BaseLib_String_GetKeyValueA(pptszUrlList[0], "=", tszStrKey, tszStrVlu);
		//通知类型.用于多服务器
		if (0 == _tcsxnicmp(lpszAPIPass, tszStrVlu, _tcsxlen(lpszAPIPass)))
		{
			//http://127.0.0.1:5100/api?function=pass&param=xxx
			memset(tszStrVlu, '\0', sizeof(tszStrVlu));
			BaseLib_String_GetKeyValueA(pptszUrlList[1], "=", tszStrKey, tszStrVlu);
			XEngine_Task_Pass(tszStrVlu, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount);
		}
		else if (0 == _tcsxnicmp(lpszAPIManage, tszStrVlu, _tcsxlen(lpszAPIManage)))
		{
			//http://127.0.0.1:5100/api?function=pass&param=xxx
			memset(tszStrVlu, '\0', sizeof(tszStrVlu));
			BaseLib_String_GetKeyValueA(pptszUrlList[1], "=", tszStrKey, tszStrVlu);
			XEngine_Task_Manage(tszStrVlu, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount);
		}
		else if (0 == _tcsxnicmp(lpszAPIAction, tszStrVlu, _tcsxlen(lpszAPIAction)))
		{
			//http://127.0.0.1:5100/api?function=pass&param=xxx
			memset(tszStrVlu, '\0', sizeof(tszStrVlu));
			BaseLib_String_GetKeyValueA(pptszUrlList[1], "=", tszStrKey, tszStrVlu);
			Storage_TaskAction(tszStrVlu, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		//用于心跳
		//http://127.0.0.1:5100/api?function=heart
		LPCXSTR lpszAPIHeart = _X("heart");

		BaseLib_String_GetKeyValueA(pptszUrlList[0], "=", tszStrKey, tszStrVlu);
		if (0 == _tcsxnicmp(lpszAPIHeart, tszStrVlu, _tcsxlen(lpszAPIHeart)))
		{
			LPCXSTR lpszHdrBuffer = _X("Allow: POST GET PUT\r\n");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, lpszHdrBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求GET心跳方法成功"), lpszClientAddr);
		}
	}
	else
	{
		st_HDRParam.bIsClose = true;
		st_HDRParam.nHttpCode = 405;

		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,发送的方法不支持"), lpszClientAddr);
		return false;
	}
	
	return true;
}