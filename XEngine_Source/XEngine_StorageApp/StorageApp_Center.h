#pragma once

#define XENGINE_STORAGE_APP_TASK_EVENT  _T("Event")
#define XENGINE_STORAGE_APP_TASK_PASS  _T("Pass")
#define XENGINE_STORAGE_APP_TASK_MANAGE  _T("Manage")

#define XENGINE_STORAGE_APP_METHOD_CONFIG  _T("Config")
#define XENGINE_STORAGE_APP_METHOD_UPFILE  _T("UPFile")
#define XENGINE_STORAGE_APP_METHOD_DLFILE  _T("DLFile")
#define XENGINE_STORAGE_APP_METHOD_FILE  _T("File")
#define XENGINE_STORAGE_APP_METHOD_ADD  _T("Add")
#define XENGINE_STORAGE_APP_METHOD_DEL  _T("Del")
#define XENGINE_STORAGE_APP_METHOD_QUERY  _T("Query")

XHTHREAD CALLBACK XEngine_Center_HTTPThread(LPVOID lParam);
BOOL XEngine_Task_HttpCenter_APIList(LPCTSTR lpszUrlName, TCHAR* ptszAPIVersion, TCHAR* ptszAPIMethod, TCHAR* ptszAPIName);
BOOL XEngine_Task_HttpCenter(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount);