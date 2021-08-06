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
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <json/json.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
#include "../../XStorage_Protocol.h"
#include "P2XPProtocol_Define.h"
#include "P2XPProtocol_Error.h"
/********************************************************************
//	Created:	2013/8/15  20:36
//	File Name: 	/NetEngine_Linux/NetEngine_SourceCode/NetEngine_P2xp/NetEngine_P2XPProtocol/P2XPProtocol_CommHeader.h
//	File Path:	/NetEngine_Linux/NetEngine_SourceCode/NetEngine_P2xp/NetEngine_P2XPProtocol/
//	File Base:	P2XPProtocol_CommHeader
//	File Ext:	h
//  Project:    NetSocketEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	P2XP协议模块公用头
//	History:
*********************************************************************/
extern BOOL P2XPProtocol_IsErrorOccur;
extern DWORD P2XPProtocol_dwErrorCode;

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif