#pragma once
/********************************************************************
//    Created:     2021/06/09  13:55:33
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_SQLPacket\SQLPacket_Define.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_SQLPacket
//    File Base:   SQLPacket_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出的定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD SQLPacket_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                        公用                                          */
/************************************************************************/
extern "C" BOOL SQLPacket_Connect(LPCTSTR lpszSQLAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass);
extern "C" BOOL SQLPacket_Close();
/************************************************************************/
/*                        文件管理                                     */
/************************************************************************/
extern "C" BOOL SQLPacket_File_Insert(XENGINE_PROTOCOLFILE * pSt_ProtocolFile);
extern "C" BOOL SQLPacket_File_Delete(XENGINE_PROTOCOLFILE* pSt_ProtocolFile);
extern "C" BOOL SQLPacket_File_QueryAll(XENGINE_PROTOCOLFILE*** pppSt_ProtocolFile, int* pInt_ListCount);