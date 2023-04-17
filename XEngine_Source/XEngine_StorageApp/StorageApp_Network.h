#pragma once

bool CALLBACK XEngine_Callback_DownloadLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Callback_DownloadRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK XEngine_Callback_DownloadLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
bool CALLBACK XEngine_Callback_UPLoaderLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Callback_UPLoaderRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK XEngine_Callback_UPLoaderLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
bool CALLBACK XEngine_Callback_CenterLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Callback_CenterRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK XEngine_Callback_CenterLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);

void CALLBACK XEngine_Callback_HBDownload(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
void CALLBACK XEngine_Callback_HBUPLoader(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
void CALLBACK XEngine_Callback_HBCenter(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);

bool XEngine_Net_CloseClient(LPCXSTR lpszClientAddr, int nLeaveType, int nClientType = STORAGE_NETTYPE_HTTPDOWNLOAD);
bool XEngine_Net_SendMsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nType = STORAGE_NETTYPE_HTTPDOWNLOAD);