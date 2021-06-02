#pragma once

XHTHREAD CALLBACK XEngine_Download_Thread(LPVOID lParam);
BOOL XEngine_Task_Download(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);