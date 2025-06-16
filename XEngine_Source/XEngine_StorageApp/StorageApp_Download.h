#pragma once

XHTHREAD XCALLBACK XEngine_Download_HTTPThread(XPVOID lParam);
void XCALLBACK XEngine_Download_CBSend(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);

bool XEngine_Task_HttpDownload(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount, int nNetType);
bool XEngine_Task_SendDownload(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nNetType);