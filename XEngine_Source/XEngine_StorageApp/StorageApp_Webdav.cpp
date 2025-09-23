#include "StorageApp_Hdr.h"

XHTHREAD XCALLBACK XEngine_Webdav_HTTPThread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (HttpProtocol_Server_EventWaitEx(xhWebdavHttp, nThreadPos))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			HttpProtocol_Server_GetPoolEx(xhWebdavHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					int nMsgLen = 0;
					int nHdrCount = 0;
					XCHAR* ptszMsgBuffer = NULL;
					XCHAR** ppszListHdr = NULL;
					RFCCOMPONENTS_HTTP_REQPARAM st_HTTPParam;

					memset(&st_HTTPParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
					//获得指定上传客户端触发信息
					if (HttpProtocol_Server_GetMemoryEx(xhWebdavHttp, ppSt_PKTClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpWebdav(ppSt_PKTClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
					}
					BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
					BaseLib_Memory_Free((XPPPMEM)&ppszListHdr, nHdrCount);
				}
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}

bool XEngine_Task_HttpWebdav(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 8192;
	int nRVLen = 8192;
	XCHAR tszSDBuffer[8192] = {};
	XCHAR tszRVBuffer[8192] = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	LPCXSTR lpszMethodOption = _X("OPTIONS");
	LPCXSTR lpszMethodPropfind = _X("PROPFIND");
	LPCXSTR lpszMethodPropPatch = _X("PROPPATCH");
	LPCXSTR lpszMethodGet = _X("GET");
	LPCXSTR lpszMethodPut = _X("PUT");
	LPCXSTR lpszMethodDel = _X("DELETE");
	LPCXSTR lpszMethodLock = _X("LOCK");
	LPCXSTR lpszMethodUNLock = _X("UNLOCK");
	LPCXSTR lpszMethodMKCol = _X("MKCOL");

	st_HDRParam.bIsClose = false;
	st_HDRParam.nHttpCode = 200;
	_tcsxcpy(st_HDRParam.tszMimeType, _X("xml"));

	if (st_ServiceCfg.st_XProxy.bWDPass)
	{
		int nVType = 0;
		RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

		st_HDRParam.nHttpCode = 401;
		st_HDRParam.bIsClose = true;
		st_HDRParam.bAuth = true;
		//打包验证信息
		int nHDRLen = 0;
		XCHAR tszHDRBuffer[XPATH_MAX] = {};
		if (1 == st_ServiceCfg.st_XProxy.nVType)
		{
			Verification_HTTP_BasicServerPacket(tszHDRBuffer, &nHDRLen);
		}
		else
		{
			XCHAR tszNonceStr[64] = {};
			XCHAR tszOpaqueStr[64] = {};
			Verification_HTTP_DigestServerPacket(tszHDRBuffer, &nHDRLen, tszNonceStr, tszOpaqueStr);
		}
		//后去验证方法
		if (!Verification_HTTP_GetType(pptszListHdr, nHdrCount, &nVType))
		{
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,用户验证失败,验证方式:%d,错误:%lX"), lpszClientAddr, st_ServiceCfg.st_XProxy.nVType, Verification_GetLastError());
			return false;
		}
		//验证方式是否一致
		if (st_ServiceCfg.st_XProxy.nVType != nVType)
		{
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,用户验证失败,验证方式错误,请求:%d,需求:%d"), lpszClientAddr, nVType, st_ServiceCfg.st_XProxy.nVType);
			return false;
		}
		bool bRet = false;
		int nHTTPCode = 0;
		int nMSGLen = 0;
		XCLIENT_APIHTTP st_APIHttp = {};

		XCHAR* ptszMSGBuffer = NULL;
		if (!APIClient_Http_Request(_X("GET"), st_ServiceCfg.st_XProxy.tszWDPass, NULL, &nHTTPCode, &ptszMSGBuffer, &nMSGLen, NULL, NULL, &st_APIHttp))
		{
			st_HDRParam.nHttpCode = 500;
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_SERVERDOWN, "api server is down,cant verification");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误码:%lX"), lpszClientAddr, st_ServiceCfg.st_XProxy.tszWDPass, APIClient_GetLastError());
			return false;
		}
		if (200 != nHTTPCode)
		{
			st_HDRParam.nHttpCode = 500;
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_SERVERDOWN, "api server is down,cant verification");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误:%d"), lpszClientAddr, st_ServiceCfg.st_XProxy.tszWDPass, nHTTPCode);
			return false;
		}
		XENGINE_PROTOCOL_USERAUTH st_UserAuth = {};
		if (!Protocol_StorageParse_User(ptszMSGBuffer, nMsgLen, &st_UserAuth))
		{
			st_HDRParam.nHttpCode = 500;
			Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_AUTHFAIL, "api server reply failure,cant verification");
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,用户验证失败,返回内容:%s 错误,无法继续"), lpszClientAddr, ptszMSGBuffer);
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);
			return false;
		}
		BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);

		if (1 == nVType)
		{
			bRet = Verification_HTTP_Basic(st_UserAuth.tszUserName, st_UserAuth.tszUserPass, pptszListHdr, nHdrCount);
		}
		else if (2 == nVType)
		{
			bRet = Verification_HTTP_Digest(st_UserAuth.tszUserName, st_UserAuth.tszUserPass, pSt_HTTPParam->tszHttpMethod, pptszListHdr, nHdrCount);
		}

		if (!bRet)
		{
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,用户验证失败,验证处理错误,可能用户密码登信息不匹配,类型:%d"), lpszClientAddr, nVType);
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,HTTP验证类型:%d 通过"), lpszClientAddr, nVType);
	}
	//http://127.0.0.1:5103/storagekey1
	if (0 == _tcsxnicmp(lpszMethodOption, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodOption)))
	{
		//用于心跳
		st_HDRParam.bIsClose = true;
		st_HDRParam.nHttpCode = 200;
		LPCXSTR lpszHdrBuffer = _X("Allow: OPTIONS POST GET PUT PROPFIND PROPPATCH DELETE LOCK UNLOCK MKCOL\r\n");
		HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, lpszHdrBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,请求OPTIONS心跳方法成功"), lpszClientAddr);
	}
	else if (0 == _tcsxnicmp(lpszMethodPropfind, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPropfind)))
	{
		if (_tcsxlen(pSt_HTTPParam->tszHttpUri) <= 1)
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 403;
			HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,处理WEBDAV协议PROPFIND方法失败,地址不正确,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		XENGINE_STORAGEBUCKET st_StorageBucket = {};
		if (!APIHelp_Distributed_DLStorage(pSt_HTTPParam->tszHttpUri, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,处理WEBDAV协议PROPFIND方法失败,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		int nListCount = 0;
		XCHAR** pptszListFile;
		//得到是否是文件
		int nALen = 0;
		XCHAR tszFileUrl[XPATH_MAX] = {};
		SYSTEMAPI_FILE_ATTR st_FileAttr = {};
		_tcsxcpy(tszFileUrl, pSt_HTTPParam->tszHttpUri + 1);
		BaseLib_String_Replace(tszFileUrl, &nALen, st_StorageBucket.tszBuckKey, st_StorageBucket.tszFilePath);

		if (!SystemApi_File_GetFileAttr(tszFileUrl, &st_FileAttr))
		{
			st_HDRParam.nHttpCode = 404;
			Protocol_StoragePacket_Notfound(tszRVBuffer, &nRVLen, pSt_HTTPParam->tszHttpUri);
			HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,处理WEBDAV协议PROPFIND方法失败,文件没有找到,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		if (st_FileAttr.bFile)
		{
			//如果是文件
			nListCount = 1;
			BaseLib_Memory_Malloc((XPPPMEM)&pptszListFile, nListCount, XPATH_MAX);
			_tcsxcpy(pptszListFile[0], tszFileUrl);
		}
		else
		{
			//得到深度
			XCHAR tszVluStr[8] = {};
			HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, _X("Depth"), tszVluStr);
			if (1 == _ttxoi(tszVluStr))
			{
				SystemApi_File_EnumFile(tszFileUrl, &pptszListFile, &nListCount, false, 3);
			}
			else
			{
				SystemApi_File_EnumFile(tszFileUrl, &pptszListFile, &nListCount);
			}
			//如果是空的文件夹
			if (0 == nListCount)
			{
				nListCount = 1;
				BaseLib_Memory_Malloc((XPPPMEM)&pptszListFile, nListCount, XPATH_MAX);
				_tcsxcpy(pptszListFile[0], tszFileUrl);
			}
		}
		Protocol_StoragePacket_WDPropfind(tszRVBuffer, &nRVLen, &pptszListFile, nListCount, st_StorageBucket.tszFilePath, st_StorageBucket.tszBuckKey);

		st_HDRParam.nHttpCode = 207;
		_tcsxcpy(st_HDRParam.tszMimeType, _X("xml"));
		HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,处理WEBDAV协议PROPFIND方法成功,文件名称:%s"), lpszClientAddr, tszFileUrl);
	}
	else if (0 == _tcsxnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		if (st_ServiceCfg.st_XStorage.bWDLocation)
		{
			XEngine_Task_HttpDownload(lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPWEBDAV);
		}
		else
		{
			//使用重定向实现下载
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 302;

			XCHAR tszRequestAddr[512] = {};
			XCHAR tszHostStr[128] = {};
			HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, _X("Host"), tszHostStr);

			XCHAR tszPortWebdav[64] = {};
			XCHAR tszPortDownload[64] = {};
			_xstprintf(tszPortWebdav, _X("%d"), st_ServiceCfg.nWebdavPort);
			_xstprintf(tszPortDownload, _X("%d"), st_ServiceCfg.nStorageDLPort);
			//转换端口
			int nReplaceLen = 0;
			BaseLib_String_Replace(tszHostStr, &nReplaceLen, tszPortWebdav, tszPortDownload, true);
			_xstprintf(tszRequestAddr, _X("Location: http://%s%s\r\n"), tszHostStr, pSt_HTTPParam->tszHttpUri);

			HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszRequestAddr);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,请求文件下载被重定向到:%s"), lpszClientAddr, tszRequestAddr);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodPut, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPut)))
	{
		if (st_ServiceCfg.st_XStorage.bWDLocation)
		{
			//转换参数格式:/storagekey1/webdav/clash-verge-rev-backup/backup.zip 为 /api?filename=webdav/clash-verge-rev-backup/backup.zip&storeagekey=storagekey1
			XCHAR tszConvertStr[XPATH_MAX] = {};
			APIHelp_Api_WDToUrl(pSt_HTTPParam->tszHttpUri, tszConvertStr);
			memset(pSt_HTTPParam->tszHttpUri, '\0', sizeof(pSt_HTTPParam->tszHttpUri));
			_tcsxcpy(pSt_HTTPParam->tszHttpUri, tszConvertStr);
			XEngine_Task_HttpUPLoader(lpszClientAddr, lpszMsgBuffer, nMsgLen, pSt_HTTPParam, pptszListHdr, nHdrCount, STORAGE_NETTYPE_HTTPWEBDAV);
		}
		else
		{
			//使用重定向实现上传
			st_HDRParam.bIsClose = false;
			st_HDRParam.nHttpCode = 302;

			XCHAR tszRequestAddr[1024] = {};
			XCHAR tszHostStr[128] = {};
			HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, _X("Host"), tszHostStr);

			XCHAR tszPortWebdav[64] = {};
			XCHAR tszPortDownload[64] = {};
			_xstprintf(tszPortWebdav, _X("%d"), st_ServiceCfg.nWebdavPort);
			_xstprintf(tszPortDownload, _X("%d"), st_ServiceCfg.nStorageUPPort);
			//转换端口
			int nRLen = 0;
			BaseLib_String_Replace(tszHostStr, &nRLen, tszPortWebdav, tszPortDownload, true);
			//转换地址
			XCHAR tszStroageKey[XPATH_MAX] = {};
			XCHAR tszFileName[XPATH_MAX] = {};

			int nRet = _stxscanf(pSt_HTTPParam->tszHttpUri + 1, _X("%99[^/]/%199[^\n]"), tszStroageKey, tszFileName);
			if (2 != nRet)
			{
				st_HDRParam.nHttpCode = 413;
				HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,处理WEBDAV协议上传方法失败,文件请求路径不正确,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			_xstprintf(tszRequestAddr, _X("Location: http://%s/api?filename=%s&storeagekey=%s\r\n"), tszHostStr, tszFileName, tszStroageKey);

			HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszRequestAddr);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,请求文件上传被重定向到:%s"), lpszClientAddr, tszRequestAddr);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodLock, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodLock)))
	{
		//使用重定向实现上传
		st_HDRParam.bIsClose = false;
		st_HDRParam.nHttpCode = 200;

		XENGINE_WEBDAVLOCK st_WDLock = {};
		if (!Protocol_StorageParse_WDLock(lpszMsgBuffer, nMsgLen, &st_WDLock))
		{
			st_HDRParam.nHttpCode = 413;
			HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,处理WEBDAV协议LOCK方法失败,协议不正确,内容:%s"), lpszClientAddr, lpszMsgBuffer);
			return false;
		}
		Session_Webdav_Insert(pSt_HTTPParam->tszHttpUri, &st_WDLock);
		Protocol_StoragePacket_WDLock(tszRVBuffer, &nRVLen, &st_WDLock);

		HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,处理WEBDAV协议LOCK方法成功,锁用户:%s"), lpszClientAddr, st_WDLock.tszOwner);
	}
	else if (0 == _tcsxnicmp(lpszMethodUNLock, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodUNLock)))
	{
		//使用重定向实现上传
		st_HDRParam.bIsClose = false;
		st_HDRParam.nHttpCode = 204;

		Session_Webdav_Delete(pSt_HTTPParam->tszHttpUri);
		HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,处理WEBDAV协议UNLOCK方法成功,锁文件:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
	}
	else if (0 == _tcsxnicmp(lpszMethodPropPatch, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPropPatch)))
	{
		st_HDRParam.bIsClose = false;
		st_HDRParam.nHttpCode = 207;

		std::list<string> stl_ListName;
		Protocol_StorageParse_WDPropPatch(lpszMsgBuffer, nMsgLen, &stl_ListName);

		Protocol_StoragePacket_WDPropPatch(tszRVBuffer, &nRVLen, pSt_HTTPParam->tszHttpUri, &stl_ListName);
		HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,处理WEBDAV协议PROPPATCH方法成功,锁用户:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
	}
	else if (0 == _tcsxnicmp(lpszMethodDel, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodDel)))
	{
		//使用重定向实现上传
		st_HDRParam.bIsClose = false;
		st_HDRParam.nHttpCode = 204;

		XENGINE_STORAGEBUCKET st_StorageBucket = {};
		if (!APIHelp_Distributed_DLStorage(pSt_HTTPParam->tszHttpUri, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,处理WEBDAV协议DELETE方法失败,获取BUCKET失败,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		int nFLen = 0;
		XCHAR tszFileName[XPATH_MAX] = {};
		_tcsxcpy(tszFileName, pSt_HTTPParam->tszHttpUri + 1);
		BaseLib_String_Replace(tszFileName, &nFLen, st_StorageBucket.tszBuckKey, st_StorageBucket.tszFilePath);

		SYSTEMAPI_FILE_ATTR st_FileAttr = {};
		if (SystemApi_File_GetFileAttr(tszFileName, &st_FileAttr))
		{
			if (st_FileAttr.bFile)
			{
				__int64x nTmpSize = st_FileAttr.nFileSize;
				APIHelp_Distributed_SetSize(st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, st_StorageBucket.tszBuckKey, -nTmpSize);
				_xtremove(tszFileName);
			}
			else
			{
				//如果是文件夹
				int nFileCount = 0;
				XCHAR** pptszListFile;
				SystemApi_File_EnumFile(tszFileName, &pptszListFile, &nFileCount);
				BaseLib_Memory_Free((XPPPMEM)&pptszListFile, nFileCount);
				if (nFileCount > 0)
				{
					st_HDRParam.bIsClose = true;
					st_HDRParam.nHttpCode = 400;
					HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
					XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,处理WEBDAV协议DELETE方法失败,删除的文件夹不为空,URL:%s"), lpszClientAddr, tszFileName);
					return false;
				}
				SystemApi_File_DeleteMutilFolder(tszFileName, true);
			}
		}
		HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,处理WEBDAV协议DELETE方法成功,删除的文件:%s"), lpszClientAddr, tszFileName);
	}
	else if (0 == _tcsxnicmp(lpszMethodMKCol, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodMKCol)))
	{
		XENGINE_STORAGEBUCKET st_StorageBucket = {};
		if (!APIHelp_Distributed_DLStorage(pSt_HTTPParam->tszHttpUri, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,处理WEBDAV协议PROPFIND方法失败,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		int nALen = 0;
		XCHAR tszCreateDir[XPATH_MAX] = {};
		_tcsxcpy(tszCreateDir, pSt_HTTPParam->tszHttpUri + 1);
		BaseLib_String_Replace(tszCreateDir, &nALen, st_StorageBucket.tszBuckKey, st_StorageBucket.tszFilePath, true);

		SystemApi_File_CreateMutilFolder(tszCreateDir);
		st_HDRParam.nHttpCode = 201;
		HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBDAV客户端:%s,处理WEBDAV协议MLCOL方法成功,创建的路径:%s"), lpszClientAddr, tszCreateDir);
	}
	else
	{
		st_HDRParam.bIsClose = true;
		st_HDRParam.nHttpCode = 405;

		HttpProtocol_Server_SendMsgEx(xhWebdavHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPWEBDAV);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBDAV客户端:%s,发送的方法不支持"), lpszClientAddr);
		return false;
	}
	
	return true;
}