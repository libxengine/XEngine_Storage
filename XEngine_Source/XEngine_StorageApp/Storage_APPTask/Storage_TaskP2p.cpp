#include "../StorageApp_Hdr.h"

XHTHREAD XEngine_Task_P2PThread()
{
	TCHAR tszMsgBuffer[2048];
	TCHAR tszIPAddr[128];
	TCHAR tszTimeStart[64];
	TCHAR tszTimeEnd[64];
	TCHAR tszBuckKey[MAX_PATH];
	TCHAR tszFileName[MAX_PATH];
	TCHAR tszFileHash[MAX_PATH];

	while (bIsRun)
	{
		int nMsgLen = 2048;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(tszIPAddr, '\0', sizeof(tszIPAddr));
		memset(tszTimeStart, '\0', sizeof(tszTimeStart));
		memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
		memset(tszBuckKey, '\0', MAX_PATH);
		memset(tszFileName, '\0', MAX_PATH);
		memset(tszFileHash, '\0', MAX_PATH);

		if (NetCore_BroadCast_Recv(hBroadSocket, tszMsgBuffer, &nMsgLen, tszIPAddr))
		{
			XNETHANDLE xhToken = 0;
			//收到文件查询请求
			if (Protocol_StorageParse_QueryFile(tszMsgBuffer, tszTimeStart, tszTimeEnd, tszBuckKey, tszFileName, tszFileHash, NULL, &xhToken))
			{
				//查询文件是否存在数据库,不存在不关心
				int nListCount = 0;
				XSTORAGECORE_DBFILE** pppSt_ListFile;
				Database_File_FileQuery(&pppSt_ListFile, &nListCount, tszTimeStart, tszTimeEnd, tszBuckKey, tszFileName, tszFileHash);
				if (nListCount > 0)
				{
					_stprintf(pppSt_ListFile[0]->tszTableName, _T("%s:%d"), st_ServiceCfg.tszIPAddr, st_ServiceCfg.nStorageDLPort);
					Protocol_StoragePacket_QueryFile(tszMsgBuffer, &nMsgLen, &pppSt_ListFile, nListCount, tszTimeStart, tszTimeEnd, xhToken);
					BaseLib_OperatorMemory_Free((XPPPMEM)&pppSt_ListFile, nListCount);

					SOCKET hSocket;
					NetCore_BroadCast_Create(&hSocket, st_ServiceCfg.st_P2xp.nSDPort, st_ServiceCfg.tszIPAddr);
					NetCore_BroadCast_Send(hSocket, tszMsgBuffer, nMsgLen);
					NetCore_BroadCast_Close(hSocket);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("广播端:%s,请求查询文件成功,Token:%llu,列表个数:%d"), tszIPAddr, xhToken, nListCount);
				}
			}
		}
	}
	return 0;
}