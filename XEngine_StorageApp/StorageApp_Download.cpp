#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_Download_Thread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	TCHAR tszClientAddr[128];
	TCHAR tszMsgBuffer[4096];

	while (bIsRun)
	{
		int nListCount = 0;
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_PKTClient;

		RfcComponents_HttpServer_EventWaitEx(xhDLHttp, nThreadPos);

		RfcComponents_HttpServer_GetPoolEx(xhDLHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			for (int j = 0; i < ppSt_PKTClient[i]->nPktCount; j++)
			{
			}
		}
	}
	return 0;
}
BOOL XEngine_Task_Download(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	XEngine_Net_SendMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,正在发送文件数据,大小:%d"), lpszClientAddr, nMsgLen);
	return TRUE;
}
