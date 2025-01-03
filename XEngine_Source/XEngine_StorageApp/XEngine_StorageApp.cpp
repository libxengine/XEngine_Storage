#include "StorageApp_Hdr.h"

bool bIsRun = false;
bool bIsTest = false;
XHANDLE xhLog = NULL;

XHANDLE xhHBDownload = NULL;
XHANDLE xhHBUPLoader = NULL;
XHANDLE xhHBCenter = NULL;
XHANDLE xhHBWebdav = NULL;

XHANDLE xhNetDownload = NULL;
XHANDLE xhNetUPLoader = NULL;
XHANDLE xhNetCenter = NULL;
XHANDLE xhNetWebdav = NULL;

XHANDLE xhUPPool = NULL;
XHANDLE xhDLPool = NULL;
XHANDLE xhCTPool = NULL;
XHANDLE xhWDPool = NULL;

XHANDLE xhDLSsl = NULL;
XHANDLE xhUPSsl = NULL;
XHANDLE xhCHSsl = NULL;
XHANDLE xhWDSsl = NULL;

XHANDLE xhLimit = NULL;
XHANDLE xhUPHttp = NULL;
XHANDLE xhDLHttp = NULL;
XHANDLE xhCenterHttp = NULL;
XHANDLE xhWebdavHttp = NULL;

XSOCKET hBroadSocket = 0;
shared_ptr<std::thread> pSTDThread = NULL;
shared_ptr<std::thread> pSTDThread_Action = NULL;

