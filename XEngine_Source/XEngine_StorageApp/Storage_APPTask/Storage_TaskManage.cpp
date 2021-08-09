#include "../StorageApp_Hdr.h"


BOOL XEngine_Task_Manage(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	//文件存储成功的事件上传
	if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_ADD, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_ADD)))
	{
		int nListCount = 0;
		XSTORAGECORE_DBFILE** ppSt_DBFile;

		XStorageProtocol_Core_ReportFileParse(lpszMsgBuffer, nMsgLen, &ppSt_DBFile, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			if (0 == ppSt_DBFile[i]->st_ProtocolFile.nFileSize)
			{
				int nHashLen = 0;
				UCHAR tszHashStr[MAX_PATH];
				TCHAR tszFileDir[1024];
				struct __stat64 st_FStat;

				memset(tszHashStr, '\0', MAX_PATH);
				memset(tszFileDir, '\0', sizeof(tszFileDir));

				_stprintf(tszFileDir, _T("%s/%s"), ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);

				int nRet = _stat64(tszFileDir, &st_FStat);
				if (0 != nRet)
				{
					st_HDRParam.bIsClose = TRUE;
					st_HDRParam.nHttpCode = 404;
					RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
					XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求添加文件到数据库失败,文件不存在,文件;%s"), lpszClientAddr, tszFileDir);
					return FALSE;
				}
				//获取大小
				ppSt_DBFile[i]->st_ProtocolFile.nFileSize = st_FStat.st_size;
				//计算HASH
				OPenSsl_Api_Digest(tszFileDir, tszHashStr, &nHashLen, TRUE, st_ServiceCfg.st_XStorage.nHashMode);
				BaseLib_OperatorString_StrToHex((char*)tszHashStr, nHashLen, ppSt_DBFile[i]->st_ProtocolFile.tszFileHash);
			}
			if (0 != st_ServiceCfg.st_XSql.nSQLType)
			{
				if (1 == st_ServiceCfg.st_XSql.nSQLType)
				{
					XStorageSQL_File_FileInsert(ppSt_DBFile[i]);
				}
				else
				{
					XStorage_SQLite_FileInsert(ppSt_DBFile[i]);
				}
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求添加文件到数据库成功,文件名:%s/%s"), lpszClientAddr, ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);
		}
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 200;
		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_DBFile, nListCount);
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_DEL, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_DEL)))
	{
		int nListCount = 0;
		XSTORAGECORE_DBFILE** ppSt_DBFile;

		XStorageProtocol_Core_ReportFileParse(lpszMsgBuffer, nMsgLen, &ppSt_DBFile, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			if (_tcslen(ppSt_DBFile[i]->st_ProtocolFile.tszFileHash) > 0)
			{
				if (0 != st_ServiceCfg.st_XSql.nSQLType)
				{
					int nQueryCount = 0;
					XSTORAGECORE_DBFILE** ppSt_DBQuery;
					if (1 == st_ServiceCfg.st_XSql.nSQLType)
					{
						XStorageSQL_File_FileQuery(&ppSt_DBQuery, &nQueryCount, NULL, NULL, NULL, ppSt_DBFile[i]->st_ProtocolFile.tszFileHash);
						//删除数据库与文件
						for (int i = 0; i < nQueryCount; i++)
						{
							TCHAR tszFilePath[1024];
							memset(tszFilePath, '\0', sizeof(tszFilePath));

							_stprintf(tszFilePath, _T("%s/%s"), ppSt_DBQuery[i]->st_ProtocolFile.tszFilePath, ppSt_DBQuery[i]->st_ProtocolFile.tszFileName);
							XStorageSQL_File_FileDelete(NULL, ppSt_DBQuery[i]->st_ProtocolFile.tszFileHash);
						}
					}
					else
					{
						XStorage_SQLite_FileQuery(&ppSt_DBQuery, &nQueryCount, NULL, NULL, NULL, ppSt_DBFile[i]->st_ProtocolFile.tszFileHash);
						//删除数据库与文件
						for (int i = 0; i < nQueryCount; i++)
						{
							TCHAR tszFilePath[1024];
							memset(tszFilePath, '\0', sizeof(tszFilePath));

							_stprintf(tszFilePath, _T("%s/%s"), ppSt_DBQuery[i]->st_ProtocolFile.tszFilePath, ppSt_DBQuery[i]->st_ProtocolFile.tszFileName);
							XStorage_SQLite_FileDelete(NULL, ppSt_DBQuery[i]->st_ProtocolFile.tszFileHash);
						}
					}
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求删除文件HASH成功,文件名:%s"), lpszClientAddr, ppSt_DBFile[i]->st_ProtocolFile.tszFileHash);
				}
				else
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求删除文件失败,因为没有启用数据库,删除的HASH:%s"), lpszClientAddr, ppSt_DBFile[i]->st_ProtocolFile.tszFileHash);
				}
			}
			else
			{
				if (0 != st_ServiceCfg.st_XSql.nSQLType)
				{
					int nQueryCount = 0;
					TCHAR tszFileDir[1024];
					XSTORAGECORE_DBFILE** ppSt_DBQuery;

					memset(tszFileDir, '\0', sizeof(tszFileDir));
					_stprintf(tszFileDir, _T("%s/%s"), ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);
					if (1 == st_ServiceCfg.st_XSql.nSQLType)
					{
						XStorageSQL_File_FileQuery(&ppSt_DBQuery, &nQueryCount, NULL, NULL, tszFileDir);
						//删除数据库与文件
						for (int i = 0; i < nQueryCount; i++)
						{
							TCHAR tszFilePath[1024];
							memset(tszFilePath, '\0', sizeof(tszFilePath));

							_stprintf(tszFilePath, _T("%s/%s"), ppSt_DBQuery[i]->st_ProtocolFile.tszFilePath, ppSt_DBQuery[i]->st_ProtocolFile.tszFileName);
							XStorageSQL_File_FileDelete(NULL, ppSt_DBQuery[i]->st_ProtocolFile.tszFileHash);
						}
					}
					else
					{
						XStorage_SQLite_FileQuery(&ppSt_DBQuery, &nQueryCount, NULL, NULL, tszFileDir);
						//删除数据库与文件
						for (int i = 0; i < nQueryCount; i++)
						{
							TCHAR tszFilePath[1024];
							memset(tszFilePath, '\0', sizeof(tszFilePath));

							_stprintf(tszFilePath, _T("%s/%s"), ppSt_DBQuery[i]->st_ProtocolFile.tszFilePath, ppSt_DBQuery[i]->st_ProtocolFile.tszFileName);
							XStorage_SQLite_FileDelete(NULL, ppSt_DBQuery[i]->st_ProtocolFile.tszFileHash);
						}
					}
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求删除文件名称成功,文件名:%s/%s"), lpszClientAddr, ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);
				}
				else
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求删除文件名称失败,因为没有启用数据库,删除的HASH:%s"), lpszClientAddr, ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);
				}
			}
		}
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 200;
		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_DBFile, nListCount);
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_QUERYFILE, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_QUERYFILE)))
	{
		//查询文件列表
		int nMsgLen = 10240;
		TCHAR tszFileName[MAX_PATH];
		TCHAR tszFileHash[MAX_PATH];
		TCHAR tszTimeStart[128];
		TCHAR tszTimeEnd[128];
		TCHAR tszMsgBuffer[10240];

		memset(tszFileName, '\0', MAX_PATH);
		memset(tszFileHash, '\0', MAX_PATH);
		memset(tszTimeStart, '\0', sizeof(tszTimeStart));
		memset(tszTimeEnd, '\0', sizeof(tszTimeEnd));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		int nListCount = 0;
		XSTORAGECORE_DBFILE** ppSt_ListFile;
		XStorageProtocol_Core_REQQueryFile(lpszMsgBuffer, tszTimeStart, tszTimeEnd, tszFileHash);

		if (0 == st_ServiceCfg.st_XSql.nSQLType)
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 406;

			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("业务客户端:%s,请求查询文件列表失败,服务器没有启用这个功能"), lpszClientAddr);
		}
		else
		{
			if (1 == st_ServiceCfg.st_XSql.nSQLType)
			{
				XStorageSQL_File_FileQuery(&ppSt_ListFile, &nListCount, tszTimeStart, tszTimeEnd, tszFileName, tszFileHash);
			}
			else
			{
				XStorage_SQLite_FileQuery(&ppSt_ListFile, &nListCount, tszTimeStart, tszTimeEnd, tszFileName, tszFileHash);
			}
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 200;

			XStorageProtocol_Core_REPQueryFile(tszMsgBuffer, &nMsgLen, &ppSt_ListFile, nListCount, tszTimeStart, tszTimeEnd);
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszMsgBuffer, nMsgLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListFile, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求查询文件列表成功,列表个数:%d"), lpszClientAddr, nListCount);
		}
	}
	return TRUE;
}