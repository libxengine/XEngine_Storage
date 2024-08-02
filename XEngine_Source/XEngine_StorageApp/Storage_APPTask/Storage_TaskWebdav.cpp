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

	st_HDRParam.bIsClose = true;
	st_HDRParam.nHttpCode = 200;
	
	if (0 == _tcsxnicmp(lpszAPIPropfind, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszAPIPropfind)))
	{
		XENGINE_STORAGEBUCKET st_StorageBucket = {};
		if (!APIHelp_Distributed_CTStorage(pSt_HTTPParam->tszHttpUri + 1, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理WEBDAV协议PROPFIND方法失败,URL:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
			return false;
		}
		int nListCount = 0;
		XCHAR** pptszListFile;

		XCHAR tszVluStr[8] = {};
		HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, _X("Depth"), tszVluStr);

		if (1 == _ttxoi(tszVluStr))
		{
			_tcsxcat(st_StorageBucket.tszFilePath, _X("/*"));
		}
		
		SystemApi_File_EnumFile(st_StorageBucket.tszFilePath, &pptszListFile, &nListCount);
		Protocol_StoragePacket_Propfind(tszRVBuffer, &nRVLen, &pptszListFile, nListCount);
		nRVLen -= 1;

		st_HDRParam.nHttpCode = 207;
		_tcsxcpy(st_HDRParam.tszMimeType, _X("xml"));

		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理WEBDAV协议PROPFIND方法成功,KEY名称:%s"), lpszClientAddr, st_StorageBucket.tszFileName);
		return false;
	}
	
	return true;
}