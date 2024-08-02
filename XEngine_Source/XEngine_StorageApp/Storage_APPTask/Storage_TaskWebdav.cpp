#include "../StorageApp_Hdr.h"

bool Storage_TaskWebdav(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 10240;
	int nRVLen = 10240;
	XCHAR tszSDBuffer[10240] = {};
	XCHAR tszRVBuffer[10240] = {};
	LPCXSTR lpszAPIPropfind = _X("PROPFIND");
	LPCXSTR lpszAPIUPload = _X("upload");

	XENGINE_ACTIONINFO st_ActionInfo = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	st_HDRParam.bIsClose = false;
	st_HDRParam.nHttpCode = 200;
	
	if (0 == _tcsxnicmp(lpszAPIPropfind, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszAPIPropfind)))
	{
		if (_tcsxlen(pSt_HTTPParam->tszHttpUri) <= 1)
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 403;
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理WEBDAV协议PROPFIND方法失败,地址不正确,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		XENGINE_STORAGEBUCKET st_StorageBucket = {};
		if (!APIHelp_Distributed_CTStorage(pSt_HTTPParam->tszHttpUri + 1, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
		{
			st_HDRParam.bIsClose = true;
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理WEBDAV协议PROPFIND方法失败,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		XCHAR tszFindStr[MAX_PATH] = {};
		//得到是否是文件
		if (APIHelp_Api_UrlStr(st_StorageBucket.tszBuckKey, pSt_HTTPParam->tszHttpUri))
		{
			int nALen = 0;
			_tcsxcpy(tszFindStr, pSt_HTTPParam->tszHttpUri + 1);
			APIHelp_Api_UrlChange(tszFindStr, st_StorageBucket.tszBuckKey, st_StorageBucket.tszFilePath);
		}
		else
		{
			//得到深度
			XCHAR tszVluStr[8] = {};
			HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, _X("Depth"), tszVluStr);
			if (1 == _ttxoi(tszVluStr))
			{
				_xstprintf(tszFindStr, _X("%s/*"), st_StorageBucket.tszFilePath);
			}
			else
			{
				_tcsxcpy(tszFindStr, st_StorageBucket.tszFilePath);
			}
		}
		//枚举文件
		int nListCount = 0;
		XCHAR** pptszListFile;
		SystemApi_File_EnumFile(tszFindStr, &pptszListFile, &nListCount);
		if (0 == nListCount)
		{
			st_HDRParam.nHttpCode = 404;
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理WEBDAV协议PROPFIND方法失败,文件没有找到,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		Protocol_StoragePacket_Propfind(tszRVBuffer, &nRVLen, &pptszListFile, nListCount);
		nRVLen -= 1;

		st_HDRParam.nHttpCode = 207;
		_tcsxcpy(st_HDRParam.tszMimeType, _X("xml"));
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理WEBDAV协议PROPFIND方法成功,文件名称:%s"), lpszClientAddr, tszFindStr);
		return false;
	}
	
	return true;
}