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
#include <io.h>
#else
#include <unistd.h>
#endif
#endif //PCH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <list>
#include <memory>
#include <unordered_map>
#include <shared_mutex>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include "Session_Define.h"
#include "Session_Error.h"
/********************************************************************
//    Created:     2021/06/02  14:16:31
//    File Name:   D:\XEngine_Storage\StorageModule_Session\pch.h
//    File Path:   D:\XEngine_Storage\StorageModule_Session
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     会话公用头文件
//    History:
*********************************************************************/
extern bool Session_IsErrorOccur;
extern XLONG Session_dwErrorCode;

#ifdef _WINDOWS
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#endif