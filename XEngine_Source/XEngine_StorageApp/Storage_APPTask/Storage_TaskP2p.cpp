#include "../StorageApp_Hdr.h"

XHTHREAD XEngine_Task_P2PThread()
{
	TCHAR tszMsgBuffer[2048];
	TCHAR tszIPAddr[128];
	TCHAR tszTimeStart[64];
	TCHAR tszTimeEnd[64];
	TCHAR tszFileName[MAX_PATH];
	TCHAR tszFileHash[MAX_PATH];

	while (bIsRun)
	{
		int nMsgLen = 2048;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(tszIPAddr, '\0', sizeof(tszIPAddr));
		memset(tszTimeStart, '\0', sizeof(tszTimeStart));
		memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
		memset(tszFileName, '\0', MAX_PATH);
		memset(tszFileHash, '\0', MAX_PATH);

		if (NetCore_BroadCast_Recv(hBroadSocket, tszMsgBuffer, &nMsgLen, tszIPAddr))
		{
			//收到文件查询请求
			if (XStorageProtocol_Core_REQQueryFile(tszMsgBuffer, tszTimeStart, tszTimeEnd, tszFileName, tszFileHash))
			{
				//查询文件是否存在数据库,不存在不关心
				if (0 != st_ServiceCfg.st_XSql.nSQLType)
				{
					int nListCount = 0;
					XSTORAGECORE_DBFILE** pppSt_ListFile;
					if (1 == st_ServiceCfg.st_XSql.nSQLType)
					{
						XStorageSQL_File_FileQuery(&pppSt_ListFile, &nListCount, tszTimeStart, tszTimeEnd, tszFileName, tszFileHash);
					}
					else
					{
						XStorage_SQLite_FileQuery(&pppSt_ListFile, &nListCount, tszTimeStart, tszTimeEnd, tszFileName, tszFileHash);
					}
					if (nListCount > 0)
					{
						_stprintf(pppSt_ListFile[0]->tszTableName, _T("%s:%d"), st_ServiceCfg.tszIPAddr, st_ServiceCfg.nStorageDLPort);
						XStorageProtocol_Core_REPQueryFile(tszMsgBuffer, &nMsgLen, &pppSt_ListFile, nListCount, tszTimeStart, tszTimeEnd);
						BaseLib_OperatorMemory_Free((XPPPMEM)&pppSt_ListFile, nListCount);

						SOCKET hSDSocket;
						NetCore_BroadCast_SendInit(&hSDSocket, st_ServiceCfg.st_P2xp.nSDPort, st_ServiceCfg.tszIPAddr);
						NetCore_BroadCast_Send(hSDSocket, tszMsgBuffer, nMsgLen);
						NetCore_BroadCast_Close(hSDSocket);
						XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("广播端:%s,请求查询文件成功,列表个数:%d"), tszIPAddr, nListCount);
					}
				}
			}
		}
	}
	return 0;
}
BOOL XEngine_Task_P2p(LPCTSTR lpszFileHash, LPCTSTR lpszClientAddr, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam)
{
	int nSDLen = 2048;
	int nRVLen = 2048;
	int nMsgLen = 2048;
	TCHAR tszSDBuffer[2048];
	TCHAR tszRVBuffer[2048];
	TCHAR tszMsgBuffer[2048];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));
	//首先判断本机存在不
	if (0 != st_ServiceCfg.st_XSql.nSQLType)
	{
		int nListCount = 0;
		XSTORAGECORE_DBFILE** pppSt_ListFile;
		if (1 == st_ServiceCfg.st_XSql.nSQLType)
		{
			XStorageSQL_File_FileQuery(&pppSt_ListFile, &nListCount, NULL, NULL, NULL, lpszFileHash);
		}
		else
		{
			XStorage_SQLite_FileQuery(&pppSt_ListFile, &nListCount, NULL, NULL, NULL, lpszFileHash);
		}
		if (nListCount > 0)
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 200;

			_stprintf(pppSt_ListFile[0]->tszTableName, _T("127.0.0.1"));
			XStorageProtocol_Core_REPQueryFile(tszRVBuffer, &nRVLen, &pppSt_ListFile, nListCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&pppSt_ListFile, nListCount);

			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszMsgBuffer, &nMsgLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszMsgBuffer, nMsgLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("广播端:%s,请求文件查询,发现本地拥有此文件.直接返回"), lpszClientAddr);
			return TRUE;
		}
	}
	//根据使用模式来操作
	if (0 == st_ServiceCfg.st_P2xp.nMode)
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 405;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszMsgBuffer, nMsgLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("广播端:%s,暂时不支持的请求"), lpszClientAddr);
		return FALSE;
	}
	else if (1 == st_ServiceCfg.st_P2xp.nMode)
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 405;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszMsgBuffer, nMsgLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("广播端:%s,暂时不支持的请求"), lpszClientAddr);
		return FALSE;
	}
	else
	{
		//开始广播请求文件
		XStorageProtocol_Client_REQQueryFile(tszSDBuffer, &nSDLen, NULL, lpszFileHash);
		if (!NetCore_BroadCast_Send(hBroadSocket, tszSDBuffer, nSDLen))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 500;

			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszMsgBuffer, &nMsgLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszMsgBuffer, nMsgLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("广播端:%s,发送广播请求失败,错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}

		SOCKET hRVSocket;
		list<APIHELP_LBFILEINFO> stl_ListFile;
		NetCore_BroadCast_RecvInit(&hRVSocket, st_ServiceCfg.st_P2xp.nSDPort);
		time_t nTimeStart = time(NULL);
		while (1)
		{
			APIHELP_LBFILEINFO st_FileInfo;
			memset(&st_FileInfo, '\0', sizeof(APIHELP_LBFILEINFO));

			st_FileInfo.nMsgLen = sizeof(st_FileInfo.tszMsgBuffer);
			if (NetCore_BroadCast_Recv(hRVSocket, st_FileInfo.tszMsgBuffer, &st_FileInfo.nMsgLen))
			{
				stl_ListFile.push_back(st_FileInfo);
			}
			time_t nTimeEnd = time(NULL);
			if ((nTimeEnd - nTimeStart) > st_ServiceCfg.st_P2xp.nTime)
			{
				//大于ntime秒,退出
				break;
			}
		}
		NetCore_BroadCast_Close(hRVSocket);

		if (stl_ListFile.empty())
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 404;
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		}
		else
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 200;

			int nListCount = 0;
			XSTORAGECORE_DBFILE** ppSt_ListPacket;
			APIHelp_Distributed_FileList(&stl_ListFile, &ppSt_ListPacket, &nListCount);
			XStorageProtocol_Core_REPQueryFile(tszRVBuffer, &nRVLen, &ppSt_ListPacket, nListCount);
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszMsgBuffer, &nMsgLen, &st_HDRParam, tszRVBuffer, nRVLen);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListPacket, nListCount);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("广播端:%s,请求局域网文件列表成功,文件分布服务器个数:%d"), lpszClientAddr, stl_ListFile.size());
		XEngine_Net_SendMsg(lpszClientAddr, tszMsgBuffer, nMsgLen, STORAGE_NETTYPE_HTTPCENTER);
		stl_ListFile.clear();
	}
	return TRUE;
}