#pragma once

XHTHREAD CALLBACK XEngine_UPLoader_HTTPThread(XPVOID lParam);
bool XEngine_Task_HttpUPLoader(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount);