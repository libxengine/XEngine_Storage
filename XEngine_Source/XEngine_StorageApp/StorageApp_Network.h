#pragma once

bool CALLBACK XEngine_Callback_DownloadLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void CALLBACK XEngine_Callback_DownloadRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void CALLBACK XEngine_Callback_DownloadLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
bool CALLBACK XEngine_Callback_UPLoaderLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void CALLBACK XEngine_Callback_UPLoaderRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void CALLBACK XEngine_Callback_UPLoaderLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
bool CALLBACK XEngine_Callback_CenterLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void CALLBACK XEngine_Callback_CenterRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void CALLBACK XEngine_Callback_CenterLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);

void CALLBACK XEngine_Callback_HBDownload(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);
void CALLBACK XEngine_Callback_HBUPLoader(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);
void CALLBACK XEngine_Callback_HBCenter(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);

BOOL XEngine_Net_CloseClient(LPCTSTR lpszClientAddr, int nLeaveType, int nClientType = STORAGE_NETTYPE_HTTPDOWNLOAD);
BOOL XEngine_Net_SendMsg(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nType = STORAGE_NETTYPE_HTTPDOWNLOAD);