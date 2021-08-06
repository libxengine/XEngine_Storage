#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#else
#endif
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>

#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp")
#pragma comment(lib,"Ws2_32")


int main()
{
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	int nHTTPCode = 0;
	int nBodyLen = 2048;
	TCHAR *ptszMsgBody = NULL;
	//请求分布式存储文件所有位置
	LPCTSTR lpszUrl = _T("http://192.168.1.7:5100/EC9B9B75A04F3B323EFD348F9B795539");
	if (!APIHelp_HttpRequest_Get(lpszUrl, &ptszMsgBody, &nBodyLen, &nHTTPCode))
	{
		return -1;
	}
	printf("%s\n", ptszMsgBody);

	BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBody);
	WSACleanup();
	return 0;
}