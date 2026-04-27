#pragma once
/********************************************************************
//    Created:     2026/04/16  17:27:59
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageApp\StorageApp_HTTPHelp.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageApp
//    File Base:   StorageApp_HTTPHelp
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     HTTP 帮助函数封装
//    History:
*********************************************************************/
bool StorageApp_HTTPHelp_Verification(LPCXSTR lpszClientAddr, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount, LPCXSTR lpszRestApi);