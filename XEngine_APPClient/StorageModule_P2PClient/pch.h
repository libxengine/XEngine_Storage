// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#ifdef _WINDOWS
// 添加要在此处预编译的标头
#include "framework.h"
#include <tchar.h>
#include <WinSock2.h>
#endif
#endif //PCH_H
#include <memory>
#include <thread>
#include <unordered_map>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Define.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include "../../XEngine_Source/XStorage_Protocol.h"
#include "P2XPClient_Define.h"
#include "P2XPClient_Error.h"
/********************************************************************
//    Created:     2021/07/08  10:09:34
//    File Name:   D:\XEngine_Storage\XEngine_APPClient\StorageModule_P2PClient\pch.h
//    File Path:   D:\XEngine_Storage\XEngine_APPClient\StorageModule_P2PClient
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     P2XP客户端模块
//    History:
*********************************************************************/
extern BOOL P2XPClient_IsErrorOccur;
extern DWORD P2XPClient_dwErrorCode;

#ifdef _WINDOWS
#ifdef _DEBUG
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"x86/XEngine_Core/XEngine_NetXApi.lib")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"x86/XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"x86/XEngine_Client/XClient_Socket.lib")
#else
#ifdef _WIN64
#pragma comment(lib,"../../x64/Release/XEngine_BaseLib.lib")
#pragma comment(lib,"../../x64/Release/XEngine_Core.lib")
#pragma comment(lib,"../../x64/Release/XEngine_SystemApi.lib")
#pragma comment(lib,"../../x64/Release/XClient_Socket.lib")
#else
#pragma comment(lib,"../../Release/XEngine_BaseLib.lib")
#pragma comment(lib,"../../Release/XEngine_Core.lib")
#pragma comment(lib,"../../Release/XEngine_SystemApi.lib")
#pragma comment(lib,"../../Release/XClient_Socket.lib")
#endif
#endif
#pragma comment(lib,"Shlwapi.lib")
#else
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