XENGINE_SERVERCONFIG st_ServiceCfg;
XENGINE_LBCONFIG st_LoadbalanceCfg;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("存储中心服务器退出..."));
		bIsRun = false;

		HttpProtocol_Server_DestroyEx(xhUPHttp);
		HttpProtocol_Server_DestroyEx(xhDLHttp);
		HttpProtocol_Server_DestroyEx(xhCenterHttp);
		HttpProtocol_Server_DestroyEx(xhWebdavHttp);

		Cryption_Server_StopEx(xhDLSsl);
		Cryption_Server_StopEx(xhUPSsl);
		Cryption_Server_StopEx(xhCHSsl);
		Cryption_Server_StopEx(xhWDSsl);

		NetCore_TCPXCore_DestroyEx(xhNetDownload);
		NetCore_TCPXCore_DestroyEx(xhNetUPLoader);
		NetCore_TCPXCore_DestroyEx(xhNetCenter);
		NetCore_TCPXCore_DestroyEx(xhNetWebdav);

		SocketOpt_HeartBeat_DestoryEx(xhHBDownload);
		SocketOpt_HeartBeat_DestoryEx(xhHBUPLoader);
		SocketOpt_HeartBeat_DestoryEx(xhHBCenter);
		SocketOpt_HeartBeat_DestoryEx(xhHBWebdav);

		ManagePool_Thread_NQDestroy(xhUPPool);
		ManagePool_Thread_NQDestroy(xhDLPool);
		ManagePool_Thread_NQDestroy(xhCTPool);
		ManagePool_Thread_NQDestroy(xhWDPool);

		Algorithm_Calculation_Close(xhLimit);
		HelpComponents_XLog_Destroy(xhLog);

		Session_DLStroage_Destory();
		Session_UPStroage_Destory();
		Database_File_Destory();
		Database_Memory_Destory();

		if (NULL != pSTDThread)
		{
			NetCore_BroadCast_Close(hBroadSocket);
			pSTDThread->join();
		}
		if (NULL != pSTDThread_Action)
		{
			pSTDThread_Action->join();
		}
		exit(0);
	}
}
static int ServiceApp_Deamon(int wait)
{
#ifndef _MSC_BUILD
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
#ifdef _MSC_BUILD
LONG WINAPI Coredump_ExceptionFilter(EXCEPTION_POINTERS* pExceptionPointers)
{
	static int i = 0;
	XCHAR tszFileStr[MAX_PATH] = {};
	XCHAR tszTimeStr[128] = {};
	BaseLib_Time_TimeToStr(tszTimeStr);
	_xstprintf(tszFileStr, _X("./XEngine_Coredump/dumpfile_%s_%d.dmp"), tszTimeStr, i++);

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_FATAL, _X("主程序:软件崩溃,写入dump:%s"), tszFileStr);

	HANDLE hDumpFile = CreateFileA(tszFileStr, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hDumpFile)
	{
		MINIDUMP_EXCEPTION_INFORMATION st_DumpInfo = {};
		st_DumpInfo.ExceptionPointers = pExceptionPointers;
		st_DumpInfo.ThreadId = GetCurrentThreadId();
		st_DumpInfo.ClientPointers = TRUE;

		// 写入 dump 文件
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &st_DumpInfo, NULL, NULL);
		CloseHandle(hDumpFile);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif
int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	SetUnhandledExceptionFilter(Coredump_ExceptionFilter);
#endif
	bIsRun = true;
	int nRet = -1;
	LPCXSTR lpszHTTPMime = _X("./XEngine_Config/HttpMime.types");
	LPCXSTR lpszHTTPCode = _X("./XEngine_Config/HttpCode.types");
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	THREADPOOL_PARAMENT** ppSt_ListUPThread;
	THREADPOOL_PARAMENT** ppSt_ListDLThread;
	THREADPOOL_PARAMENT** ppSt_ListCTThread;
	THREADPOOL_PARAMENT** ppSt_ListWDThread;

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
		XCHAR tszAddr[128];
		memset(tszAddr, '\0', sizeof(tszAddr));

		_xstprintf(tszAddr, _X("http://127.0.0.1:%d/Api/Manage/Config"), st_ServiceCfg.nCenterPort);
		APIClient_Http_Request(_X("POST"), tszAddr);
		return 0;
	}
	st_XLogConfig.XLog_MaxBackupFile = st_ServiceCfg.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_ServiceCfg.st_XLog.nMaxSize;
	_tcsxcpy(st_XLogConfig.tszFileName, _X("./XEngine_XLog/XEngine_StorageApp.log"));

	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务器失败，启动日志失败，错误：%lX", XLog_GetLastError());
		goto XENGINE_EXITAPP;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_ServiceCfg.st_XLog.nLogLeave);

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化日志系统成功"));
	if (st_ServiceCfg.bDeamon)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，使用守护进程启动服务..."));
		ServiceApp_Deamon(1);
	}

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化服务器信号管理成功"));

	xhLimit = Algorithm_Calculation_Create();
	if (NULL == xhLimit)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，创建流量限速对象模式失败,错误:%lX"), Algorithm_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，创建流量限速对象模式成功"));

	if (st_ServiceCfg.st_XTime.bHBTime)
	{
		xhHBDownload = SocketOpt_HeartBeat_InitEx(st_ServiceCfg.st_XTime.nStorageTimeOut, st_ServiceCfg.st_XTime.nTimeCheck, XEngine_Callback_HBDownload);
		if (NULL == xhHBDownload)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化下载心跳管理服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化下载心跳管理服务成功,时间:%d,次数:%d"), st_ServiceCfg.st_XTime.nStorageTimeOut, st_ServiceCfg.st_XTime.nTimeCheck);

		xhHBUPLoader = SocketOpt_HeartBeat_InitEx(st_ServiceCfg.st_XTime.nStorageTimeOut, st_ServiceCfg.st_XTime.nTimeCheck, XEngine_Callback_HBUPLoader);
		if (NULL == xhHBUPLoader)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化上传心跳管理服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化上传心跳管理服务成功,时间:%d,次数:%d"), st_ServiceCfg.st_XTime.nStorageTimeOut, st_ServiceCfg.st_XTime.nTimeCheck);

		xhHBCenter = SocketOpt_HeartBeat_InitEx(st_ServiceCfg.st_XTime.nCenterTimeOut, st_ServiceCfg.st_XTime.nTimeCheck, XEngine_Callback_HBCenter);
		if (NULL == xhHBCenter)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化业务管理服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化业务管理服务成功,时间:%d,次数:%d"), st_ServiceCfg.st_XTime.nCenterTimeOut, st_ServiceCfg.st_XTime.nTimeCheck);

		xhHBWebdav = SocketOpt_HeartBeat_InitEx(st_ServiceCfg.st_XTime.nWebdavTimeOut, st_ServiceCfg.st_XTime.nTimeCheck, XEngine_Callback_HBWebdav);
		if (NULL == xhHBCenter)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化WEBDAV服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化WEBDAV服务成功,时间:%d,次数:%d"), st_ServiceCfg.st_XTime.nWebdavTimeOut, st_ServiceCfg.st_XTime.nTimeCheck);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，心跳管理服务配置为不启用..."));
	}

	if (st_ServiceCfg.st_XSql.bEnable)
	{
		if (!Database_File_Init((DATABASE_MYSQL_CONNECTINFO*)&st_ServiceCfg.st_XSql, st_ServiceCfg.st_XTime.nDBMonth))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化MYSQL数据库服务失败，错误：%lX"), Database_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化MYSQL数据库服务成功,数据库地址:%s"), st_ServiceCfg.st_XSql.tszSQLAddr);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，初始化数据库失败,数据库被设置为禁用,相关功能已经被禁止使用!"));
	}

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，验证服务启动标志:API:%d,上传:%d,下载:%d"), st_ServiceCfg.st_XProxy.bAuthPass, st_ServiceCfg.st_XProxy.bDLPass, st_ServiceCfg.st_XProxy.bUPPass);
	//启动下载服务
	if (st_ServiceCfg.nStorageDLPort > 0)
	{
		xhDLHttp = HttpProtocol_Server_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceCfg.st_XMax.nStorageDLThread);
		if (NULL == xhDLHttp)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化HTTP下载服务失败，错误：%lX"), HttpProtocol_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化HTTP下载服务成功，IO线程个数:%d"), st_ServiceCfg.st_XMax.nStorageDLThread);

		if (!Session_DLStroage_Init(st_ServiceCfg.st_XLimit.nMaxDNConnect))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动下载会话服务失败，错误：%lX"), Session_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动下载会话服务成功,下载限速模式:%s,连接数限制;%d"), st_ServiceCfg.st_XLimit.bLimitMode ? "开" : "关", st_ServiceCfg.st_XLimit.nMaxDNConnect);

		if (st_ServiceCfg.st_XCert.bDLEnable)
		{
			if (_tcsxlen(st_ServiceCfg.st_XCert.tszCertServer) > 0)
			{
				xhDLSsl = Cryption_Server_InitEx(st_ServiceCfg.st_XCert.tszCertChain, st_ServiceCfg.st_XCert.tszCertServer, st_ServiceCfg.st_XCert.tszCertKey, false, false, XENGINE_CRYPTION_PROTOCOL_TLS);
			}
			else
			{
				xhDLSsl = Cryption_Server_InitEx(st_ServiceCfg.st_XCert.tszCertChain, NULL, st_ServiceCfg.st_XCert.tszCertKey, false, false, XENGINE_CRYPTION_PROTOCOL_TLS);
			}
			if (NULL == xhDLSsl)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动下载SSL服务失败，错误：%lX"), Session_GetLastError());
				goto XENGINE_EXITAPP;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动下载SSL服务成功,证书链:%s,证书Key:%s"), st_ServiceCfg.st_XCert.tszCertChain, st_ServiceCfg.st_XCert.tszCertKey);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，检测到没有启用下载SSL服务"));
		}

		xhNetDownload = NetCore_TCPXCore_StartEx(st_ServiceCfg.nStorageDLPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread, false, st_ServiceCfg.bReuseraddr);
		if (NULL == xhNetDownload)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动下载存储网络服务失败,端口:%d，错误：%lX,%d"), st_ServiceCfg.nStorageDLPort, NetCore_GetLastError(), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动下载存储网络服务成功，端口：%d,IO线程个数:%d"), st_ServiceCfg.nStorageDLPort, st_ServiceCfg.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhNetDownload, XEngine_Callback_DownloadLogin, XEngine_Callback_DownloadRecv, XEngine_Callback_DownloadLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，注册下载存储网络服务事件成功！"));

		BaseLib_Memory_Malloc((XPPPMEM)&ppSt_ListDLThread, st_ServiceCfg.st_XMax.nStorageDLThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceCfg.st_XMax.nStorageDLThread; i++)
		{
			int* pInt_Pos = new int;
			*pInt_Pos = i;

			ppSt_ListDLThread[i]->lParam = pInt_Pos;
			ppSt_ListDLThread[i]->fpCall_ThreadsTask = XEngine_Download_HTTPThread;
		}
		xhDLPool = ManagePool_Thread_NQCreate(&ppSt_ListDLThread, st_ServiceCfg.st_XMax.nStorageDLThread);
		if (NULL == xhDLPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动HTTP下载处理线程池失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动HTTP下载任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nStorageDLThread);
	}
	//启动上传服务
	if (st_ServiceCfg.nStorageUPPort > 0)
	{
		xhUPHttp = HttpProtocol_Server_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceCfg.st_XMax.nStorageUPThread);
		if (NULL == xhDLHttp)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化HTTP上传服务失败，错误：%lX"), HttpProtocol_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化HTTP上传服务成功，IO线程个数:%d"), st_ServiceCfg.st_XMax.nStorageUPThread);

		if (!Session_UPStroage_Init(st_ServiceCfg.st_XLimit.nMaxUPConnect, st_ServiceCfg.st_XStorage.bResumable))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动上传会话服务失败，错误：%lX"), Session_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动上传会话服务成功,连接数限制;%d"), st_ServiceCfg.st_XLimit.nMaxUPConnect);

		if (st_ServiceCfg.st_XCert.bUPEnable)
		{
			if (_tcsxlen(st_ServiceCfg.st_XCert.tszCertServer) > 0)
			{
				xhUPSsl = Cryption_Server_InitEx(st_ServiceCfg.st_XCert.tszCertChain, st_ServiceCfg.st_XCert.tszCertServer, st_ServiceCfg.st_XCert.tszCertKey, false, false, XENGINE_CRYPTION_PROTOCOL_TLS);
			}
			else
			{
				xhUPSsl = Cryption_Server_InitEx(st_ServiceCfg.st_XCert.tszCertChain, NULL, st_ServiceCfg.st_XCert.tszCertKey, false, false, XENGINE_CRYPTION_PROTOCOL_TLS);
			}
			if (NULL == xhUPSsl)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动上传SSL服务失败，错误：%lX"), Session_GetLastError());
				goto XENGINE_EXITAPP;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动上传SSL服务成功,证书链:%s,证书Key:%s"), st_ServiceCfg.st_XCert.tszCertChain, st_ServiceCfg.st_XCert.tszCertKey);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，检测到没有启用上传SSL服务"));
		}

		xhNetUPLoader = NetCore_TCPXCore_StartEx(st_ServiceCfg.nStorageUPPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread, false, st_ServiceCfg.bReuseraddr);
		if (NULL == xhNetUPLoader)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动上传存储网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nStorageUPPort, NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动上传存储网络服务成功，端口：%d,IO线程个数:%d"), st_ServiceCfg.nStorageUPPort, st_ServiceCfg.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhNetUPLoader, XEngine_Callback_UPLoaderLogin, XEngine_Callback_UPLoaderRecv, XEngine_Callback_UPLoaderLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，注册上传存储网络服务事件成功！"));

		BaseLib_Memory_Malloc((XPPPMEM)&ppSt_ListUPThread, st_ServiceCfg.st_XMax.nStorageUPThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceCfg.st_XMax.nStorageUPThread; i++)
		{
			int* pInt_Pos = new int;
			*pInt_Pos = i;

			ppSt_ListUPThread[i]->lParam = pInt_Pos;
			ppSt_ListUPThread[i]->fpCall_ThreadsTask = XEngine_UPLoader_HTTPThread;
		}
		xhUPPool = ManagePool_Thread_NQCreate(&ppSt_ListUPThread, st_ServiceCfg.st_XMax.nStorageDLThread);
		if (NULL == xhUPPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动HTTP上传处理线程池失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动HTTP上传任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nStorageDLThread);
	}
	//启动接口服务
	if (st_ServiceCfg.nCenterPort > 0)
	{
		xhCenterHttp = HttpProtocol_Server_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceCfg.st_XMax.nCenterThread);
		if (NULL == xhCenterHttp)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化HTTP业务服务失败，错误：%lX"), HttpProtocol_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化HTTP业务服务成功，IO线程个数:%d"), st_ServiceCfg.st_XMax.nCenterThread);

		if (st_ServiceCfg.st_XCert.bCHEnable)
		{
			if (_tcsxlen(st_ServiceCfg.st_XCert.tszCertServer) > 0)
			{
				xhCHSsl = Cryption_Server_InitEx(st_ServiceCfg.st_XCert.tszCertChain, st_ServiceCfg.st_XCert.tszCertServer, st_ServiceCfg.st_XCert.tszCertKey, false, false, XENGINE_CRYPTION_PROTOCOL_TLS);
			}
			else
			{
				xhCHSsl = Cryption_Server_InitEx(st_ServiceCfg.st_XCert.tszCertChain, NULL, st_ServiceCfg.st_XCert.tszCertKey, false, false, XENGINE_CRYPTION_PROTOCOL_TLS);
			}
			if (NULL == xhCHSsl)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动业务SSL服务失败，错误：%lX"), Session_GetLastError());
				goto XENGINE_EXITAPP;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动业务SSL服务成功,证书链:%s,证书Key:%s"), st_ServiceCfg.st_XCert.tszCertChain, st_ServiceCfg.st_XCert.tszCertKey);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，检测到没有启用业务SSL服务"));
		}

		xhNetCenter = NetCore_TCPXCore_StartEx(st_ServiceCfg.nCenterPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread, false, st_ServiceCfg.bReuseraddr);
		if (NULL == xhNetCenter)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动业务控制存储网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nCenterPort, NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动业务控制存储网络服务成功，端口：%d,IO线程个数:%d"), st_ServiceCfg.nCenterPort, st_ServiceCfg.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhNetCenter, XEngine_Callback_CenterLogin, XEngine_Callback_CenterRecv, XEngine_Callback_CenterLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，注册业务控制存储存储网络服务事件成功！"));

		BaseLib_Memory_Malloc((XPPPMEM)&ppSt_ListCTThread, st_ServiceCfg.st_XMax.nCenterThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceCfg.st_XMax.nCenterThread; i++)
		{
			int* pInt_Pos = new int;
			*pInt_Pos = i;

			ppSt_ListCTThread[i]->lParam = pInt_Pos;
			ppSt_ListCTThread[i]->fpCall_ThreadsTask = XEngine_Center_HTTPThread;
		}
		xhCTPool = ManagePool_Thread_NQCreate(&ppSt_ListCTThread, st_ServiceCfg.st_XMax.nCenterThread);
		if (NULL == xhCTPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动HTTP业务处理线程池失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动HTTP业务任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nCenterThread);
	}
	//WEBDAV
	if (st_ServiceCfg.nWebdavPort > 0)
	{
		xhWebdavHttp = HttpProtocol_Server_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceCfg.st_XMax.nWebdavThread);
		if (NULL == xhWebdavHttp)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，初始化WEBDAV服务失败，错误：%lX"), HttpProtocol_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，初始化WEBDAV服务成功，IO线程个数:%d"), st_ServiceCfg.st_XMax.nWebdavThread);

		if (st_ServiceCfg.st_XCert.bWDEnable)
		{
			if (_tcsxlen(st_ServiceCfg.st_XCert.tszCertServer) > 0)
			{
				xhWDSsl = Cryption_Server_InitEx(st_ServiceCfg.st_XCert.tszCertChain, st_ServiceCfg.st_XCert.tszCertServer, st_ServiceCfg.st_XCert.tszCertKey, false, false, XENGINE_CRYPTION_PROTOCOL_TLS);
			}
			else
			{
				xhWDSsl = Cryption_Server_InitEx(st_ServiceCfg.st_XCert.tszCertChain, NULL, st_ServiceCfg.st_XCert.tszCertKey, false, false, XENGINE_CRYPTION_PROTOCOL_TLS);
			}
			if (NULL == xhWDSsl)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动WEBDAV SSL服务失败，错误：%lX"), Session_GetLastError());
				goto XENGINE_EXITAPP;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动WEBDAV SSL服务成功,证书链:%s,证书Key:%s"), st_ServiceCfg.st_XCert.tszCertChain, st_ServiceCfg.st_XCert.tszCertKey);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，检测到没有启用WEBDAV SSL服务"));
		}

		xhNetWebdav = NetCore_TCPXCore_StartEx(st_ServiceCfg.nWebdavPort, st_ServiceCfg.st_XMax.nMaxClient, st_ServiceCfg.st_XMax.nIOThread, false, st_ServiceCfg.bReuseraddr);
		if (NULL == xhNetWebdav)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动WEBDAV网络服务失败,端口:%d，错误：%lX"), st_ServiceCfg.nWebdavPort, NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动WEBDAV网络服务成功，端口：%d,IO线程个数:%d"), st_ServiceCfg.nWebdavPort, st_ServiceCfg.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhNetWebdav, XEngine_Callback_WebdavLogin, XEngine_Callback_WebdavRecv, XEngine_Callback_WebdavLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，注册WEBDAV网络服务事件成功！"));

		BaseLib_Memory_Malloc((XPPPMEM)&ppSt_ListWDThread, st_ServiceCfg.st_XMax.nWebdavThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceCfg.st_XMax.nWebdavThread; i++)
		{
			int* pInt_Pos = new int;
			*pInt_Pos = i;

			ppSt_ListWDThread[i]->lParam = pInt_Pos;
			ppSt_ListWDThread[i]->fpCall_ThreadsTask = XEngine_Webdav_HTTPThread;
		}
		xhWDPool = ManagePool_Thread_NQCreate(&ppSt_ListWDThread, st_ServiceCfg.st_XMax.nWebdavThread);
		if (NULL == xhWDPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动WEBDAV处理线程池失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动WEBDAV任务处理线程池成功,线程池个数:%d"), st_ServiceCfg.st_XMax.nWebdavThread);
	}
	//只有使用了数据库,才启用P2P
	if (st_ServiceCfg.st_P2xp.bEnable)
	{
		if (!Database_Memory_Init(st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket, st_ServiceCfg.st_XStorage.nHashMode))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动P2P内存数据库失败，错误：%lX"), Database_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动P2P内存数据库成功"));

		if (!NetCore_BroadCast_Create(&hBroadSocket, st_ServiceCfg.st_P2xp.nRVPort))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动P2P存储广播服务失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动P2P存储广播服务成功,端口:%d"), st_ServiceCfg.st_P2xp.nRVPort);
		pSTDThread = make_shared<std::thread>(XEngine_Task_P2PThread);
		if (!pSTDThread->joinable())
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动P2P存储广播服务线程失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动P2P存储广播服务线程成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，P2P存储服务配置为不启动"));
	}
	//Action转录
	if (st_ServiceCfg.st_XAction.bEnable)
	{
		pSTDThread_Action = make_shared<std::thread>(Session_Action_Thread);
		if (!pSTDThread_Action->joinable())
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动转录动作处理线程失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动转录动作处理线程成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，转录动作没有启用"));
	}
	//发送信息报告
	if (st_ServiceCfg.st_XReport.bEnable && !bIsTest)
	{
		if (InfoReport_APIMachine_Send(st_ServiceCfg.st_XReport.tszAPIUrl, st_ServiceCfg.st_XReport.tszServiceName))
		{
			__int64x nTimeCount = 0;
			InfoReport_APIMachine_GetTime(st_ServiceCfg.st_XReport.tszAPIUrl, st_ServiceCfg.st_XReport.tszServiceName, &nTimeCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动信息报告给API服务器:%s 成功,报告次数:%lld"), st_ServiceCfg.st_XReport.tszAPIUrl, nTimeCount);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动信息报告给API服务器:%s 失败，错误：%lX"), st_ServiceCfg.st_XReport.tszAPIUrl, InfoReport_GetLastError());
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，信息报告给API服务器没有启用"));
	}

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("所有服务成功启动，存储中心服务运行中，发行版本次数:%d,XEngine版本:%s%s 当前运行版本：%s。。。"), st_ServiceCfg.st_XVer.pStl_ListStorage->size(), BaseLib_Version_XNumberStr(), BaseLib_Version_XTypeStr(), st_ServiceCfg.st_XVer.pStl_ListStorage->front().c_str());

	while (true)
	{
		if (bIsTest)
		{
			nRet = 0;
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_EXITAPP:

	if (bIsRun)
	{
		bIsRun = false;
		if (bIsTest && 0 == nRet)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("存储中心服务关闭，测试程序退出..."));
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("存储中心服务关闭，服务器退出..."));
		}
		HttpProtocol_Server_DestroyEx(xhUPHttp);
		HttpProtocol_Server_DestroyEx(xhDLHttp);
		HttpProtocol_Server_DestroyEx(xhCenterHttp);
		HttpProtocol_Server_DestroyEx(xhWebdavHttp);

		Cryption_Server_StopEx(xhDLSsl);
		Cryption_Server_StopEx(xhUPSsl);
		Cryption_Server_StopEx(xhCHSsl);
		Cryption_Server_StopEx(xhWDSsl);

		NetCore_TCPXCore_DestroyEx(xhNetDownload);
		NetCore_TCPXCore_DestroyEx(xhNetUPLoader);
		NetCore_TCPXCore_DestroyEx(xhNetCenter);
		NetCore_TCPXCore_DestroyEx(xhNetWebdav);

		SocketOpt_HeartBeat_DestoryEx(xhHBDownload);
		SocketOpt_HeartBeat_DestoryEx(xhHBUPLoader);
		SocketOpt_HeartBeat_DestoryEx(xhHBCenter);
		SocketOpt_HeartBeat_DestoryEx(xhHBWebdav);

		ManagePool_Thread_NQDestroy(xhUPPool);
		ManagePool_Thread_NQDestroy(xhDLPool);
		ManagePool_Thread_NQDestroy(xhCTPool);
		ManagePool_Thread_NQDestroy(xhWDPool);

		Algorithm_Calculation_Close(xhLimit);
		HelpComponents_XLog_Destroy(xhLog);

		Session_DLStroage_Destory();
		Session_UPStroage_Destory();
		Database_File_Destory();
		Database_Memory_Destory();

		if (NULL != pSTDThread)
		{
			NetCore_BroadCast_Close(hBroadSocket);
			pSTDThread->join();
		}
		if (NULL != pSTDThread_Action)
		{
			pSTDThread_Action->join();
		}
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return nRet;
}
