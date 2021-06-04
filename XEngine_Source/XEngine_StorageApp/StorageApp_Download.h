#pragma once

XHTHREAD CALLBACK XEngine_Download_HTTPThread(LPVOID lParam);
XHTHREAD CALLBACK XEngine_Download_SendThread(LPVOID lParam);

BOOL XEngine_Task_HttpDownload(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam);
BOOL XEngine_Task_SendDownload(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);