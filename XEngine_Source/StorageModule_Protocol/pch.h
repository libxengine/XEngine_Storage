// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

#ifdef _MSC_BUILD
// 添加要在此处预编译的标头
#include "framework.h"
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#endif //PCH_H
#include <json/json.h>
#include <tinyxml2.h>
#include <list>
#include <unordered_map>
#include <filesystem>
using namespace std;
using namespace tinyxml2;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../XStorage_Protocol.h"
#include "../StorageModule_Config/Config_Define.h"
#include "../StorageModule_Config/Config_Error.h"
#include "../StorageModule_Session/Session_Define.h"
#include "../StorageModule_Database/Database_Define.h"
#include "StorageProtocol_Define.h"
#include "StorageProtocol_Error.h"
/********************************************************************
//    Created:     2021/10/25  14:37:26
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\pch.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议模块公用头
//    History:
*********************************************************************/
extern bool Protocol_IsErrorOccur;
extern XLONG Protocol_dwErrorCode;

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib,"../x64/Debug/jsoncpp")
#pragma comment(lib,"../x64/Debug/tinyxml2")
#else
#pragma comment(lib,"../Debug/jsoncpp")
#pragma comment(lib,"../Debug/tinyxml2")
#endif
#else
#ifdef _WIN64
#pragma comment(lib,"../x64/Release/jsoncpp")
#pragma comment(lib,"../x64/Release/tinyxml2")
#else
#pragma comment(lib,"../Release/jsoncpp")
#pragma comment(lib,"../Release/tinyxml2")
#endif
#endif
#endif