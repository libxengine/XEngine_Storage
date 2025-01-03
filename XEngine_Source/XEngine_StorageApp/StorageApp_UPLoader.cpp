#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_UPLoader_HTTPThread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (HttpProtocol_Server_EventWaitEx(xhUPHttp, nThreadPos))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			HttpProtocol_Server_GetPoolEx(xhUPHttp, nThreadPos, &ppSt_PKTClient, &nListCount);
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
					if (HttpProtocol_Server_GetMemoryEx(xhUPHttp, ppSt_PKTClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPParam, &ppszListHdr, &nHdrCount))
					{
						XEngine_Task_HttpUPLoader(ppSt_PKTClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_HTTPParam, ppszListHdr, nHdrCount);
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
void CALLBACK XEngine_UPLoader_UPFlow(XHANDLE xhToken, bool bSDFlow, bool bRVFlow, bool bTime, __int64u nSDFlow, __int64u nRVFlow, __int64u nTimeFlow, XPVOID lParam)
{
	XCHAR tszIPAddr[128] = {};
	_tcsxcpy(tszIPAddr, (LPCXSTR)lParam);
	if (bSDFlow)
	{
		NetCore_TCPXCore_PasueRecvEx(xhNetUPLoader, tszIPAddr, false);
	}
	else
	{
		NetCore_TCPXCore_PasueRecvEx(xhNetUPLoader, tszIPAddr, true);
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("上传客户端:%s,接受数据标志:%d,当前平均流量:%llu"), tszIPAddr, bSDFlow, nSDFlow);
}
bool XEngine_Task_HttpUPLoader(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	int nLimit = 0;
	XCHAR tszSDBuffer[2048];
	XCHAR tszFileDir[1024];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszFileDir, '\0', sizeof(tszFileDir));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodPut = _X("PUT");

	if ((0 != _tcsxncmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPost))) && (0 != _tcsxncmp(lpszMethodPut, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPut))))
	{
		st_HDRParam.bIsClose = true;
		st_HDRParam.nHttpCode = 405;

		HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,发送的方法:%s 不支持"), lpszClientAddr, pSt_HTTPParam->tszHttpMethod);
		return false;
	}
	//连接数限制处理
	if (st_ServiceCfg.st_XLimit.nMaxUPConnect > 0)
	{
		if (!Session_UPStroage_MaxConnect(lpszClientAddr))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 503;

			HttpProtocol_Server_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPDOWNLOAD);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,请求失败,连接数超过限制:%d,无法继续"), lpszClientAddr, st_ServiceCfg.st_XLimit.nMaxUPConnect);
			return false;
		}
	}
	//用户验证
	if (st_ServiceCfg.st_XProxy.bUPPass)
	{
		XCHAR tszUserName[64];
		XCHAR tszUserPass[64];

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(tszUserPass, '\0', sizeof(tszUserPass));
		if (!APIHelp_Api_ProxyAuth(tszUserName, tszUserPass, pptszListHdr, nHdrCount))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.bAuth = true;
			st_HDRParam.nHttpCode = 401;

			HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,用户验证失败,错误:%lX"), lpszClientAddr, StorageHelp_GetLastError());
			return false;
		}
		int nCode = 0;
		int nResponseCode = 0;
		XCHAR* ptszBody = NULL;

		Protocol_StoragePacket_BasicAuth(pSt_HTTPParam->tszHttpMethod, pSt_HTTPParam->tszHttpUri, lpszClientAddr, tszUserName, tszUserPass, tszSDBuffer, &nSDLen);
		APIClient_Http_Request(_X("POST"), st_ServiceCfg.st_XProxy.tszAuthPass, tszSDBuffer, &nResponseCode, &ptszBody, &nSDLen);
		if (200 != nResponseCode)
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.bAuth = true;
			st_HDRParam.nHttpCode = nResponseCode;

			HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,用户验证失败,用户名:%s,密码:%s,错误码:%d,错误内容:%s"), tszUserName, tszUserPass, tszUserPass, nResponseCode, ptszBody);
		}
		Protocol_StorageParse_SpeedLimit(ptszBody, nSDLen, &nCode, &nLimit);
		BaseLib_Memory_FreeCStyle((VOID**)&ptszBody);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端:%s,代理服务:%s 验证通过,用户名:%s,密码:%s"), lpszClientAddr, st_ServiceCfg.st_XProxy.tszAuthPass, tszUserName, tszUserPass);
		st_HDRParam.bAuth = true;
	}
	//使用重定向?
	if (st_LoadbalanceCfg.st_LBDistributed.nUPLoadMode > 0)
	{
		XCHAR tszHdrBuffer[1024];
		XCHAR tszStorageAddr[128];

		memset(tszHdrBuffer, '\0', sizeof(tszHdrBuffer));
		memset(tszStorageAddr, '\0', sizeof(tszStorageAddr));

		st_HDRParam.bIsClose = true;
		st_HDRParam.nHttpCode = 302;

		APIHelp_Distributed_RandomAddr(st_LoadbalanceCfg.st_LoadBalance.pStl_ListUPLoader, tszStorageAddr, st_LoadbalanceCfg.st_LBDistributed.nUPLoadMode);
		_xstprintf(tszHdrBuffer, _X("Location: %s%s\r\n"), tszStorageAddr, pSt_HTTPParam->tszHttpUri);

		HttpProtocol_Server_SendMsgEx(xhDLHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHdrBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端:%s,请求的文件被要求重定向到:%s%s"), lpszClientAddr, tszStorageAddr, pSt_HTTPParam->tszHttpUri);
		return true;
	}
	int nRVMode = 0;
	int nRVCount = 0;
	int nHDSize = 0;
	XCHAR tszFileName[MAX_PATH];
	XENGINE_STORAGEBUCKET st_StorageBucket;

	memset(tszFileName, '\0', MAX_PATH);
	memset(&st_StorageBucket, '\0', sizeof(XENGINE_STORAGEBUCKET));
	//解析参数
	XCHAR** pptszParamList;
	int nParamCount = 0;
	HttpProtocol_ServerHelp_GetParament(pSt_HTTPParam->tszHttpUri, &pptszParamList, &nParamCount);
	if (nParamCount < 1)
	{
		st_HDRParam.nHttpCode = 413;
		HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
		BaseLib_Memory_Free((XPPPMEM)&pptszParamList, nParamCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,请求上传文件失败,提供的参数:%s 不正确,错误：%lX"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, HttpProtocol_GetLastError());
		return false;
	}
	APIHelp_Api_UrlParse(&pptszParamList, nParamCount, tszFileName, st_StorageBucket.tszBuckKey);
	//是否指定了bucket
	//http://127.0.0.1:5102/api?filename=1.txt&storeagekey=storagekey1
	if (_tcsxlen(st_StorageBucket.tszBuckKey) > 0)
	{
		if (!APIHelp_Distributed_UPStorage(st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket, 4))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 413;

			HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,请求上传文件失败,可能BUCKET:% 不正确,错误：%lX"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, StorageHelp_GetLastError());
			return false;
		}
	}
	else
	{
		if (!APIHelp_Distributed_UPStorage(st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket, st_LoadbalanceCfg.st_LBLocation.nUPLoadMode))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 413;

			HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,请求上传文件失败,可能BUCKET:% 不正确,错误：%lX"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, StorageHelp_GetLastError());
			return false;
		}
	}
	//修正文件路径
	int nPathType = 0;
	_xstprintf(tszFileDir, _X("%s/%s"), st_StorageBucket.tszFilePath, tszFileName);
	BaseLib_String_GetPath(tszFileDir, &nPathType);
	BaseLib_String_FixPath(tszFileDir, nPathType);
	//得到上传大小
	XCHAR tszVluStr[8] = {};
	if (HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, _X("Content-Length"), tszVluStr))
	{
		if (0 == _ttxoi(tszVluStr))
		{
			if (st_StorageBucket.st_PermissionFlags.bUPReady)
			{
				//文件是否存在
				if (0 == _xtaccess(tszFileDir, 0) && !st_StorageBucket.st_PermissionFlags.bRewrite)
				{
					st_HDRParam.bIsClose = true;
					st_HDRParam.nHttpCode = 403;
					HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
					XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,准备上传文件:%s 失败,文件已经存在"), lpszClientAddr, tszFileDir);
					return true;
				}
				//文件是否可写
				FILE* pSt_File = _xtfopen(tszFileDir, _X("wb"));
				if (NULL == pSt_File)
				{
					st_HDRParam.bIsClose = true;
					st_HDRParam.nHttpCode = 403;
					HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
					XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,准备上传文件:%s 失败,创建文件失败"), lpszClientAddr, tszFileDir);
					return true;
				}
				fclose(pSt_File);

				st_HDRParam.bIsClose = false;
				st_HDRParam.nHttpCode = 201;
				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端:%s,准备上传文件:%s 成功"), lpszClientAddr, tszFileDir);
				return true;
			}
			else
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.nHttpCode = 403;
				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,准备上传文件:%s 失败,服务器关闭了UPReady,无法继续"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return true;
			}
		}
	}
	//开始处理文件
	if (!Session_UPStroage_Exist(lpszClientAddr))
	{
		int nPosStart = 0;
		int nPosEnd = 0;
		__int64x nPosCount = 0;
		//得到断点续传大小
		if (!APIHelp_Api_RangeFile(&nPosStart, &nPosEnd, &nPosCount, pptszListHdr, nHdrCount))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 411;
			HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,没有用户数据大小字段,无法继续"), lpszClientAddr);
			return false;
		}
		//是新的还是断点续传的
		if ((0 != nPosStart) || (0 != nPosEnd))
		{
			//是否启用了断点续传
			if (!st_ServiceCfg.st_XStorage.bResumable)
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.nHttpCode = 416;

				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,请求断点续传上传文件失败,服务端关闭了此功能,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
				return false;
			}
		}
		XCHAR tszTmpPath[MAX_PATH];
		memset(tszTmpPath, '\0', MAX_PATH);

		BaseLib_String_GetFileAndPath(tszFileDir, tszTmpPath);
		if (0 != _xtaccess(tszTmpPath, 0))
		{
			//不存在,是否允许创建?
			if (!st_StorageBucket.st_PermissionFlags.bCreateDir)
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.nHttpCode = 404;

				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,请求上传文件失败,上传文件夹不存在,权限错误,文件:%s"), lpszClientAddr, tszFileDir);
				return false;
			}
			SystemApi_File_CreateMutilFolder(tszTmpPath);
		}
		XHANDLE xhUPSpeed = NULL;
		if (nLimit > 0 || (st_ServiceCfg.st_XLimit.bLimitMode && st_ServiceCfg.st_XLimit.nMaxUPLoader > 0))
		{
			//处理限速情况
			XCHAR* ptszIPClient = (XCHAR*)malloc(MAX_PATH);
			if (NULL == ptszIPClient)
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.nHttpCode = 500;

				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,插入用户请求失败,文件:%s,内存申请失败,服务器错误"), lpszClientAddr, tszFileDir);
				return false;
			}
			memset(ptszIPClient, '\0', MAX_PATH);
			_tcsxcpy(ptszIPClient, lpszClientAddr);

			nLimit = nLimit == 0 ? st_ServiceCfg.st_XLimit.nMaxUPLoader : nLimit;
			xhUPSpeed = Algorithm_Calculation_Create();
			Algorithm_Calculation_PassiveOPen(xhUPSpeed, XEngine_UPLoader_UPFlow, nLimit, 0, 0, false, ptszIPClient);
			NetCore_TCPXCore_PasueRecvEx(xhNetUPLoader, lpszClientAddr, false);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端:%s,上传限速被启用,文件:%s,限速:%d"), lpszClientAddr, tszFileDir, nLimit);
		}
		if (!Session_UPStroage_Insert(lpszClientAddr, st_StorageBucket.tszBuckKey, tszFileDir, xhUPSpeed, nPosCount, st_StorageBucket.st_PermissionFlags.bRewrite, nLimit, nPosStart, nPosEnd))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 500;

			HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,插入用户请求失败,文件:%s,错误：%lX"), lpszClientAddr, tszFileDir, Session_GetLastError());
			return false;
		}
		//检查上传模式
		XCHAR tszBoundStr[MAX_PATH] = {};
		if (APIHelp_Api_Boundary(&pptszListHdr, nHdrCount, tszBoundStr))
		{
			Session_UPStroage_SetBoundary(lpszClientAddr, tszBoundStr);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端:%s,请求设置了数据范围:%d - %d/%lld"), lpszClientAddr, nPosStart, nPosEnd, nPosCount);
	}
	SESSION_STORAGEINFO st_StorageInfo = {};
	Session_UPStroage_GetInfo(lpszClientAddr, &st_StorageInfo);
	//处理bound上传模式
	if (st_StorageInfo.st_Boundary.bBoundMode)
	{
		//是否开始已经处理
		if (st_StorageInfo.st_Boundary.bBoundStart)
		{
			//已处理,查找结尾
			int nPos = 0;
			if (Algorithm_String_XFastMatch(lpszMsgBuffer, st_StorageInfo.st_Boundary.tszBoundStr, &nPos, nMsgLen))
			{
				int nRLen = _tcsxlen(st_StorageInfo.st_Boundary.tszBoundStr) + 3;
				Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nPos - nRLen);
			}
			else
			{
				Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nMsgLen);
			}
		}
		else
		{
			//没有处理,查找开始
			int nPosStart = 0;
			int nPosEnd = 0;
			if (Algorithm_String_XFastMatch(lpszMsgBuffer, _X("\r\n\r\n"), &nPosStart, nMsgLen))
			{
				if (Algorithm_String_XFastMatch(lpszMsgBuffer + nPosStart, st_StorageInfo.st_Boundary.tszBoundStr, &nPosEnd, nMsgLen - nPosStart))
				{
					int nRLen = _tcsxlen(st_StorageInfo.st_Boundary.tszBoundStr) + 3;
					Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer + nPosStart, nPosEnd - nRLen);
				}
				else
				{
					Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer + nPosStart, nMsgLen - nPosStart);
				}
				Session_UPStroage_SetBoundaryStart(lpszClientAddr);
			}
		}
	}
	else
	{
		Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nMsgLen);
	}
	Algorithm_Calculation_ADDSDFlow(Session_UPStroage_GetSpeed(lpszClientAddr), nMsgLen);
	HttpProtocol_Server_GetRecvModeEx(xhUPHttp, lpszClientAddr, &nRVMode, &nRVCount, &nHDSize);
	if (nHDSize >= nRVCount)
	{
		int nPLen = MAX_PATH;
		XCHAR tszPassNotify[MAX_PATH] = {};

		Session_UPStroage_GetInfo(lpszClientAddr, &st_StorageInfo);
		if (!st_StorageInfo.st_Boundary.bBoundMode)
		{
			//大小是否足够
			if (st_StorageInfo.ullCount != st_StorageInfo.ullRWLen)
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.nHttpCode = 200;
				Protocol_StoragePacket_UPDown(tszPassNotify, &nPLen, st_StorageInfo.tszBuckKey, st_StorageInfo.tszFileDir, st_StorageInfo.tszClientAddr, st_StorageInfo.ullRWLen, false);
				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPassNotify, nPLen);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("上传客户端:%s,请求上传文件成功,文件名:%s,总大小:%lld,写入大小:%lld,文件不完整,需要等待断点续传完毕"), lpszClientAddr, tszFileDir, st_StorageInfo.ullCount, st_StorageInfo.ullRWLen);
				return true;
			}
		}
		int nHashLen = 0;
		XBYTE tszHashStr[MAX_PATH];
		XSTORAGECORE_DBFILE st_ProtocolFile;

		memset(tszHashStr, '\0', MAX_PATH);
		memset(&st_ProtocolFile, '\0', sizeof(XSTORAGECORE_DBFILE));

		_tcsxcpy(st_ProtocolFile.st_ProtocolFile.tszFilePath, st_StorageBucket.tszFilePath);
		_tcsxcpy(st_ProtocolFile.st_ProtocolFile.tszFileName, tszFileName);
		_tcsxcpy(st_ProtocolFile.tszBuckKey, st_StorageBucket.tszBuckKey);
		st_ProtocolFile.st_ProtocolFile.nFileSize = st_StorageInfo.ullRWLen;
		//上传完毕需要关闭,否则计算HASH会不正常
		Session_UPStroage_Close(lpszClientAddr);
		Cryption_Api_Digest(tszFileDir, tszHashStr, &nHashLen, true, st_ServiceCfg.st_XStorage.nHashMode);
		BaseLib_String_StrToHex((char*)tszHashStr, nHashLen, st_ProtocolFile.st_ProtocolFile.tszFileHash);
		//处理结果
		if (st_ServiceCfg.st_XStorage.bUPHash)
		{
			if (APIHelp_Api_VerHash(st_ProtocolFile.st_ProtocolFile.tszFileHash, pptszListHdr, nHdrCount))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,上传的文件信息HASH效验成功,文件:%s HASH:%s"), lpszClientAddr, tszFileDir, st_ProtocolFile.st_ProtocolFile.tszFileHash);
			}
			else
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.nHttpCode = 403;
				_xtremove(tszFileDir);
				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,上传的文件信息HASH校验失败,无法继续,文件:%s 已被删除,HASH:%s,错误:%lX"), lpszClientAddr, tszFileDir, st_ProtocolFile.st_ProtocolFile.tszFileHash, StorageHelp_GetLastError());
				return false;
			}
		}
		Protocol_StoragePacket_UPDown(tszPassNotify, &nPLen, st_StorageInfo.tszBuckKey, st_StorageInfo.tszFileDir, st_StorageInfo.tszClientAddr, st_StorageInfo.ullRWLen, false, st_ProtocolFile.st_ProtocolFile.tszFileHash);
		//PASS代理
		if (st_ServiceCfg.st_XProxy.bUPPass)
		{
			int nHttpCode = 0;
			if (!APIClient_Http_Request(_X("POST"), st_ServiceCfg.st_XProxy.tszUPPass, tszPassNotify, &nHttpCode))
			{
				_xtremove(st_StorageInfo.tszFileDir);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("上传客户端:%s,请求完成通知失败,可能服务器不正确:文件:%s,地址:%s"), lpszClientAddr, st_StorageInfo.tszFileDir, st_ServiceCfg.st_XProxy.tszUPPass);
				return false;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端:%s,请求完成通知返回值:%d,文件:%s,地址:%s"), lpszClientAddr, nHttpCode, st_StorageInfo.tszFileDir, st_ServiceCfg.st_XProxy.tszUPPass);
		}
		//数据库写
		if (st_ServiceCfg.st_XSql.bEnable)
		{
			if (Database_File_FileInsert(&st_ProtocolFile, st_StorageInfo.bRewrite))
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.nHttpCode = 200;
				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPassNotify, nPLen);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端:%s,请求上传文件成功,文件名:%s,大小:%d"), lpszClientAddr, tszFileDir, nRVCount);
			}
			else
			{
				st_HDRParam.bIsClose = true;
				st_HDRParam.nHttpCode = 403;
				_xtremove(st_StorageInfo.tszFileDir);
				HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPassNotify, nPLen);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("上传客户端:%s,请求上传文件失败,插入数据库失败:%s,错误:%lX"), lpszClientAddr, tszFileDir, Database_GetLastError());
			}
		}
		else
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 200;
			HttpProtocol_Server_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPassNotify, nPLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPUPLOADER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("上传客户端:%s,请求上传文件成功,文件名:%s,大小:%d,数据库没有启用,不插入数据库"), lpszClientAddr, tszFileDir, nRVCount);
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("上传客户端:%s,请求上传文件中,文件名:%s,大小:%d"), lpszClientAddr, tszFileDir, nMsgLen);
	}
	
	return true;
}