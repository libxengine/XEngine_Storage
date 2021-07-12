#include "../StorageApp_Hdr.h"

BOOL XEngine_Task_Event(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	//事件处理类型,上传事件,用于NGINX
	if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_UPFILE, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_UPFILE)))
	{
		LPCTSTR lpszContentType = _T("Content-Type");
		LPCTSTR lpszBoundaryStr = _T("boundary=");
		TCHAR tszContentStr[MAX_PATH];
		TCHAR tszBoundarTmp[MAX_PATH];
		TCHAR tszFileDir[MAX_PATH];
		TCHAR tszBoundarStr[512];
		XSTORAGECORE_DBFILE st_DBFile;

		memset(tszContentStr, '\0', MAX_PATH);
		memset(tszBoundarTmp, '\0', MAX_PATH);
		memset(tszContentStr, '\0', MAX_PATH);
		memset(tszFileDir, '\0', MAX_PATH);
		memset(tszBoundarStr, '\0', sizeof(tszBoundarStr));
		memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

		if (!RfcComponents_HttpHelp_GetField(&pptszListHdr, nHdrCount, lpszContentType, tszContentStr))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 400;

			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求的HTTP头内容不正确,没有类型字段"), lpszClientAddr);
			return FALSE;
		}
		if (!BaseLib_OperatorString_FromStrGetKeyValue(tszContentStr, lpszBoundaryStr, NULL, tszBoundarTmp))
		{
			st_HDRParam.bIsClose = TRUE;
			st_HDRParam.nHttpCode = 400;

			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求的HTTP头内容不正确,没有Boundary值"), lpszClientAddr);
			return FALSE;
		}
		_stprintf(tszBoundarStr, _T("--%s\r\n"), tszBoundarTmp);
		XStorageProtocol_Core_REQUPEvent(lpszMsgBuffer, tszBoundarStr, st_DBFile.st_ProtocolFile.tszFileName, tszFileDir, st_DBFile.st_ProtocolFile.tszFileHash, &st_DBFile.st_ProtocolFile.nFileSize);

		if (st_ServiceCfg.st_XStorage.bRename)
		{
			memset(st_DBFile.st_ProtocolFile.tszFilePath, '\0', MAX_PATH);
			_tcscpy(st_DBFile.st_ProtocolFile.tszFilePath, st_ServiceCfg.st_XStorage.tszFileDir);
		}
		else
		{
			memset(st_DBFile.st_ProtocolFile.tszFileName, '\0', MAX_PATH);
			BaseLib_OperatorString_GetFileAndPath(tszFileDir, st_DBFile.st_ProtocolFile.tszFilePath, st_DBFile.st_ProtocolFile.tszFileName);
			st_DBFile.st_ProtocolFile.tszFilePath[_tcslen(st_DBFile.st_ProtocolFile.tszFilePath) - 1] = '\0';
		}

		if (0 != st_ServiceCfg.st_XSql.nSQLType)
		{
			BOOL bRet = FALSE;
			if (1 == st_ServiceCfg.st_XSql.nSQLType)
			{
				bRet = XStorageSQL_File_FileInsert(&st_DBFile);
			}
			else
			{
				bRet = XStorage_SQLite_FileInsert(&st_DBFile);
			}
			if (bRet)
			{
				st_HDRParam.nHttpCode = 200;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,处理NGINX代理上传文件成功,文件名:%s,大小:%lld"), lpszClientAddr, tszFileDir, st_DBFile.st_ProtocolFile.nFileSize);
			}
			else
			{
				st_HDRParam.nHttpCode = 403;
				RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,处理NGINX代理上传文件失败,插入数据库失败:%s,错误:%lX"), lpszClientAddr, tszFileDir, XStorageDB_GetLastError());
			}
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("业务客户端:%s,处理NGINX代理上传文件成功,文件名:%s,大小:%lld,服务器没有启用数据库,不插入"), lpszClientAddr, tszFileDir, st_DBFile.st_ProtocolFile.nFileSize);
		}

		if (st_ServiceCfg.st_XStorage.bRename)
		{
			TCHAR tszFileTmp[1024];
			memset(tszFileTmp, '\0', sizeof(tszFileTmp));

			_stprintf(tszFileTmp, _T("%s/%s"), st_DBFile.st_ProtocolFile.tszFilePath, st_DBFile.st_ProtocolFile.tszFileName);
			_trename(tszFileDir, tszFileTmp);
		}
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_CONFIG, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_CONFIG)))
	{
		StorageApp_Config_Parament(0, NULL);
		st_HDRParam.nHttpCode = 200;
		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,处理用户重载配置文件成功"), lpszClientAddr);
	}
	return TRUE;
}