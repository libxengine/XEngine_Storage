#pragma once

XHTHREAD CALLBACK XEngine_Download_HTTPThread(LPVOID lParam);
void CALLBACK XEngine_Download_CBSend(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);

BOOL XEngine_Task_HttpDownload(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount);
BOOL XEngine_Task_SendDownload(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);