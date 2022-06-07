#pragma once
#define XENGINE_STORAGE_APP_TASK_PASS  _T("Pass")
#define XENGINE_STORAGE_APP_TASK_MANAGE  _T("Manage")

#define XENGINE_STORAGE_APP_METHOD_CONFIG  _T("Config")
#define XENGINE_STORAGE_APP_METHOD_UPFILE  _T("UPFile")
#define XENGINE_STORAGE_APP_METHOD_DLFILE  _T("DLFile")

XHTHREAD CALLBACK XEngine_Center_HTTPThread(LPVOID lParam);
BOOL XEngine_Task_HttpCenter_APIList(LPCTSTR lpszUrlName, TCHAR* ptszAPIVersion, TCHAR* ptszAPIMethod, TCHAR* ptszAPIName);
BOOL XEngine_Task_HttpCenter(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount);