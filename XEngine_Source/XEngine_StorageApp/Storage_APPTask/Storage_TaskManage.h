#pragma once

#define XENGINE_STORAGE_APP_METHOD_ADD _T("Add")
#define XENGINE_STORAGE_APP_METHOD_DEL _T("Del")
#define XENGINE_STORAGE_APP_METHOD_QUERYFILE _T("Query")
#define XENGINE_STORAGE_APP_METHOD_INFO _T("Info")
#define XENGINE_STORAGE_APP_METHOD_DIR _T("Dir")

BOOL XEngine_Task_Manage(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount);