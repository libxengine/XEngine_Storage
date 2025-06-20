#pragma once

XHTHREAD XCALLBACK XEngine_UPLoader_HTTPThread(XPVOID lParam);
void XCALLBACK XEngine_UPLoader_UPFlow(XHANDLE xhToken, bool bSDFlow, bool bRVFlow, bool bTime, XPVOID lParam);
bool XEngine_Task_HttpUPLoader(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount, int nNetType);