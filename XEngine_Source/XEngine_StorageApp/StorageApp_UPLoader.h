#pragma once

XHTHREAD CALLBACK XEngine_UPLoader_HTTPThread(LPVOID lParam);
BOOL XEngine_Task_HttpUPLoader(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount);