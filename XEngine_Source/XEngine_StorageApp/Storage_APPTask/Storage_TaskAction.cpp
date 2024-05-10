#include "../StorageApp_Hdr.h"

void CALLBACK Storage_TaskAction_Callback(XHANDLE xhToken, double dlTotal, double dlNow, double ulTotal, double ulNow, ENUM_XCLIENT_APIHELP_FILE_STATUS en_DownHttpStatus, XPVOID lParam)
{

}
bool Storage_TaskAction(LPCXSTR lpszAPIName, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam)
{
	int nSDLen = 10240;
	int nRVLen = 10240;
	XCHAR tszSDBuffer[10240] = {};
	XCHAR tszRVBuffer[10240] = {};
	LPCXSTR lpszAPIDownload = _X("download");
	LPCXSTR lpszAPIUPload = _X("upload");
	XENGINE_ACTIONINFO st_ActionInfo = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	st_HDRParam.bIsClose = true;
	st_HDRParam.nHttpCode = 200;

	if (!Protocol_StorageParse_Action(lpszMsgBuffer, nMsgLen, &st_ActionInfo))
	{
		st_HDRParam.nHttpCode = 400;
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理用户转录动作失败,协议解析失败,负载内容:%s"), lpszClientAddr, lpszMsgBuffer);
		return false;
	}
	XCHAR tszFileName[MAX_PATH] = {};
	XENGINE_STORAGEBUCKET st_StorageBucket = {};
	if (!APIHelp_Distributed_CTStorage(st_ActionInfo.tszBucketStr, st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, &st_StorageBucket))
	{
		st_HDRParam.nHttpCode = 400;
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理用户转录动作失败,存储Key解析失败,URL:%s,路径:%s,Bucket:%s"), lpszClientAddr, st_ActionInfo.tszFileUrl, st_ActionInfo.tszFileName, st_ActionInfo.tszBucketStr);
		return false;
	}
	if (0 == _tcsxnicmp(lpszAPIDownload, lpszAPIName, _tcsxlen(lpszAPIDownload)))
	{
		XHANDLE xhAction = APIClient_File_Create(st_ActionInfo.tszFileName, tszFileName, true, NULL, Storage_TaskAction_Callback);
		if (NULL == xhAction)
		{
			st_HDRParam.nHttpCode = 501;
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理用户转录动作失败,下载文件失败,URL:%s,路径:%s,Bucket:%s"), lpszClientAddr, st_ActionInfo.tszFileUrl, tszFileName, st_ActionInfo.tszBucketStr);
			return false;
		}
		//APIClient_File_SetMaxSpeed(xhAction);
		XNETHANDLE xhToken = 0;
		BaseLib_OperatorHandle_Create(&xhToken);
		Session_Action_Insert(xhToken, xhAction, &st_ActionInfo);
		Protocol_StoragePacket_Action(tszRVBuffer, &nRVLen, xhToken, &st_ActionInfo);
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,处理用户下载动作成功,URL:%s,路径:%s,句柄:%lld"), lpszClientAddr, st_ActionInfo.tszFileUrl, tszFileName, xhToken);
	}
	else if (0 == _tcsxnicmp(lpszAPIUPload, lpszAPIName, _tcsxlen(lpszAPIUPload)))
	{
		XHANDLE xhAction = APIClient_File_Create(st_ActionInfo.tszFileName, tszFileName, false, NULL, Storage_TaskAction_Callback);
		if (NULL == xhAction)
		{
			st_HDRParam.nHttpCode = 501;
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理用户转录动作失败,下载文件失败,URL:%s,路径:%s,Bucket:%s"), lpszClientAddr, st_ActionInfo.tszFileUrl, tszFileName, st_ActionInfo.tszBucketStr);
			return false;
		}
		//APIClient_File_SetMaxSpeed(xhAction);
		XNETHANDLE xhToken = 0;
		BaseLib_OperatorHandle_Create(&xhToken);
		Session_Action_Insert(xhToken, xhAction, &st_ActionInfo);
		Protocol_StoragePacket_Action(tszRVBuffer, &nRVLen, xhToken, &st_ActionInfo);
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,处理用户上传动作成功,URL:%s,路径:%s,句柄:%lld"), lpszClientAddr, st_ActionInfo.tszFileUrl, tszFileName, xhToken);
	}
	
	return true;
}