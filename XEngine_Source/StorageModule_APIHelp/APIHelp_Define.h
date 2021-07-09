#pragma once
/********************************************************************
//    Created:     2021/07/08  15:37:18
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Define.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp
//    File Base:   APIHelp_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD StorageHelp_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                                                                      */
/************************************************************************/
extern "C" BOOL APIHelp_Distributed_IsMode(list<int>*pStl_ListMode, int nMode);
extern "C" BOOL APIHelp_Distributed_RandomAddr(list<tstring>* pStl_ListAddr, TCHAR* ptszAddr);