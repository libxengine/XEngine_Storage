#pragma once
#define XENGINE_STORAGE_APP_TASK_PASS  _X("Pass")
#define XENGINE_STORAGE_APP_TASK_MANAGE  _X("Manage")

#define XENGINE_STORAGE_APP_METHOD_CONFIG  _X("Config")
#define XENGINE_STORAGE_APP_METHOD_UPFILE  _X("UPFile")
#define XENGINE_STORAGE_APP_METHOD_DLFILE  _X("DLFile")

XHTHREAD CALLBACK XEngine_Center_HTTPThread(XPVOID lParam);
bool XEngine_Task_HttpCenter(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount);