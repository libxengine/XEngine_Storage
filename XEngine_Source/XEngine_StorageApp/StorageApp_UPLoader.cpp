#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_UPLoader_HTTPThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	TCHAR tszClientAddr[128];
	TCHAR tszMsgBuffer[4096];

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (RfcComponents_HttpServer_EventWaitEx(xhUPHttp, nThreadPos + 1))
		{
			int nListCount = 0;
			int nMsgLen = 0;
			RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParam;
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;

			memset(&st_HTTPParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
			memset(tszClientAddr, '\0', sizeof(tszClientAddr));
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			//获取当前队列池中所有触发上传客户端
			RfcComponents_HttpServer_GetPoolEx(xhUPHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; i < ppSt_PKTClient[i]->nPktCount; j++)
				{
					//获得指定上传客户端触发信息
					if (RfcComponents_HttpServer_GetClientEx(xhUPHttp, ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_HTTPParam))
					{
						XEngine_Task_HttpUPLoader(ppSt_PKTClient[i]->tszClientAddr, tszClientAddr, nMsgLen, &st_HTTPParam);
					}
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}
BOOL XEngine_Task_HttpUPLoader(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam)
{
	int nSDLen = 2048;
	__int64x ullSize = 0;
	TCHAR tszSDBuffer[2048];
	TCHAR tszFileDir[512];
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
	int nRVCount = 0;
	int nHDSize = 0;
	if (!Session_UPStroage_Exist(lpszClientAddr))
	{
		RfcComponents_HttpServer_GetRecvModeEx(xhUPHttp, lpszClientAddr, NULL, &nRVCount, &nHDSize);

		_stprintf(tszFileDir, _T("%s%s"), st_ServiceCfg.st_XStorage.tszFileDir, pSt_HTTPParam->tszHttpUri);
		if (!Session_UPStroage_Insert(lpszClientAddr, tszFileDir, nRVCount))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 404;

			RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("上传客户端:%s,插入用户请求失败,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
			return FALSE;
		}
	}
	Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nMsgLen);
	RfcComponents_HttpServer_GetRecvModeEx(xhUPHttp, lpszClientAddr, NULL, &nRVCount, &nHDSize);
	if (nHDSize >= nRVCount)
	{
		st_HDRParam.nHttpCode = 200;
		_stprintf(tszFileDir, _T("%s%s"), st_ServiceCfg.st_XStorage.tszFileDir, pSt_HTTPParam->tszHttpUri);

		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("上传客户端:%s,请求上传文件成功,文件名:%s,大小:%llu"), lpszClientAddr, tszFileDir, ullSize);
	}
	else
	{

	}
	
	return TRUE;
}