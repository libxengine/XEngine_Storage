#include "StorageApp_Hdr.h"

XHTHREAD XCALLBACK XEngine_FTPContral_Thread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (FTPProtocol_Parse_EventWaitEx(xhFTPContral, nThreadPos))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			FTPProtocol_Parse_GetPoolEx(xhFTPContral, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					XENGINE_KEYVALUE st_KeyValue = {};
					//获得指定上传客户端触发信息
					if (FTPProtocol_Parse_GetClientEx(xhFTPContral, ppSt_PKTClient[i]->tszClientAddr, &st_KeyValue))
					{
						XEngine_Task_FTP(ppSt_PKTClient[i]->tszClientAddr, &st_KeyValue, STORAGE_NETTYPE_FTPCONTRAL);
					}
				}
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}
XHTHREAD XCALLBACK XEngine_FTPDatas_Thread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (FTPProtocol_Parse_EventWaitEx(xhFTPDatas, nThreadPos))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			FTPProtocol_Parse_GetPoolEx(xhFTPDatas, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					XENGINE_KEYVALUE st_KeyValue = {};
					//获得指定上传客户端触发信息
					if (FTPProtocol_Parse_GetClientEx(xhFTPDatas, ppSt_PKTClient[i]->tszClientAddr, &st_KeyValue))
					{
						XEngine_Task_FTP(ppSt_PKTClient[i]->tszClientAddr, &st_KeyValue, STORAGE_NETTYPE_FTPDATAS);
					}
				}
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}

bool XEngine_Task_FTP(LPCXSTR lpszClientAddr, XENGINE_KEYVALUE *pSt_KeyValue, int nNetType)
{
	int nSDLen = 8192;
	int nRVLen = 8192;
	XCHAR tszSDBuffer[8192] = {};
	XCHAR tszRVBuffer[8192] = {};

	if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_USER, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_USER)))
	{
		//用户请求
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_331, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求用户名成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_PASS, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_PASS)))
	{
		//密码请求
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_230, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求密码成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_TYPE, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_TYPE)))
	{
		//类型请求
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_200, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求类型成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_PASV, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_PASV)))
	{
		//被动模式请求
		int nPort = 0;
		XCHAR tszIPAddr[XPATH_MIN] = {};
		APIADDR_IPADDR st_IPAddr = {};

		APIAddr_IPAddr_IsIPV4Addr(st_ServiceCfg.tszIPAddr, &st_IPAddr);
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_227, tszRVBuffer, &nRVLen, false);

		int p1 = st_ServiceCfg.nFTPDPort / 256;
		int p2 = st_ServiceCfg.nFTPDPort % 256;
		_xstprintf(tszSDBuffer, _X("%d %s (%d,%d,%d,%d,%d,%d)"), XENGINE_FTPROTOCOL_RESPONSE_227, tszRVBuffer, st_IPAddr.nIPAddr1, st_IPAddr.nIPAddr2, st_IPAddr.nIPAddr3, st_IPAddr.nIPAddr4, p1, p2);
		
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求被动模式成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else
	{
		FTPProtocol_Packet_Request(tszSDBuffer, &nSDLen, pSt_KeyValue);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,发送的方法 %s:%s 不支持"), lpszClientAddr, pSt_KeyValue->tszStrKey, pSt_KeyValue->tszStrVlu);
		return false;
	}
	
	return true;
}