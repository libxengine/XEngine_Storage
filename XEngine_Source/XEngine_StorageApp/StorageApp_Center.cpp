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
bool XEngine_Task_HttpCenter(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	XCHAR tszSDBuffer[2048];
	XCHAR tszAPIVersion[64];
	XCHAR tszAPIMethod[64];
	XCHAR tszAPIName[64];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszAPIVersion, '\0', sizeof(tszAPIVersion));
	memset(tszAPIMethod, '\0', sizeof(tszAPIMethod));
	memset(tszAPIName, '\0', sizeof(tszAPIName));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodOption = _X("OPTIONS");

	if (st_ServiceCfg.st_XAuth.bCHAuth)
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
		if (st_ServiceCfg.st_XProxy.bAuthPass)
		{
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
			BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszBody);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,代理服务:%s 验证通过,用户名:%s,密码:%s"), lpszClientAddr, st_ServiceCfg.st_XProxy.tszAuthPass, tszUserName, tszUserPass);
		}
		else
		{
			if (!Session_User_Exist(tszUserName, tszUserPass))
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.bAuth = true;
				st_HDRParam.nHttpCode = 401;

				HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,验证用户失败,无法继续"), lpszClientAddr);
				return false;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,本地验证用户验证通过,用户名:%s,密码:%s"), lpszClientAddr, tszUserName, tszUserPass);
		}
		st_HDRParam.bAuth = true;
	}

	if (0 == _tcsxnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPost)))
	{
		LPCXSTR lpszMethodPass = _X("Pass");
		LPCXSTR lpszMehtodManage = _X("Manage");
		LPCXSTR lpszMehtodAction = _X("Action");

		if (!HttpProtocol_ServerHelp_GetUrlApi(pSt_HTTPParam->tszHttpUri, tszAPIVersion, tszAPIMethod, tszAPIName))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 404;

			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,请求的API不支持"), lpszClientAddr);
			return false;
		}
		//通知类型.用于多服务器
		if (0 == _tcsxnicmp(lpszMethodPass, tszAPIMethod, _tcsxlen(lpszMethodPass)))
		{
			XEngine_Task_Pass(tszAPIName, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount);
		}
		else if (0 == _tcsxnicmp(lpszMehtodManage, tszAPIMethod, _tcsxlen(lpszMehtodManage)))
		{
			XEngine_Task_Manage(tszAPIName, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount);
		}
		else if (0 == _tcsxnicmp(lpszMehtodAction, tszAPIMethod, _tcsxlen(lpszMehtodAction)))
		{
			Storage_TaskAction(tszAPIName, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodOption, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodOption)))
	{
		//用于心跳
		st_HDRParam.bIsClose = true;
		st_HDRParam.nHttpCode = 200;
		LPCXSTR lpszHdrBuffer = _X("Allow: POST GET PUT\r\n");
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, lpszHdrBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求OPTIONS心跳方法成功"), lpszClientAddr);
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