﻿#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include <locale.h>
#include <thread>
#include <memory>
#include <list>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <minidumpapiset.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_BaseLib/Algorithm_Define.h>
#include <XEngine_Include/XEngine_BaseLib/Algorithm_Error.h>
#include <XEngine_Include/XEngine_BaseLib/Algorithm_Define.h>
#include <XEngine_Include/XEngine_BaseLib/Algorithm_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../XStorage_Protocol.h"
#include "../StorageModule_Config/Config_Define.h"
#include "../StorageModule_Config/Config_Error.h"
#include "../StorageModule_Session/Session_Define.h"
#include "../StorageModule_Session/Session_Error.h"
#include "../StorageModule_Database/Database_Define.h"
#include "../StorageModule_Database/Database_Error.h"
#include "../StorageModule_Protocol/StorageProtocol_Define.h"
#include "../StorageModule_Protocol/StorageProtocol_Error.h"
#include "../StorageModule_APIHelp/APIHelp_Define.h"
#include "../StorageModule_APIHelp/APIHelp_Error.h"
#include "../StorageModule_BTorrent/BTorrent_Define.h"
#include "../StorageModule_BTorrent/BTorrent_Error.h"
#include "../XEngine_Depend/XEngine_Module/XEngine_InfoReport/InfoReport_Define.h"
#include "../XEngine_Depend/XEngine_Module/XEngine_InfoReport/InfoReport_Error.h"

#define STORAGE_LEAVETYPE_HEARTBEAT 1
#define STORAGE_LEAVETYPE_BYSELF 2
#define STORAGE_LEAVETYPE_CLOSE 3

#define STORAGE_NETTYPE_HTTPUPLOADER 1
#define STORAGE_NETTYPE_HTTPDOWNLOAD 2
#define STORAGE_NETTYPE_HTTPCENTER 3
#define STORAGE_NETTYPE_HTTPWEBDAV 4

extern bool bIsRun;
extern bool bIsTest;
extern XHANDLE xhLog;

extern XHANDLE xhHBDownload;
extern XHANDLE xhHBUPLoader;
extern XHANDLE xhHBCenter;
extern XHANDLE xhHBWebdav;

extern XHANDLE xhNetDownload;
extern XHANDLE xhNetUPLoader;
extern XHANDLE xhNetCenter;
extern XHANDLE xhNetWebdav;

extern XHANDLE xhUPPool;
extern XHANDLE xhDLPool;
extern XHANDLE xhCTPool;
extern XHANDLE xhWDPool;

extern XHANDLE xhDLSsl;
extern XHANDLE xhUPSsl;
extern XHANDLE xhCHSsl;
extern XHANDLE xhWDSsl;

extern XHANDLE xhLimit;
extern XHANDLE xhUPHttp;
extern XHANDLE xhDLHttp;
extern XHANDLE xhCenterHttp;
extern XHANDLE xhWebdavHttp;

extern XSOCKET hBroadSocket;
extern shared_ptr<std::thread> pSTDThread;
extern shared_ptr<std::thread> pSTDThread_Action;

extern XENGINE_SERVERCONFIG st_ServiceCfg;
extern XENGINE_LBCONFIG st_LoadbalanceCfg;

#include "StorageApp_Network.h"
#include "StorageApp_Config.h"
#include "StorageApp_Download.h"
#include "StorageApp_UPLoader.h"
#include "StorageApp_Center.h"
#include "StorageApp_Webdav.h"
#include "Storage_APPTask/Storage_TaskPass.h"
#include "Storage_APPTask/Storage_TaskP2p.h"
#include "Storage_APPTask/Storage_TaskManage.h"
#include "Storage_APPTask/Storage_TaskAction.h"

#ifdef _MSC_BUILD
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_Algorithm.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption.lib")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../x64/Debug/StorageModule_Session.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_Config.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_APIHelp.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_Protocol.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_Database.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_BTorrent.lib")
#pragma comment(lib,"../x64/Debug/XEngine_InfoReport.lib")
#elif _M_ARM64
#pragma comment(lib,"../ARM64/Debug/StorageModule_Session.lib")
#pragma comment(lib,"../ARM64/Debug/StorageModule_Config.lib")
#pragma comment(lib,"../ARM64/Debug/StorageModule_APIHelp.lib")
#pragma comment(lib,"../ARM64/Debug/StorageModule_Protocol.lib")
#pragma comment(lib,"../ARM64/Debug/StorageModule_Database.lib")
#pragma comment(lib,"../ARM64/Debug/StorageModule_BTorrent.lib")
#pragma comment(lib,"../ARM64/Debug/XEngine_InfoReport.lib")
#elif _M_IX86
#pragma comment(lib,"../Debug/StorageModule_Session.lib")
#pragma comment(lib,"../Debug/StorageModule_Config.lib")
#pragma comment(lib,"../Debug/StorageModule_APIHelp.lib")
#pragma comment(lib,"../Debug/StorageModule_Protocol.lib")
#pragma comment(lib,"../Debug/StorageModule_Database.lib")
#pragma comment(lib,"../Debug/StorageModule_BTorrent.lib")
#pragma comment(lib,"../Debug/XEngine_InfoReport.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../x64/Release/StorageModule_Session.lib")
#pragma comment(lib,"../x64/Release/StorageModule_Config.lib")
#pragma comment(lib,"../x64/Release/StorageModule_APIHelp.lib")
#pragma comment(lib,"../x64/Release/StorageModule_Protocol.lib")
#pragma comment(lib,"../x64/Release/StorageModule_Database.lib")
#pragma comment(lib,"../x64/Release/StorageModule_BTorrent.lib")
#pragma comment(lib,"../x64/Release/XEngine_InfoReport.lib")
#elif _M_ARM64
#pragma comment(lib,"../ARM64/Release/StorageModule_Session.lib")
#pragma comment(lib,"../ARM64/Release/StorageModule_Config.lib")
#pragma comment(lib,"../ARM64/Release/StorageModule_APIHelp.lib")
#pragma comment(lib,"../ARM64/Release/StorageModule_Protocol.lib")
#pragma comment(lib,"../ARM64/Release/StorageModule_Database.lib")
#pragma comment(lib,"../ARM64/Release/StorageModule_BTorrent.lib")
#pragma comment(lib,"../ARM64/Release/XEngine_InfoReport.lib")
#elif _M_IX86
#pragma comment(lib,"../Release/StorageModule_Session.lib")
#pragma comment(lib,"../Release/StorageModule_Config.lib")
#pragma comment(lib,"../Release/StorageModule_APIHelp.lib")
#pragma comment(lib,"../Release/StorageModule_Protocol.lib")
#pragma comment(lib,"../Release/StorageModule_Database.lib")
#pragma comment(lib,"../Release/StorageModule_BTorrent.lib")
#pragma comment(lib,"../Release/XEngine_InfoReport.lib")
#endif
#endif
#endif