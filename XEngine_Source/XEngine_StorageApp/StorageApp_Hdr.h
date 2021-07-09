#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include <thread>
#include <list>
#include <sys/types.h>
#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
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
#include <XEngine_Include/XEngine_RfcComponents/HttpServer_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpServer_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include "../XEngine_StorageComponents/XStorage_SQLPacket/SQLPacket_Define.h"
#include "../XEngine_StorageComponents/XStorage_SQLPacket/SQLPacket_Error.h"
#include "../XEngine_StorageComponents/XStorage_Protocol/XStorageProtocol_Define.h"
#include "../XEngine_StorageComponents/XStorage_Protocol/XStorageProtocol_Error.h"
#include "../XEngine_P2XPComponents/XEngine_P2XPPeer/P2XPPeer_Define.h"
#include "../XEngine_P2XPComponents/XEngine_P2XPPeer/P2XPPeer_Error.h"
#include "../XEngine_P2XPComponents/XEngine_P2XPProtocol/P2XPProtocol_Define.h"
#include "../XEngine_P2XPComponents/XEngine_P2XPProtocol/P2XPProtocol_Error.h"
#include "../StorageModule_Session/Session_Define.h"
#include "../StorageModule_Session/Session_Error.h"
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
extern BOOL bIsSQL;
extern XLOG xhLog;

extern XNETHANDLE xhHBDownload;
extern XNETHANDLE xhHBUPLoader;
extern XNETHANDLE xhNetDownload;
extern XNETHANDLE xhNetUPLoader;
extern XNETHANDLE xhNetCenter;
extern XNETHANDLE xhUPPool;
extern XNETHANDLE xhDLPool;
extern XNETHANDLE xhSDPool;
extern XNETHANDLE xhCTPool;
extern XHANDLE xhUPHttp;
extern XHANDLE xhDLHttp;
extern XHANDLE xhCenterHttp;

extern XENGINE_SERVERCONFIG st_ServiceCfg;
extern XENGINE_LBCONFIG st_LoadbalanceCfg;

#include "StorageApp_Network.h"
#include "StorageApp_Config.h"
#include "StorageApp_Task.h"
#include "StorageApp_Download.h"
#include "StorageApp_UPLoader.h"
#include "StorageApp_Center.h"
#include "Storage_APPTask/Storage_TaskEvent.h"
#include "Storage_APPTask/Storage_TaskPass.h"
#include "Storage_APPTask/Storage_TaskQuery.h"
#include "Storage_APPTask/Storage_TaskP2P.h"

#ifdef _WINDOWS
#pragma comment(lib,"Ws2_32.lib")
#ifdef _WIN64
#pragma comment(lib,"../x64/Release/StorageModule_Session.lib")
#pragma comment(lib,"../x64/Release/StorageModule_Config.lib")
#pragma comment(lib,"../x64/Release/StorageModule_APIHelp.lib")
#pragma comment(lib,"../x64/Release/XStorage_SQLPacket.lib")
#pragma comment(lib,"../x64/Release/XStorage_Protocol.lib")
#pragma comment(lib,"../x64/Release/XEngine_P2XPPeer.lib")
#pragma comment(lib,"../x64/Release/XEngine_P2XPProtocol.lib")
#pragma comment(lib,"x64/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x64/XEngine_BaseLib/XEngine_Algorithm.lib")
#pragma comment(lib,"x64/XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"x64/XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"x64/XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"x64/XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"x64/XEngine_RfcComponents/RfcComponents_HttpServer.lib")
#pragma comment(lib,"x64/XEngine_NetHelp/NetHelp_APIHelp.lib")
#else
#ifdef _DEBUG
#pragma comment(lib,"../Debug/StorageModule_Session.lib")
#pragma comment(lib,"../Debug/StorageModule_Config.lib")
#pragma comment(lib,"../Debug/StorageModule_APIHelp.lib")
#pragma comment(lib,"../Debug/XStorage_SQLPacket.lib")
#pragma comment(lib,"../Debug/XStorage_Protocol.lib")
#pragma comment(lib,"../Debug/XEngine_P2XPPeer.lib")
#pragma comment(lib,"../Debug/XEngine_P2XPProtocol.lib")
#else
#pragma comment(lib,"../Release/StorageModule_Session.lib")
#pragma comment(lib,"../Release/StorageModule_Config.lib")
#pragma comment(lib,"../Release/StorageModule_APIHelp.lib")
#pragma comment(lib,"../Release/XStorage_SQLPacket.lib")
#pragma comment(lib,"../Release/XStorage_Protocol.lib")
#pragma comment(lib,"../Release/XEngine_P2XPPeer.lib")
#pragma comment(lib,"../Release/XEngine_P2XPProtocol.lib")
#endif
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_Algorithm.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"x86/XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"x86/XEngine_RfcComponents/RfcComponents_HttpServer.lib")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp.lib")
#endif

#else
#endif