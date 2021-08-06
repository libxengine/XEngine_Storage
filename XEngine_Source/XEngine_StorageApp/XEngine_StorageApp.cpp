#include "StorageApp_Hdr.h"

BOOL bIsRun = FALSE;
XLOG xhLog = NULL;

XNETHANDLE xhHBDownload = 0;
XNETHANDLE xhHBUPLoader = 0;
XNETHANDLE xhHBP2xp = 0;

XNETHANDLE xhNetDownload = 0;
XNETHANDLE xhNetUPLoader = 0;
XNETHANDLE xhNetCenter = 0;
XNETHANDLE xhNetP2xp = 0;

XNETHANDLE xhUPPool = 0;
XNETHANDLE xhDLPool = 0;
XNETHANDLE xhSDPool = 0;
XNETHANDLE xhCTPool = 0;
XNETHANDLE xhP2XPPool = 0;

XHANDLE xhUPHttp = NULL;
XHANDLE xhDLHttp = NULL;
XHANDLE xhCenterHttp = NULL;
XNETHANDLE xhP2XPPacket = 0;

SOCKET hBroadSocket = 0;
shared_ptr<std::thread> pSTDThread = NULL;

XENGINE_SERVERCONFIG st_ServiceCfg;
XENGINE_LBCONFIG st_LoadbalanceCfg;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("存储中心服务器退出..."));
		bIsRun = FALSE;

		RfcComponents_HttpServer_DestroyEx(xhUPHttp);
		RfcComponents_HttpServer_DestroyEx(xhDLHttp);
		RfcComponents_HttpServer_DestroyEx(xhCenterHttp);
		HelpComponents_Datas_Destory(xhP2XPPacket);

		NetCore_TCPXCore_DestroyEx(xhNetDownload);
		NetCore_TCPXCore_DestroyEx(xhNetUPLoader);
		NetCore_TCPXCore_DestroyEx(xhNetCenter);
		NetCore_TCPXCore_DestroyEx(xhNetP2xp);

		SocketOpt_HeartBeat_DestoryEx(xhHBDownload);
		SocketOpt_HeartBeat_DestoryEx(xhHBUPLoader);
		SocketOpt_HeartBeat_DestoryEx(xhHBP2xp);

		ManagePool_Thread_NQDestroy(xhUPPool);
		ManagePool_Thread_NQDestroy(xhDLPool);
		ManagePool_Thread_NQDestroy(xhSDPool);
		ManagePool_Thread_NQDestroy(xhCTPool);
		ManagePool_Thread_NQDestroy(xhP2XPPool);

		HelpComponents_XLog_Destroy(xhLog);

		Session_User_Destory();
		Session_DLStroage_Destory();
		Session_UPStroage_Destory();
		XStorageSQL_Destory();
		XStorage_SQLite_Destory();

		if (NULL != pSTDThread)
		{
			NetCore_BroadCast_Close(hBroadSocket);
			pSTDThread->join();
		}
		exit(0);
	}
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
	THREADPOOL_PARAMENT** ppSt_ListSDThread;
	THREADPOOL_PARAMENT** ppSt_ListCTThread;
	THREADPOOL_PARAMENT** ppSt_ListP2xpThread;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceCfg, '\0', sizeof(XENGINE_SERVERCONFIG));
	memset(&st_LoadbalanceCfg, '\0', sizeof(XENGINE_LBCONFIG));

	if (!StorageApp_Config_Parament(argc, argv))
	{
		return -1;
	}
	
	if (st_ServiceCfg.st_Memory.bReload)
	{
		//重载配置文件后退出
		TCHAR tszAddr[128];
		memset(tszAddr, '\0', sizeof(tszAddr));

		_stprintf(tszAddr, _T("Http://127.0.0.1:%d/Api/Event/Config"), st_ServiceCfg.nCenterPort);
		APIHelp_HttpRequest_Post(tszAddr);
		return 0;
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

	if (!NetXApi_Address_OpenQQWry(st_ServiceCfg.st_P2xp.tszQQWryFile))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化IP地址数据库失败,错误:%lX"), NetXApi_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化IP地址数据库成功"));

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

		if (!SocketOpt_HeartBeat_InitEx(&xhHBP2xp, st_ServiceCfg.st_XTime.nP2XPTimeOut, st_ServiceCfg.st_XTime.nTimeCheck, XEngine_Callback_HBP2xp))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化P2XP心跳管理服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化P2XP心跳管理服务成功，句柄：%llu,时间:%d,次数:%d"), xhHBP2xp, st_ServiceCfg.st_XTime.nP2XPTimeOut, st_ServiceCfg.st_XTime.nTimeCheck);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中，心跳管理服务配置为不启用..."));
	}

	if (1 == st_ServiceCfg.st_XSql.nSQLType)
	{
		if (!XStorageSQL_Init((DATABASE_MYSQL_CONNECTINFO*)&st_ServiceCfg.st_XSql, st_ServiceCfg.st_XTime.nDBMonth))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化MYSQL数据库服务失败，错误：%lX"), XStorageDB_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化MYSQL数据库服务成功"));
	}
	else if (2 == st_ServiceCfg.st_XSql.nSQLType)
	{
		if (!XStorage_SQLite_Init(st_ServiceCfg.st_XSql.tszSQLFile, st_ServiceCfg.st_XTime.nDBMonth))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化SQLITE数据库服务失败，错误：%lX"), XStorageDB_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化SQLITE数据库服务成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中，数据库被设置为不启用"));
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
	xhCenterHttp = RfcComponents_HttpServer_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceCfg.st_XMax.nCenterThread);
	if (NULL == xhCenterHttp)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，初始化HTTP业务服务失败，错误：%lX"), HttpServer_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化HTTP业务服务成功，IO线程个数:%d"), st_ServiceCfg.st_XMax.nCenterThread);

	if (!HelpComponents_Datas_Init(&xhP2XPPacket, st_ServiceCfg.st_XMax.nMaxQueue, 0, st_ServiceCfg.st_XMax.nP2XPThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，初始化P2XP包管理器失败，错误：%lX"), Packets_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动P2XP组包器成功"));

	if (!Session_User_Init(st_ServiceCfg.st_XProxy.st_XProxyAuth.tszUserList))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动用户管理服务失败，错误：%lX"), Session_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动用户管理服务成功"));
	if (!Session_DLStroage_Init(st_ServiceCfg.st_XMax.nStorageDLThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动下载会话服务失败，错误：%lX"), Session_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动下载会话服务成功"));
	if (!Session_UPStroage_Init())
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动上传会话服务失败，错误：%lX"), Session_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动上传会话服务成功"));

	if (!NetCore_TCPXCore_StartEx(&xhNetDownload, st_ServiceCfg.nStorageDLPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动下载存储网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nStorageDLPort, NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动下载存储网络服务成功，句柄：%llu，端口：%d,IO线程个数:%d"), xhNetDownload, st_ServiceCfg.nStorageDLPort, st_ServiceCfg.st_XMax.nIOThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhNetDownload, XEngine_Callback_DownloadLogin, XEngine_Callback_DownloadRecv, XEngine_Callback_DownloadLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册下载存储网络服务事件成功！"));

	if (!NetCore_TCPXCore_StartEx(&xhNetUPLoader, st_ServiceCfg.nStorageUPPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动上传存储网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nStorageUPPort, NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动上传存储网络服务成功，句柄：%llu，端口：%d,IO线程个数:%d"), xhNetUPLoader, st_ServiceCfg.nStorageUPPort, st_ServiceCfg.st_XMax.nIOThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhNetUPLoader, XEngine_Callback_UPLoaderLogin, XEngine_Callback_UPLoaderRecv, XEngine_Callback_UPLoaderLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册上传存储网络服务事件成功！"));

	if (!NetCore_TCPXCore_StartEx(&xhNetCenter, st_ServiceCfg.nCenterPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动业务控制存储网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nCenterPort, NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动业务控制存储网络服务成功，句柄：%llu，端口：%d,IO线程个数:%d"), xhNetCenter, st_ServiceCfg.nCenterPort, st_ServiceCfg.st_XMax.nIOThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhNetCenter, XEngine_Callback_CenterLogin, XEngine_Callback_CenterRecv, XEngine_Callback_CenterLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册业务控制存储存储网络服务事件成功！"));

	if (!NetCore_TCPXCore_StartEx(&xhNetP2xp, st_ServiceCfg.nP2XPPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nP2XPThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动P2XP网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nP2XPPort, NetCore_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动P2XP网络服务成功，句柄：%llu，端口：%d,IO线程个数:%d"), xhNetP2xp, st_ServiceCfg.nP2XPPort, st_ServiceCfg.st_XMax.nP2XPThread);
	NetCore_TCPXCore_RegisterCallBackEx(xhNetP2xp, XEngine_Callback_P2xpLogin, XEngine_Callback_P2xpRecv, XEngine_Callback_P2xpLeave);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册P2XP网络服务事件成功！"));

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListDLThread, st_ServiceCfg.st_XMax.nStorageDLThread, sizeof(THREADPOOL_PARAMENT));
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListSDThread, st_ServiceCfg.st_XMax.nStorageDLThread, sizeof(THREADPOOL_PARAMENT));
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListUPThread, st_ServiceCfg.st_XMax.nStorageUPThread, sizeof(THREADPOOL_PARAMENT));
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListCTThread, st_ServiceCfg.st_XMax.nCenterThread, sizeof(THREADPOOL_PARAMENT));
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListP2xpThread, st_ServiceCfg.st_XMax.nP2XPThread, sizeof(THREADPOOL_PARAMENT));
	for (int i = 0; i < st_ServiceCfg.st_XMax.nStorageDLThread; i++)
	{
		int* pInt_Pos = new int;
		*pInt_Pos = i;

		ppSt_ListDLThread[i]->lParam = pInt_Pos;
		ppSt_ListDLThread[i]->fpCall_ThreadsTask = XEngine_Download_HTTPThread;

		ppSt_ListSDThread[i]->lParam = pInt_Pos;
		ppSt_ListSDThread[i]->fpCall_ThreadsTask = XEngine_Download_SendThread;
	}
	if (!ManagePool_Thread_NQCreate(&xhDLPool, &ppSt_ListDLThread, st_ServiceCfg.st_XMax.nStorageDLThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动HTTP下载处理线程池失败，错误：%d"), errno);
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动HTTP下载任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nStorageDLThread);
	if (!ManagePool_Thread_NQCreate(&xhSDPool, &ppSt_ListSDThread, st_ServiceCfg.st_XMax.nStorageDLThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动下载任务线程池失败，错误：%d"), errno);
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动下载任务线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nStorageDLThread);

	for (int i = 0; i < st_ServiceCfg.st_XMax.nStorageUPThread; i++)
	{
		int* pInt_Pos = new int;
		*pInt_Pos = i;

		ppSt_ListUPThread[i]->lParam = pInt_Pos;
		ppSt_ListUPThread[i]->fpCall_ThreadsTask = XEngine_UPLoader_HTTPThread;
	}
	if (!ManagePool_Thread_NQCreate(&xhUPPool, &ppSt_ListUPThread, st_ServiceCfg.st_XMax.nStorageDLThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动HTTP上传处理线程池失败，错误：%d"), errno);
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动HTTP上传任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nStorageDLThread);

	for (int i = 0; i < st_ServiceCfg.st_XMax.nCenterThread; i++)
	{
		int* pInt_Pos = new int;
		*pInt_Pos = i;

		ppSt_ListCTThread[i]->lParam = pInt_Pos;
		ppSt_ListCTThread[i]->fpCall_ThreadsTask = XEngine_Center_HTTPThread;
	}
	if (!ManagePool_Thread_NQCreate(&xhCTPool, &ppSt_ListCTThread, st_ServiceCfg.st_XMax.nCenterThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动HTTP业务处理线程池失败，错误：%d"), errno);
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动HTTP业务任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nCenterThread);

	for (int i = 0; i < st_ServiceCfg.st_XMax.nP2XPThread; i++)
	{
		int* pInt_Pos = new int;
		*pInt_Pos = i;

		ppSt_ListP2xpThread[i]->lParam = pInt_Pos;
		ppSt_ListP2xpThread[i]->fpCall_ThreadsTask = XEngine_P2XP_TCPThread;
	}
	if (!ManagePool_Thread_NQCreate(&xhP2XPPool, &ppSt_ListP2xpThread, st_ServiceCfg.st_XMax.nP2XPThread))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动P2XP处理线程池失败，错误：%d"), errno);
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动P2XP任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nP2XPThread);

	if (st_ServiceCfg.st_P2xp.nMode > 0)
	{
		if (!NetCore_BroadCast_SendInit(&hBroadSocket, st_ServiceCfg.st_P2xp.nRVPort, st_ServiceCfg.tszIPAddr))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动P2P存储广播服务失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动P2P存储广播服务成功,端口:%d"), st_ServiceCfg.st_P2xp.nRVPort);
		pSTDThread = make_shared<std::thread>(XEngine_Task_P2PThread);
		if (!pSTDThread->joinable())
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动P2P存储广播服务线程失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动P2P存储广播服务线程成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中，P2P存储服务配置为不启动"));
	}

	m_StrVersion = st_ServiceCfg.st_XVer.pStl_ListStorage->front();
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动，存储中心服务运行中，发行版本次数:%d,当前运行版本：%s。。。"), st_ServiceCfg.st_XVer.pStl_ListStorage->size(), m_StrVersion.c_str());

	while (bIsRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_EXITAPP:

	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("存储中心服务关闭，服务器退出..."));
		bIsRun = FALSE;

		RfcComponents_HttpServer_DestroyEx(xhUPHttp);
		RfcComponents_HttpServer_DestroyEx(xhDLHttp);
		RfcComponents_HttpServer_DestroyEx(xhCenterHttp);
		HelpComponents_Datas_Destory(xhP2XPPacket);

		NetCore_TCPXCore_DestroyEx(xhNetDownload);
		NetCore_TCPXCore_DestroyEx(xhNetUPLoader);
		NetCore_TCPXCore_DestroyEx(xhNetCenter);
		NetCore_TCPXCore_DestroyEx(xhNetP2xp);

		SocketOpt_HeartBeat_DestoryEx(xhHBDownload);
		SocketOpt_HeartBeat_DestoryEx(xhHBUPLoader);
		SocketOpt_HeartBeat_DestoryEx(xhHBP2xp);

		ManagePool_Thread_NQDestroy(xhUPPool);
		ManagePool_Thread_NQDestroy(xhDLPool);
		ManagePool_Thread_NQDestroy(xhSDPool);
		ManagePool_Thread_NQDestroy(xhCTPool);
		ManagePool_Thread_NQDestroy(xhP2XPPool);

		HelpComponents_XLog_Destroy(xhLog);

		Session_User_Destory();
		Session_DLStroage_Destory();
		Session_UPStroage_Destory();
		XStorageSQL_Destory();
		XStorage_SQLite_Destory();

		if (NULL != pSTDThread)
		{
			NetCore_BroadCast_Close(hBroadSocket);
			pSTDThread->join();
		}
	}
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
