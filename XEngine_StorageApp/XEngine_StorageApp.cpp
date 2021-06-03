#include "StorageApp_Hdr.h"

BOOL bIsRun = FALSE;
XLOG xhLog = NULL;

XNETHANDLE xhHBDownload = 0;
XNETHANDLE xhHBUPLoader = 0;
XNETHANDLE xhNetDownload = 0;
XNETHANDLE xhNetUPLoader = 0;
XNETHANDLE xhUPPool = 0;
XNETHANDLE xhDLPool = 0;
XHANDLE xhUPHttp = NULL;
XHANDLE xhDLHttp = NULL;

XENGINE_SERVERCONFIG st_ServiceCfg;

void ServiceApp_Stop(int signo)
{
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("存储中心服务器退出..."));
	bIsRun = FALSE;

	NetCore_TCPXCore_DestroyEx(xhNetDownload);
	NetCore_TCPXCore_DestroyEx(xhNetUPLoader);
	SocketOpt_HeartBeat_DestoryEx(xhHBDownload);
	SocketOpt_HeartBeat_DestoryEx(xhHBUPLoader);
	ManagePool_Thread_NQDestroy(xhUPPool);
	ManagePool_Thread_NQDestroy(xhDLPool);
	RfcComponents_HttpServer_DestroyEx(xhUPHttp);
	RfcComponents_HttpServer_DestroyEx(xhDLHttp);
	HelpComponents_XLog_Destroy(xhLog);
	Session_DLStroage_Destory();
	exit(0);
}
static int ServiceApp_Deamon(int wait)
{
#ifndef _WINDOWS
	pid_t pid = 0;
	int status;
	pid = fork();
	if (pid > 0)
	{
		exit(0);
	}

	close(2);
	while (1)
	{

		pid = fork();
		if (pid < 0)
			exit(1);
		if (pid == 0)
		{
			return 0;
		}
		waitpid(pid, &status, 0);

		if (wait > 0)
			sleep(wait);
	}
#endif
	return 0;
}

