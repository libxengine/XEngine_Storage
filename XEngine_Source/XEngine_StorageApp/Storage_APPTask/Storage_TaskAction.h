#pragma once

void CALLBACK Storage_TaskAction_Callback(XHANDLE xhToken, double dlTotal, double dlNow, double ulTotal, double ulNow, ENUM_XCLIENT_APIHELP_FILE_STATUS en_DownHttpStatus, XPVOID lParam);
bool Storage_TaskAction(LPCXSTR lpszAPIName, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam);