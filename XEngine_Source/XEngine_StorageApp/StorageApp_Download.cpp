#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_Download_Thread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	TCHAR tszClientAddr[128];
	TCHAR tszMsgBuffer[4096];

	while (bIsRun)
	{
		//等待指定线程事件触发
		RfcComponents_HttpServer_EventWaitEx(xhDLHttp, nThreadPos + 1);

		int nListCount = 0;
		int nMsgLen = 0;
		RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParam;
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;
		
		memset(&st_HTTPParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		//获取当前队列池中所有触发客户端
		RfcComponents_HttpServer_GetPoolEx(xhDLHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			for (int j = 0; i < ppSt_PKTClient[i]->nPktCount; j++)
			{
				//获得指定客户端触发信息
				if (RfcComponents_HttpServer_GetClientEx(xhDLHttp, ppSt_PKTClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_HTTPParam))
				{
					XEngine_Task_Download(ppSt_PKTClient[i]->tszClientAddr, tszClientAddr, nMsgLen, &st_HTTPParam);
				}
			}
		}
	}
	return 0;
}
BOOL XEngine_Task_Download(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam)
{
	XEngine_Net_SendMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,正在发送文件数据,大小:%d"), lpszClientAddr, nMsgLen);
	return TRUE;
}