int main(int argc, char** argv)
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	bIsRun = TRUE;
	tstring m_StrVersion;
	LPCTSTR lpszHTTPMime = _T("./XEngine_Config/HttpMime.types");
	LPCTSTR lpszHTTPCode = _T("./XEngine_Config/HttpCode.types");
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	THREADPOOL_PARAMENT** ppSt_ListUPThread;
	THREADPOOL_PARAMENT** ppSt_ListDLThread;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceCfg, '\0', sizeof(XENGINE_SERVERCONFIG));

	if (!StorageApp_Config_Parament(argc, argv, &st_ServiceCfg))
	{
		return -1;
	}
	st_XLogConfig.XLog_MaxBackupFile = st_ServiceCfg.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_ServiceCfg.st_XLog.nMaxSize;
	_tcscpy(st_XLogConfig.tszFileName, _T("./XEngine_XLog/XEngine_StorageApp.log"));

	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务器失败，启动日志失败，错误：%lX", XLog_GetLastError());
		goto XENGINE_EXITAPP;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_ServiceCfg.st_XLog.nLogLeave);

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化日志系统成功"));
	if (st_ServiceCfg.bDeamon)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，使用守护进程启动服务..."));
		ServiceApp_Deamon(1);
	}

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化服务器信号管理成功"));

	if (st_ServiceCfg.st_XTime.bHBTime)
	{
		if (!SocketOpt_HeartBeat_InitEx(&xhHBDownload, st_ServiceCfg.st_XTime.nStorageTimeOut, st_ServiceCfg.st_XTime.nTimeCheck, XEngine_Callback_HBDownload))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化下载心跳管理服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化下载心跳管理服务成功，句柄：%llu,时间:%d,次数:%d"), xhHBDownload, st_ServiceCfg.st_XTime.nStorageTimeOut, st_ServiceCfg.st_XTime.nTimeCheck);

		if (!SocketOpt_HeartBeat_InitEx(&xhHBUPLoader, st_ServiceCfg.st_XTime.nStorageTimeOut, st_ServiceCfg.st_XTime.nTimeCheck, XEngine_Callback_HBUPLoader))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化上传心跳管理服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化上传心跳管理服务成功，句柄：%llu,时间:%d,次数:%d"), xhHBUPLoader, st_ServiceCfg.st_XTime.nStorageTimeOut, st_ServiceCfg.st_XTime.nTimeCheck);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中，心跳管理服务配置为不启用..."));
	}

	xhDLHttp = RfcComponents_HttpServer_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceCfg.st_XMax.nStorageDLThread);
	if (NULL == xhDLHttp)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，初始化HTTP下载服务失败，错误：%lX"), HttpServer_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化HTTP下载服务成功，IO线程个数:%d"), st_ServiceCfg.st_XMax.nStorageDLThread);
	xhUPHttp = RfcComponents_HttpServer_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceCfg.st_XMax.nStorageUPThread);
	if (NULL == xhDLHttp)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，初始化HTTP上传服务失败，错误：%lX"), HttpServer_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化HTTP上传服务成功，IO线程个数:%d"), st_ServiceCfg.st_XMax.nStorageUPThread);

	if (!NetCore_TCPXCore_StartEx(&xhNetDownload, st_ServiceCfg.nStorageDLPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动下载存储网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nStorageDLPort, NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动下载存储网络服务成功，句柄：%llu，端口：%d,IO线程个数:%d"), xhNetDownload, st_ServiceCfg.nStorageDLPort, st_ServiceCfg.st_XMax.nIOThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhNetDownload, XEngine_Callback_DownloadLogin, XEngine_Callback_DownloadRecv, XEngine_Callback_DownloadLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册下载存储网络服务事件成功！"));

	if (!NetCore_TCPXCore_StartEx(&xhNetUPLoader, st_ServiceCfg.nStorageDLPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动上传存储网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nStorageUPPort, NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动上传存储网络服务成功，句柄：%llu，端口：%d,IO线程个数:%d"), xhNetUPLoader, st_ServiceCfg.nStorageDLPort, st_ServiceCfg.st_XMax.nIOThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhNetUPLoader, XEngine_Callback_UPLoaderLogin, XEngine_Callback_UPLoaderRecv, XEngine_Callback_UPLoaderLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册上传存储网络服务事件成功！"));

	if (!Session_DLStroage_Init(st_ServiceCfg.st_XMax.nStorageDLThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动下载会话服务失败，错误：%lX"), NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动下载会话服务成功"));

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListDLThread, st_ServiceCfg.st_XMax.nStorageDLThread, sizeof(THREADPOOL_PARAMENT));
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListUPThread, st_ServiceCfg.st_XMax.nStorageUPThread, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < st_ServiceCfg.st_XMax.nStorageDLThread; i++)
	{
		int* pInt_Pos = new int;
		*pInt_Pos = i;

		ppSt_ListDLThread[i]->lParam = pInt_Pos;
		ppSt_ListDLThread[i]->fpCall_ThreadsTask = XEngine_Download_Thread;
	}
	if (!ManagePool_Thread_NQCreate(&xhDLPool, &ppSt_ListDLThread, st_ServiceCfg.st_XMax.nStorageDLThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动任务处理线程池失败，错误：%d"), errno);
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动下载任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nStorageDLThread);

	m_StrVersion = st_ServiceCfg.st_XVer.pStl_ListStorage->front();
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动，存储中心服务运行中，发行版本次数:%d,当前运行版本：%s。。。"), st_ServiceCfg.st_XVer.pStl_ListStorage->size(), m_StrVersion.c_str());
	while (bIsRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_EXITAPP:
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("存储中心服务关闭，服务器退出..."));
	bIsRun = FALSE;

	NetCore_TCPXCore_DestroyEx(xhNetDownload);
	NetCore_TCPXCore_DestroyEx(xhNetUPLoader);
	SocketOpt_HeartBeat_DestoryEx(xhHBDownload);
	SocketOpt_HeartBeat_DestoryEx(xhHBUPLoader);
	ManagePool_Thread_NQDestroy(xhUPPool);
	ManagePool_Thread_NQDestroy(xhDLPool);
	RfcComponents_HttpServer_DestroyEx(xhUPHttp);
	RfcComponents_HttpServer_DestroyEx(xhDLHttp);
	HelpComponents_XLog_Destroy(xhLog);
	Session_DLStroage_Destory();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
