#include "../StorageApp_Hdr.h"

XHTHREAD Session_Action_Thread()
{
	while (bIsRun)
	{
		int nListCount = 0;
		XNETHANDLE** ppxhToken;
		Session_Action_GetAll(&ppxhToken, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XCLIENT_APIFILE st_TaskInfo = {};
			XHANDLE xhAction = Session_Action_GetToken((*ppxhToken)[i]);
			
			if (!APIClient_File_Query(xhAction, &st_TaskInfo))
			{
				Session_Action_Delete((*ppxhToken)[i]);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("转录动作线程:查询文件传输信息失败,句柄:%lld,错误:%lX"), (*ppxhToken)[i], Session_GetLastError());
				continue;
			}
			XENGINE_ACTIONINFO st_ActionInfo = {};
			Session_Action_GetInfo((*ppxhToken)[i], &st_ActionInfo);
			if (ENUM_XCLIENT_APIHELP_FILE_STATUS_COMPLETE == st_TaskInfo.en_DownStatus)
			{
				APIClient_File_Delete(xhAction);
				Session_Action_Delete((*ppxhToken)[i]);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("转录动作线程:完成文件转录下载,句柄:%lld,URL:%s,Bucket:%s,FileName:%s"), (*ppxhToken)[i], st_ActionInfo.tszFileUrl, st_ActionInfo.tszBucketStr, st_ActionInfo.tszFileName);
			}
			else if (ENUM_XCLIENT_APIHELP_FILE_STATUS_DOWNLOADDING == st_TaskInfo.en_DownStatus || ENUM_XCLIENT_APIHELP_FILE_STATUS_INIT == st_TaskInfo.en_DownStatus)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("转录动作线程:文件转录进行中,句柄:%lld,URL:%s,Bucket:%s,FileName:%s,大小:%lf,已处理大小:%lf"), (*ppxhToken)[i], st_ActionInfo.tszFileUrl, st_ActionInfo.tszBucketStr, st_ActionInfo.tszFileName, st_TaskInfo.dlTotal, st_TaskInfo.dlNow);
			}
			else
			{
				APIClient_File_Delete(xhAction);
				Session_Action_Delete((*ppxhToken)[i]);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("转录动作线程:文件转录失败,句柄:%lld,URL:%s,Bucket:%s,FileName:%s,错误:%lX"), (*ppxhToken)[i], st_ActionInfo.tszFileUrl, st_ActionInfo.tszBucketStr, st_ActionInfo.tszFileName, Session_GetLastError());
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppxhToken, nListCount);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
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
	_stprintf(tszFileName, _X("%s/%s"), st_StorageBucket.tszFilePath, st_ActionInfo.tszFileName);
	if (0 == _tcsxnicmp(lpszAPIDownload, lpszAPIName, _tcsxlen(lpszAPIDownload)))
	{
		XNETHANDLE xhToken = 0;
		BaseLib_OperatorHandle_Create(&xhToken);

		XHANDLE xhAction = APIClient_File_Create(st_ActionInfo.tszFileUrl, tszFileName, true);
		if (NULL == xhAction)
		{
			st_HDRParam.nHttpCode = 501;
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理用户转录动作失败,下载文件失败,URL:%s,路径:%s,Bucket:%s"), lpszClientAddr, st_ActionInfo.tszFileUrl, tszFileName, st_ActionInfo.tszBucketStr);
			return false;
		}
		APIClient_File_Start(xhAction);
		//APIClient_File_SetMaxSpeed(xhAction);
		Session_Action_Insert(xhToken, xhAction, &st_ActionInfo);
		Protocol_StoragePacket_Action(tszRVBuffer, &nRVLen, xhToken, &st_ActionInfo);
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,处理用户下载动作成功,URL:%s,路径:%s,句柄:%lld"), lpszClientAddr, st_ActionInfo.tszFileUrl, tszFileName, xhToken);
	}
	else if (0 == _tcsxnicmp(lpszAPIUPload, lpszAPIName, _tcsxlen(lpszAPIUPload)))
	{
		XNETHANDLE xhToken = 0;
		BaseLib_OperatorHandle_Create(&xhToken);

		XHANDLE xhAction = APIClient_File_Create(st_ActionInfo.tszFileUrl, tszFileName, false);
		if (NULL == xhAction)
		{
			st_HDRParam.nHttpCode = 501;
			HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("业务客户端:%s,处理用户转录动作失败,下载文件失败,URL:%s,路径:%s,Bucket:%s"), lpszClientAddr, st_ActionInfo.tszFileUrl, tszFileName, st_ActionInfo.tszBucketStr);
			return false;
		}
		APIClient_File_Start(xhAction);
		//APIClient_File_SetMaxSpeed(xhAction);
		Session_Action_Insert(xhToken, xhAction, &st_ActionInfo);
		Protocol_StoragePacket_Action(tszRVBuffer, &nRVLen, xhToken, &st_ActionInfo);
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,处理用户上传动作成功,URL:%s,路径:%s,句柄:%lld"), lpszClientAddr, st_ActionInfo.tszFileUrl, tszFileName, xhToken);
	}
	
	return true;
}