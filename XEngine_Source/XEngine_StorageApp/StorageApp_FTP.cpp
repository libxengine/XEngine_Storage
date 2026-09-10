#include "StorageApp_Hdr.h"

XHTHREAD XCALLBACK XEngine_FTP_Thread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (FTPProtocol_Parse_EventWaitEx(xhFTPPacket, nThreadPos))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			FTPProtocol_Parse_GetPoolEx(xhFTPPacket, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					XENGINE_KEYVALUE st_KeyValue = {};
					//获得指定上传客户端触发信息
					if (FTPProtocol_Parse_GetClientEx(xhFTPPacket, ppSt_PKTClient[i]->tszClientAddr, &st_KeyValue))
					{
						XEngine_Task_FTP(ppSt_PKTClient[i]->tszClientAddr, &st_KeyValue);
					}
				}
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}

bool XEngine_Task_FTP(LPCXSTR lpszClientAddr, XENGINE_KEYVALUE *pSt_KeyValue)
{
	int nSDLen = 8192;
	int nRVLen = 8192;
	XCHAR tszSDBuffer[8192] = {};
	XCHAR tszRVBuffer[8192] = {};

	if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_USER, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_USER)))
	{
		//用于心跳
		LPCXSTR lpszHdrBuffer = _X("Allow: OPTIONS POST GET PUT PROPFIND PROPPATCH DELETE LOCK UNLOCK MKCOL\r\n");
		FTPProtocol_Packet_Request(tszSDBuffer, &nSDLen, pSt_KeyValue);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_FTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,请求OPTIONS心跳方法成功"), lpszClientAddr);
	}
	else
	{
		FTPProtocol_Packet_Request(tszSDBuffer, &nSDLen, pSt_KeyValue);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_FTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,发送的方法不支持"), lpszClientAddr);
		return false;
	}
	
	return true;
}