#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine_Source/x64/Debug/jsoncpp")
#else
#pragma comment(lib,"../../../XEngine_Source/Debug/jsoncpp")
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string>
#include <list>
#include <thread>
#include <json/json.h>
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>

typedef struct
{
	XCHAR tszModulePath[MAX_PATH];                                         //模块路径,新增加的必须拥有此值
	XCHAR tszModuleName[MAX_PATH];                                         //模块名称
	XCHAR tszModuleDownload[MAX_PATH];                                     //模块下载地址
	XCHAR tszModuleCode[MAX_PATH];                                         //远程模块标识符
	__int64x nModuleVersion;                                               //文件版本号
	bool bIsRun;                                                          //安装完成是否运行
	struct
	{
		__int64x nModuleVersion;
		XCHAR tszMoudelPath[MAX_PATH];
		XCHAR tszMoudelName[MAX_PATH];
		XCHAR tszMoudelCode[MAX_PATH];
	}st_LocalVersion;
}FILEPARSER_VERSIONINFO, * LPFILEPARSER_VERSIONINFO;