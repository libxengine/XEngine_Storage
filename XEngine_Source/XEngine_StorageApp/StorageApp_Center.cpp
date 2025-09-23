#include "StorageApp_Hdr.h"

XHTHREAD XCALLBACK XEngine_Center_HTTPThread(XPVOID lParam)
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
	XCHAR tszStrKey[XPATH_MAX] = {};
	XCHAR tszStrVlu[XPATH_MAX] = {};
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
		st_HDRParam.nHttpCode = 400;
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
		int nVType = 0;
		RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

		st_HDRParam.nHttpCode = 401;
		st_HDRParam.bIsClose = true;
		st_HDRParam.bAuth = true;
		//打包验证信息
		int nHDRLen = 0;
		XCHAR tszHDRBuffer[XPATH_MAX] = {};
		if (1 == st_ServiceCfg.st_XProxy.nVType)
		{
			Verification_HTTP_BasicServerPacket(tszHDRBuffer, &nHDRLen);
		}
		else
		{
			XCHAR tszNonceStr[64] = {};
			XCHAR tszOpaqueStr[64] = {};
			Verification_HTTP_DigestServerPacket(tszHDRBuffer, &nHDRLen, tszNonceStr, tszOpaqueStr);
		}
		//后去验证方法
		if (!Verification_HTTP_GetType(pptszListHdr, nHdrCount, &nVType))
		{
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式:%d,错误:%lX"), lpszClientAddr, st_ServiceCfg.st_XProxy.nVType, Verification_GetLastError());
			return false;
		}
		//验证方式是否一致
		if (st_ServiceCfg.st_XProxy.nVType != nVType)
		{
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式错误,请求:%d,需求:%d"), lpszClientAddr, nVType, st_ServiceCfg.st_XProxy.nVType);
			return false;
		}
		bool bRet = false;
		int nHTTPCode = 0;
		int nMSGLen = 0;
		XCLIENT_APIHTTP st_APIHttp = {};

		XCHAR* ptszMSGBuffer = NULL;
		if (!APIClient_Http_Request(_X("GET"), st_ServiceCfg.st_XProxy.tszAuthPass, NULL, &nHTTPCode, &ptszMSGBuffer, &nMSGLen, NULL, NULL, &st_APIHttp))
		{
			st_HDRParam.nHttpCode = 500;
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_SERVERDOWN, "api server is down,cant verification");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误码:%lX"), lpszClientAddr, st_ServiceCfg.st_XProxy.tszAuthPass, APIClient_GetLastError());
			return false;
		}
		if (200 != nHTTPCode)
		{
			st_HDRParam.nHttpCode = 500;
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_SERVERDOWN, "api server is down,cant verification");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误:%d"), lpszClientAddr, st_ServiceCfg.st_XProxy.tszAuthPass, nHTTPCode);
			return false;
		}
		XENGINE_PROTOCOL_USERAUTH st_UserAuth = {};
		if (!Protocol_StorageParse_User(ptszMSGBuffer, nMsgLen, &st_UserAuth))
		{
			st_HDRParam.nHttpCode = 500;
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_AUTHFAIL, "api server reply failure,cant verification");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,返回内容:%s 错误,无法继续"), lpszClientAddr, ptszMSGBuffer);
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);
			return false;
		}
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);

		if (1 == nVType)
		{
			bRet = Verification_HTTP_Basic(st_UserAuth.tszUserName, st_UserAuth.tszUserPass, pptszListHdr, nHdrCount);
		}
		else if (2 == nVType)
		{
			bRet = Verification_HTTP_Digest(st_UserAuth.tszUserName, st_UserAuth.tszUserPass, pSt_HTTPParam->tszHttpMethod, pptszListHdr, nHdrCount);
		}

		if (!bRet)
		{
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证处理错误,可能用户密码登信息不匹配,类型:%d"), lpszClientAddr, nVType);
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,HTTP验证类型:%d 通过"), lpszClientAddr, nVType);
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
			XEngine_Task_Pass(tszStrVlu, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam);
		}
		else if (0 == _tcsxnicmp(lpszAPIManage, tszStrVlu, _tcsxlen(lpszAPIManage)))
		{
			//http://127.0.0.1:5100/api?function=manage&param=xxx
			memset(tszStrVlu, '\0', sizeof(tszStrVlu));
			BaseLib_String_GetKeyValueA(pptszUrlList[1], "=", tszStrKey, tszStrVlu);
			XEngine_Task_Manage(tszStrVlu, lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam);
		}
		else if (0 == _tcsxnicmp(lpszAPIAction, tszStrVlu, _tcsxlen(lpszAPIAction)))
		{
			//http://127.0.0.1:5100/api?function=action&param=xxx
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