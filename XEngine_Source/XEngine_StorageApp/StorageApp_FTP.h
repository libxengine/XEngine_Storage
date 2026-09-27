#pragma once
/********************************************************************
//    Created:     2026/09/10  17:37:00
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageApp\StorageApp_FTP.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageApp
//    File Base:   StorageApp_FTP
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     FTP服务端
//    History:
*********************************************************************/
XHTHREAD XCALLBACK XEngine_FTPContral_Thread(XPVOID lParam);
bool XEngine_Task_FTP(LPCXSTR lpszClientAddr, XENGINE_KEYVALUE* pSt_KeyValue, int nNetType);