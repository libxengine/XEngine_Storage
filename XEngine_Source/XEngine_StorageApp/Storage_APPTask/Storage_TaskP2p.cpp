﻿#include "../StorageApp_Hdr.h"

XHTHREAD XEngine_Task_P2PThread()
{
	XCHAR tszMsgBuffer[2048];
	XCHAR tszIPAddr[128];
	XCHAR tszTimeStart[64];
	XCHAR tszTimeEnd[64];
	XCHAR tszBuckKey[XPATH_MAX];
	XCHAR tszFileName[XPATH_MAX];
	XCHAR tszFileHash[XPATH_MAX];

	while (bIsRun)
	{
		int nMsgLen = 2048;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(tszIPAddr, '\0', sizeof(tszIPAddr));
		memset(tszTimeStart, '\0', sizeof(tszTimeStart));
		memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
		memset(tszBuckKey, '\0', XPATH_MAX);
		memset(tszFileName, '\0', XPATH_MAX);
		memset(tszFileHash, '\0', XPATH_MAX);

		if (NetCore_BroadCast_Recv(hBroadSocket, tszMsgBuffer, &nMsgLen, tszIPAddr))
		{
			XNETHANDLE xhToken = 0;
			//收到文件查询请求
			if (Protocol_StorageParse_QueryFile(tszMsgBuffer, tszTimeStart, tszTimeEnd, tszBuckKey, tszFileName, tszFileHash, NULL, &xhToken))
			{
				//查询文件是否存在数据库,不存在不关心
				int nListCount = 0;
				XSTORAGECORE_DBFILE** pppSt_ListFile;
				Database_Memory_FileQuery(&pppSt_ListFile, &nListCount, tszBuckKey, tszFileName, tszFileHash);
				if (nListCount > 0)
				{
					_xstprintf(pppSt_ListFile[0]->tszTableName, _X("%s:%d"), st_ServiceCfg.tszIPAddr, st_ServiceCfg.nStorageDLPort);
					Protocol_StoragePacket_QueryFile(tszMsgBuffer, &nMsgLen, &pppSt_ListFile, nListCount, tszTimeStart, tszTimeEnd, xhToken);
					BaseLib_Memory_Free((XPPPMEM)&pppSt_ListFile, nListCount);

					XSOCKET hSocket;
					NetCore_BroadCast_Create(&hSocket, st_ServiceCfg.st_P2xp.nSDPort, st_ServiceCfg.tszIPAddr);
					NetCore_BroadCast_Send(hSocket, tszMsgBuffer, nMsgLen);
					NetCore_BroadCast_Close(hSocket);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("广播端:%s,请求查询文件成功,Token:%llu,列表个数:%d"), tszIPAddr, xhToken, nListCount);
				}
			}
		}
	}
	return 0;
}