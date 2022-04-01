#pragma once

#define XENGINE_STORAGE_APP_METHOD_INSERT _T("Insert")
#define XENGINE_STORAGE_APP_METHOD_DELETE _T("Delete")
#define XENGINE_STORAGE_APP_METHOD_QUERY _T("Query")
#define XENGINE_STORAGE_APP_METHOD_DIR _T("Dir")
#define XENGINE_STORAGE_APP_METHOD_TASK _T("Task")

BOOL XEngine_Task_Manage(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount);