#pragma once
/********************************************************************
//    Created:     2024/08/02  15:23:12
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageApp\StorageApp_Webdav.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageApp
//    File Base:   StorageApp_Webdav
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     WEBDAV
//    History:
*********************************************************************/
XHTHREAD XCALLBACK XEngine_Webdav_HTTPThread(XPVOID lParam);
bool XEngine_Task_HttpWebdav(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount);