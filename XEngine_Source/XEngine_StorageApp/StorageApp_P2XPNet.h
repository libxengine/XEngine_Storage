#pragma once

XHTHREAD CALLBACK XEngine_P2XP_TCPThread(LPVOID lParam);
BOOL XEngine_Task_TCPP2xp(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);