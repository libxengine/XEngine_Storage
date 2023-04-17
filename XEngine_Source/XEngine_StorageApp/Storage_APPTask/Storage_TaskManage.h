#pragma once

#define XENGINE_STORAGE_APP_METHOD_INSERT _X("Insert")
#define XENGINE_STORAGE_APP_METHOD_DELETE _X("Delete")
#define XENGINE_STORAGE_APP_METHOD_QUERY _X("Query")
#define XENGINE_STORAGE_APP_METHOD_DIR _X("Dir")
#define XENGINE_STORAGE_APP_METHOD_TASK _X("Task")

bool XEngine_Task_Manage(LPCXSTR lpszAPIName, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount);