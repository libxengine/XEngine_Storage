#pragma once
/********************************************************************
//    Created:     2024/04/16  10:43:39
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageApp\StorageApp_Center.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageApp
//    File Base:   StorageApp_Center
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP管理服务处理类
//    History:
*********************************************************************/
XHTHREAD XCALLBACK XEngine_Center_HTTPThread(XPVOID lParam);
bool XEngine_Task_HttpCenter(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount);