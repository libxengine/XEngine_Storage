#include "../StorageApp_Hdr.h"

BOOL XEngine_Task_P2P(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_LOGIN, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_LOGIN)))
	{
	}
	return TRUE;
}