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
	LPCXSTR lpszMethodHead = _X("HEAD");

	XCHAR** pptszUrlList;
	XCHAR tszUrlName[128];
	int nUrlCount = 0;
	//得到URL参数个数
	HttpProtocol_ServerHelp_GetParament(pSt_HTTPParam->tszHttpUri, &pptszUrlList, &nUrlCount, tszUrlName);
	if (nUrlCount < 1 && (0 != _tcsxnicmp(lpszMethodHead, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodHead))))
	{
		st_HDRParam.nHttpCode = 400;
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		BaseLib_Memory_Free((XPPPMEM)&pptszUrlList, nUrlCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}

	if (st_ServiceCfg.st_XProxy.bAuthPass)
	{
		if (!StorageApp_HTTPHelp_Verification(lpszClientAddr, pSt_HTTPParam, pptszListHdr, nHdrCount, st_ServiceCfg.st_XProxy.tszAuthPass))
		{
			return false;
		}
	}
	st_HDRParam.bIsClose = true;
	st_HDRParam.nHttpCode = 200;

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
		else
		{
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_APINAME, "unknow api");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求的方法:%s 不存在"), lpszClientAddr, tszStrVlu);
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
		else
		{
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_APINAME, "unknow api");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求的方法:%s 不存在"), lpszClientAddr, tszStrVlu);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodHead, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodHead)))
	{
		//获得文件大小
		XENGINE_STORAGEBUCKET st_StorageBucket = {};
		//分布式存储
		if (!APIHelp_Distributed_DLStorage(pSt_HTTPParam->tszHttpUri, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 404;

			HttpProtocol_Server_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,请求文件失败,可能BUCKET:%s 不正确,错误：%lX"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, StorageHelp_GetLastError());
			return false;
		}
		if (!st_StorageBucket.bEnable)
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 404;

			HttpProtocol_Server_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,请求文件失败,请求的BUCKET:%s 已经被禁用"), lpszClientAddr, st_StorageBucket.tszBuckKey);
			return false;
		}
		XCHAR tszFilePath[XPATH_MAX] = {};
		SYSTEMAPI_FILE_ATTR st_FileAttr = {};

		_xstprintf(tszFilePath, _X("%s%s"), st_StorageBucket.tszFilePath, st_StorageBucket.tszFileName);
		if (0 != _xtaccess(tszFilePath, 0))
		{
			st_HDRParam.nHttpCode = 404;
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_NOTFOUND, "file not found");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,请求文件大小失败,文件:%s 不存在"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		SystemApi_File_GetFileAttr(tszFilePath, &st_FileAttr);
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, st_FileAttr.nFileSize);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求获取文件:%s 大小:%lld 成功"), lpszClientAddr, tszFilePath, st_FileAttr.nFileSize);
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