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
#include <json/json.h>
#include <sys/stat.h>
#include <WinSock2.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#ifdef _CENTOS
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif
#endif
#endif //PCH_H
#include <list>
#include <unordered_map>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include "../../XStorage_Protocol.h"
#include "../XStorage_SQLPacket/SQLPacket_Define.h"
#include "XStorageProtocol_Define.h"
#include "XStorageProtocol_Error.h"
/********************************************************************
//	Created:	2019/6/28   15:07
//	Filename: 	E:\XEngine_Windows\XEngine_SourceCode\XEngine_Storage\XEngine_XStorageProtocol\stdafx.h
//	File Path:	E:\XEngine_Windows\XEngine_SourceCode\XEngine_Storage\XEngine_XStorageProtocol
//	File Base:	stdafx
//	File Ext:	h
//  Project:    XEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	公用头文件
//	History:
*********************************************************************/
extern BOOL XStorage_IsErrorOccur;
extern DWORD XStorage_dwErrorCode;

#ifdef _WINDOWS
#ifdef _WIN64
#pragma comment(lib,"x64/XEngine_BaseLib/XEngine_BaseLib.lib")
#else
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib.lib")
#endif // _WIN64
#else

#endif