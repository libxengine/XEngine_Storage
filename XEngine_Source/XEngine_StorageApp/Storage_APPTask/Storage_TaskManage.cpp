﻿#include "../StorageApp_Hdr.h"

BOOL XEngine_Task_Manage(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 10240;
	int nRVLen = 10240;
	TCHAR tszSDBuffer[10240];
	TCHAR tszRVBuffer[10240];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.bIsClose = TRUE;
	st_HDRParam.nHttpCode = 200;
	if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_CONFIG, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_CONFIG)))
	{
		StorageApp_Config_Parament(0, NULL);
		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,处理用户重载配置文件成功"), lpszClientAddr);
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_QUERY, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_QUERY)))
	{
		//查询文件列表
		int nMode = 0;
		int nListCount = 0;
		int nMsgLen = 10240;
		TCHAR tszFileName[MAX_PATH];
		TCHAR tszFileHash[MAX_PATH];
		TCHAR tszBucketKey[128];
		TCHAR tszTimeStart[128];
		TCHAR tszTimeEnd[128];
		TCHAR tszMsgBuffer[10240];
		XSTORAGECORE_DBFILE** ppSt_ListFile;

		memset(tszFileName, '\0', MAX_PATH);
		memset(tszFileHash, '\0', MAX_PATH);
		memset(tszBucketKey, '\0', sizeof(tszBucketKey));
		memset(tszTimeStart, '\0', sizeof(tszTimeStart));
		memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		Protocol_StorageParse_QueryFile(lpszMsgBuffer, tszTimeStart, tszTimeEnd, tszBucketKey, tszFileName, tszFileHash, &nMode);
		//查找数据库
		if (st_ServiceCfg.st_XSql.bEnable)
		{
			Database_File_FileQuery(&ppSt_ListFile, &nListCount, tszTimeStart, tszTimeEnd, tszBucketKey, tszFileName, tszFileHash);
		}
		else
		{
			Database_Client_FileQuery(&ppSt_ListFile, &nListCount, tszTimeStart, tszTimeEnd, tszBucketKey, tszFileName, tszFileHash);
		}
		//根据使用模式来操作
		if (0 == nMode)
		{
			Protocol_StoragePacket_QueryFile(tszMsgBuffer, &nMsgLen, &ppSt_ListFile, nListCount, tszTimeStart, tszTimeEnd);
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszMsgBuffer, nMsgLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListFile, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求查询文件列表成功,列表个数:%d"), lpszClientAddr, nListCount);
			return TRUE;
		}
		else
		{
			//开始广播请求文件
			SOCKET hSDSocket;
			SOCKET hRVSocket;
			list<APIHELP_LBFILEINFO> stl_ListFile;

			Protocol_StoragePacket_REQFile(tszSDBuffer, &nSDLen, NULL, tszFileHash);
			NetCore_BroadCast_SendInit(&hSDSocket, st_ServiceCfg.st_P2xp.nRVPort, st_ServiceCfg.tszIPAddr);
			NetCore_BroadCast_RecvInit(&hRVSocket, st_ServiceCfg.st_P2xp.nSDPort);

			if (!NetCore_BroadCast_Send(hSDSocket, tszSDBuffer, nSDLen))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 500;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				NetCore_BroadCast_Close(hSDSocket);
				NetCore_BroadCast_Close(hRVSocket);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,发送广播请求失败,错误:%lX"), lpszClientAddr, NetCore_GetLastError());
				return FALSE;
			}

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
			NetCore_BroadCast_Close(hSDSocket);
			NetCore_BroadCast_Close(hRVSocket);

			if (stl_ListFile.empty())
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 404;
				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			}
			else
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 200;

				int nListCount = 0;
				XSTORAGECORE_DBFILE** ppSt_ListPacket;
				APIHelp_Distributed_FileList(&stl_ListFile, &ppSt_ListPacket, &nListCount);
				Protocol_StoragePacket_QueryFile(tszRVBuffer, &nRVLen, &ppSt_ListPacket, nListCount);
				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListPacket, nListCount);
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求局域网文件列表成功,文件分布服务器个数:%d"), lpszClientAddr, stl_ListFile.size());
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			stl_ListFile.clear();
		}
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_INSERT, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_INSERT)))
	{
		int nListCount = 0;
		XSTORAGECORE_DBFILE** ppSt_DBFile;

		Protocol_StorageParse_ReportFile(lpszMsgBuffer, nMsgLen, &ppSt_DBFile, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			TCHAR tszFileDir[1024];
			memset(tszFileDir, '\0', sizeof(tszFileDir));
			//判断下文件是否存在就行了
			if (_tcslen(ppSt_DBFile[i]->tszBuckKey) > 0)
			{
				APIHelp_Distributed_GetPathKey(st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, ppSt_DBFile[i]->tszBuckKey, ppSt_DBFile[i]->st_ProtocolFile.tszFilePath);
			}
			_stprintf(tszFileDir, _T("%s/%s"), ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);
			if (0 != _taccess(tszFileDir, 0))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 404;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求添加文件到数据库失败,文件不存在,文件;%s"), lpszClientAddr, tszFileDir);
				return FALSE;
			}
			if (st_ServiceCfg.st_XSql.bEnable)
			{
				Database_File_FileInsert(ppSt_DBFile[i]);
			}
			else
			{
				Database_Client_FileInsert(ppSt_DBFile[i]);
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求添加文件到数据库成功,文件名:%s/%s"), lpszClientAddr, ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);
		}
		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_DBFile, nListCount);
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_DELETE, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_DELETE)))
	{
		int nListCount = 0;
		XSTORAGECORE_DBFILE** ppSt_DBFile;

		Protocol_StorageParse_ReportFile(lpszMsgBuffer, nMsgLen, &ppSt_DBFile, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			if (_tcslen(ppSt_DBFile[i]->st_ProtocolFile.tszFileHash) > 0)
			{
				int nQueryCount = 0;
				XSTORAGECORE_DBFILE** ppSt_DBQuery;

				if (st_ServiceCfg.st_XSql.bEnable)
				{
					Database_File_FileQuery(&ppSt_DBQuery, &nQueryCount, NULL, NULL, NULL, NULL, ppSt_DBFile[i]->st_ProtocolFile.tszFileHash);
				}
				else
				{
					Database_Client_FileQuery(&ppSt_DBQuery, &nQueryCount, NULL, NULL, NULL, NULL, ppSt_DBFile[i]->st_ProtocolFile.tszFileHash);
				}
				//删除数据库与文件
				for (int i = 0; i < nQueryCount; i++)
				{
					TCHAR tszFilePath[1024];
					memset(tszFilePath, '\0', sizeof(tszFilePath));

					_stprintf(tszFilePath, _T("%s/%s"), ppSt_DBQuery[i]->st_ProtocolFile.tszFilePath, ppSt_DBQuery[i]->st_ProtocolFile.tszFileName);
					if (st_ServiceCfg.st_XSql.bEnable)
					{
						Database_File_FileDelete(NULL, NULL, ppSt_DBQuery[i]->st_ProtocolFile.tszFileHash);
					}
					else
					{
						Database_Client_FileDelete(NULL, NULL, ppSt_DBQuery[i]->st_ProtocolFile.tszFileHash);
					}
					_tremove(tszFilePath);
				}
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求删除文件HASH成功,文件名:%s"), lpszClientAddr, ppSt_DBFile[i]->st_ProtocolFile.tszFileHash);
			}
			else
			{
				int nQueryCount = 0;
				TCHAR tszFileDir[1024];
				XSTORAGECORE_DBFILE** ppSt_DBQuery;

				memset(tszFileDir, '\0', sizeof(tszFileDir));
				if (_tcslen(ppSt_DBFile[i]->tszBuckKey) > 0)
				{
					APIHelp_Distributed_GetPathKey(st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, ppSt_DBFile[i]->tszBuckKey, ppSt_DBFile[i]->st_ProtocolFile.tszFilePath);
				}
				_stprintf(tszFileDir, _T("%s/%s"), ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);

				if (st_ServiceCfg.st_XSql.bEnable)
				{
					Database_File_FileQuery(&ppSt_DBQuery, &nQueryCount, NULL, NULL, NULL, tszFileDir);
				}
				else
				{
					Database_Client_FileQuery(&ppSt_DBQuery, &nQueryCount, NULL, NULL, NULL, tszFileDir);
				}
				//删除数据库与文件
				for (int i = 0; i < nQueryCount; i++)
				{
					TCHAR tszFilePath[1024];
					memset(tszFilePath, '\0', sizeof(tszFilePath));

					_stprintf(tszFilePath, _T("%s/%s"), ppSt_DBQuery[i]->st_ProtocolFile.tszFilePath, ppSt_DBQuery[i]->st_ProtocolFile.tszFileName);
					
					if (st_ServiceCfg.st_XSql.bEnable)
					{
						Database_File_FileDelete(NULL, NULL, ppSt_DBQuery[i]->st_ProtocolFile.tszFileHash);
					}
					else
					{
						Database_Client_FileDelete(NULL, NULL, ppSt_DBQuery[i]->st_ProtocolFile.tszFileHash);
					}
					_tremove(tszFilePath);
				}
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求删除文件名称成功,文件名:%s/%s"), lpszClientAddr, ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);
			}
		}
		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_DBFile, nListCount);
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_DIR, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_DIR)))
	{
		int nOPCode = 0;
		int nListCount = 0;
		CHAR** ppszListDir = NULL;
		TCHAR tszUserDir[MAX_PATH];
		TCHAR tszBuckKey[MAX_PATH];
		TCHAR tszBuckPath[MAX_PATH];
		TCHAR tszRealDir[1024];

		memset(tszUserDir, '\0', MAX_PATH);
		memset(tszBuckKey, '\0', MAX_PATH);
		memset(tszBuckPath, '\0', MAX_PATH);
		memset(tszRealDir, '\0', sizeof(tszRealDir));

		Protocol_StorageParse_DirOperator(lpszMsgBuffer, tszUserDir, tszBuckKey, &nOPCode);
		APIHelp_Distributed_GetPathKey(st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, tszBuckKey, tszBuckPath);

		if (_tcslen(tszUserDir) > 0)
		{
			_stprintf(tszRealDir, _T("%s/%s"), tszBuckPath, tszUserDir);
		}
		else
		{
			_stprintf(tszRealDir, _T("%s"), tszBuckPath);
		}

		if (0 == nOPCode)
		{
			//处理路径格式
			if (tszRealDir[_tcslen(tszRealDir) - 1] != '*')
			{
				int nPathType = 0;
				BaseLib_OperatorString_GetPath(tszRealDir, &nPathType);
				//判断是绝对路径还是相对路径
				if (1 == nPathType)
				{
					_tcscat(tszRealDir, _T("\\*"));
				}
				else if (2 == nPathType)
				{
					_tcscat(tszRealDir, _T("/*"));
				}
			}
			if (!SystemApi_File_EnumFile(tszRealDir, &ppszListDir, &nListCount, NULL, NULL, TRUE, 2))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 404;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求查询文件夹:%s,失败,错误:%lX"), lpszClientAddr, tszRealDir, SystemApi_GetLastError());
				return FALSE;
			}
			Protocol_StoragePacket_DirOperator(tszRVBuffer, &nRVLen, &ppszListDir, nListCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListDir, nListCount);
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求查询文件夹:%s,成功,文件夹个数:%d"), lpszClientAddr, tszRealDir, nListCount);
		}
		else if (1 == nOPCode)
		{
			if (!SystemApi_File_CreateMutilFolder(tszRealDir))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 404;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求创建文件夹:%s,失败,错误:%lX"), lpszClientAddr, tszRealDir, SystemApi_GetLastError());
				return FALSE;
			}
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求创建文件夹:%s,成功"), lpszClientAddr, tszRealDir);
		}
		else if (2 == nOPCode)
		{
			if (!SystemApi_File_DeleteMutilFolder(tszRealDir))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.nHttpCode = 404;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求删除文件夹:%s,失败,错误:%lX"), lpszClientAddr, tszRealDir, SystemApi_GetLastError());
				return FALSE;
			}
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求删除文件夹:%s,成功"), lpszClientAddr, tszRealDir);
		}
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_TASK, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_TASK)))
	{
		int nDLCount = 0;
		int nUPCount = 0;
		SESSION_STORAGEINFO** ppSt_DLInfo;
		SESSION_STORAGEINFO** ppSt_UPInfo;

		Session_DLStorage_GetAll(&ppSt_DLInfo, &nDLCount);
		Session_UPStorage_GetAll(&ppSt_UPInfo, &nUPCount);

		Protocol_StoragePacket_Info(tszRVBuffer, &nRVLen, &ppSt_DLInfo, &ppSt_UPInfo, nDLCount, nUPCount);
		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_DLInfo, nDLCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_UPInfo, nUPCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求获取上传下载信息成功,下载个数:%d,上传个数:%d"), lpszClientAddr, nDLCount, nUPCount);
	}
	return TRUE;
}