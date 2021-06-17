#pragma once

XHTHREAD CALLBACK XEngine_Center_HTTPThread(LPVOID lParam);
BOOL XEngine_Task_HttpCenter_APIList(LPCTSTR lpszUrlName, TCHAR* ptszAPIVersion, TCHAR* ptszAPIMethod, TCHAR* ptszAPIName);
BOOL XEngine_Task_HttpCenter(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam);