#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include <thread>
#include <list>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#include <io.h>
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
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpServer_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpServer_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../XStorage_Protocol.h"
#include "../StorageModule_Session/Session_Define.h"
#include "../StorageModule_Session/Session_Error.h"
#include "../StorageModule_Database/Database_Define.h"
#include "../StorageModule_Database/Database_Error.h"
#include "../StorageModule_Protocol/StorageProtocol_Define.h"
#include "../StorageModule_Protocol/StorageProtocol_Error.h"
#include "../StorageModule_Config/Config_Define.h"
#include "../StorageModule_Config/Config_Error.h"
#include "../StorageModule_APIHelp/APIHelp_Define.h"
#include "../StorageModule_APIHelp/APIHelp_Error.h"

#define STORAGE_LEAVETYPE_HEARTBEAT 1
#define STORAGE_LEAVETYPE_BYSELF 2
#define STORAGE_LEAVETYPE_CLOSE 3

#define STORAGE_NETTYPE_HTTPUPLOADER 1
#define STORAGE_NETTYPE_HTTPDOWNLOAD 2
#define STORAGE_NETTYPE_HTTPCENTER 3

extern BOOL bIsRun;
extern XLOG xhLog;

extern XNETHANDLE xhHBDownload;
extern XNETHANDLE xhHBUPLoader;
extern XNETHANDLE xhHBCenter;

extern XNETHANDLE xhNetDownload;
extern XNETHANDLE xhNetUPLoader;
extern XNETHANDLE xhNetCenter;

extern XNETHANDLE xhUPPool;
extern XNETHANDLE xhDLPool;
extern XNETHANDLE xhCTPool;

extern XHANDLE xhDLSsl;
extern XHANDLE xhUPSsl;
extern XHANDLE xhCHSsl;
extern XHANDLE xhLimit;
extern XHANDLE xhUPHttp;
extern XHANDLE xhDLHttp;
extern XHANDLE xhCenterHttp;

extern SOCKET hBroadSocket;
extern shared_ptr<std::thread> pSTDThread;

extern XENGINE_SERVERCONFIG st_ServiceCfg;
extern XENGINE_LBCONFIG st_LoadbalanceCfg;

#include "StorageApp_Network.h"
#include "StorageApp_Config.h"
#include "StorageApp_Download.h"
#include "StorageApp_UPLoader.h"
#include "StorageApp_Center.h"
#include "Storage_APPTask/Storage_TaskPass.h"
#include "Storage_APPTask/Storage_TaskP2p.h"
#include "Storage_APPTask/Storage_TaskManage.h"

#ifdef _WINDOWS
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_Algorithm.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpServer.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"../x64/Debug/StorageModule_Session.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_Config.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_APIHelp.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_Protocol.lib")
#pragma comment(lib,"../x64/Debug/StorageModule_Database.lib")
#else
#pragma comment(lib,"../x64/Release/StorageModule_Session.lib")
#pragma comment(lib,"../x64/Release/StorageModule_Config.lib")
#pragma comment(lib,"../x64/Release/StorageModule_APIHelp.lib")
#pragma comment(lib,"../x64/Release/StorageModule_Protocol.lib")
#pragma comment(lib,"../x64/Release/StorageModule_Database.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"../Debug/StorageModule_Session.lib")
#pragma comment(lib,"../Debug/StorageModule_Config.lib")
#pragma comment(lib,"../Debug/StorageModule_APIHelp.lib")
#pragma comment(lib,"../Debug/StorageModule_Protocol.lib")
#pragma comment(lib,"../Debug/StorageModule_Database.lib")
#else
#pragma comment(lib,"../Release/StorageModule_Session.lib")
#pragma comment(lib,"../Release/StorageModule_Config.lib")
#pragma comment(lib,"../Release/StorageModule_APIHelp.lib")
#pragma comment(lib,"../Release/StorageModule_Protocol.lib")
#pragma comment(lib,"../Release/StorageModule_Database.lib")
#endif
#endif
#endif