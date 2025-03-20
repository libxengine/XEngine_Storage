﻿// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#ifdef _MSC_BUILD
// 添加要在此处预编译的标头
#include "framework.h"
#include <tchar.h>
#include <WinSock2.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#endif //PCH_H
#include <string.h>
#include <list>
#include <string>
#include <memory>
#include <json/json.h>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#include "../XStorage_Protocol.h"
#include "../StorageModule_Config/Config_Define.h"
#include "../StorageModule_Database/Database_Define.h"
#include "APIHelp_Define.h"
#include "APIHelp_Error.h"
/********************************************************************
//    Created:     2021/07/08  16:41:05
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\pch.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公有头文件
//    History:
*********************************************************************/
extern bool APIHelp_IsErrorOccur;
extern XLONG APIHelp_dwErrorCode;

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi")
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../x64/Debug/jsoncpp.lib")
#elif _M_ARM64
#pragma comment(lib,"../ARM64/Debug/jsoncpp.lib")
#elif _M_IX86
#pragma comment(lib,"../Debug/jsoncpp.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../x64/Release/jsoncpp.lib")
#elif _M_ARM64
#pragma comment(lib,"../ARM64/Release/jsoncpp.lib")
#elif _M_IX86
#pragma comment(lib,"../Release/jsoncpp.lib")
#endif
#endif
#endif