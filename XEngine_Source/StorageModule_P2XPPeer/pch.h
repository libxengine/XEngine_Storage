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
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#endif
#endif //PCH_H
#include <list>
#include <shared_mutex>
#include <unordered_map>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include "../XStorage_Protocol.h"
#include "P2XPPeer_Define.h"
#include "P2XPPeer_Error.h"
/********************************************************************
//    Created:     2022/03/29  14:31:31
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_P2XPPeer\pch.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_P2XPPeer
//    File Base:   pch
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     P2XP节点管理器模块公用头
//    History:
*********************************************************************/
extern BOOL PeerManage_IsErrorOccur;
extern DWORD PeerManage_dwErrorCode;

#ifdef _WINDOWS
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#endif
