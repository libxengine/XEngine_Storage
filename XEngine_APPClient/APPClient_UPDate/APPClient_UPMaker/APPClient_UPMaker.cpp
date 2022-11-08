#pragma once
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine_Source/Debug/jsoncpp")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")
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
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>

typedef struct 
{
	TCHAR tszFilePath[MAX_PATH];
	TCHAR tszFileName[MAX_PATH];
}HELPMODULE_FILELIST, * LPHELPMODULE_FILELIST;
/********************************************************************
函数名称：HelpModule_Api_BuildVer
函数功能：构建应用更新文件
 参数.一：lpszPath
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要构建哪个目录中的所有文件列表
 参数.二：lpszLocalFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：构建后保存的列表文件地址
 参数.三：lpszRemoteFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：更新匹配文件保存地址,这个文件需要上传的服务器用来与lpszLocalFile判断是否有更新
 参数.四：lpszDlUrl
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：更新地址,用 http://www.xxx.com/UPFile/ 表示，我们会自动填充后面的文件名和路径
 参数.五：bSubDir
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否包含子目录，是否把子目录下的所有文件都构建成更新包
返回值
  类型：逻辑型
  意思：是否构建成功
备注：次函数可以帮助你快速构建一个可更新的文件列表，方便你快速使用我们的更新SDK
*********************************************************************/
BOOL HelpModule_Api_BuildVer(TCHAR* ptszLocalBuffer, TCHAR* ptszRemoteBuffer, int* pInt_LocalLen, int* pInt_RemoteLen, LPCTSTR lpszPath, LPCTSTR lpszDlUrl, BOOL bSubDir = TRUE)
{
	int nListCount;
	CHAR** ppszListDir;
	list<HELPMODULE_FILELIST> stl_ListFile;
	//枚举文件
	if (!SystemApi_File_EnumFile(lpszPath, &ppszListDir, &nListCount, NULL, NULL, bSubDir, 1))
	{
		return FALSE;
	}
	for (int i = 0; i < nListCount; i++)
	{
		HELPMODULE_FILELIST st_FileList;
		memset(&st_FileList, '\0', sizeof(HELPMODULE_FILELIST));

		if (!BaseLib_OperatorString_GetFileAndPath(ppszListDir[i], st_FileList.tszFilePath, st_FileList.tszFileName))
		{
			return FALSE;
		}
		stl_ListFile.push_back(st_FileList);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListDir, nListCount);

	Json::Value st_JsonLocalRoot;
	Json::Value st_JsonLocalArray;
	Json::Value st_JsonLocalObject;

	Json::Value st_JsonRemoteRoot;
	Json::Value st_JsonRemoteArray;
	Json::Value st_JsonRemoteObject;
	Json::Value st_JsonRemoteOPtion;
	Json::StreamWriterBuilder st_JsonBuilder;
	//判断是否是自定义版本
	TCHAR tszTimer[64];
	XENGINE_LIBTIMER st_Timer;

	memset(tszTimer, '\0', sizeof(tszTimer));
	memset(&st_Timer, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_OperatorTime_GetSysTime(&st_Timer);
	_stprintf_s(tszTimer, _T("%04d%02d%02d%02d%02d%02d"), st_Timer.wYear, st_Timer.wMonth, st_Timer.wDay, st_Timer.wHour, st_Timer.wMinute, st_Timer.wSecond);
	__int64x m_nFileVer = _ttoi64(tszTimer);

	st_JsonLocalRoot["MainVersion"] = (Json::UInt64)m_nFileVer;

	st_JsonRemoteOPtion["st_JsonRemoteOPtion"] = 0;
	st_JsonRemoteRoot["MainVersion"] = (Json::UInt64)m_nFileVer;
	st_JsonRemoteRoot["MainDescription"] = _T("File UPData Des!");
	st_JsonRemoteRoot["FileVerOPtion"] = st_JsonRemoteOPtion;
	//开始构架JSON文件列表
	list<HELPMODULE_FILELIST>::const_iterator stl_ListIterator = stl_ListFile.begin();
	for (unsigned int i = 1; stl_ListIterator != stl_ListFile.end(); stl_ListIterator++, i++)
	{
		TCHAR tszFileCode[64];
		memset(tszFileCode, '\0', sizeof(tszFileCode));

		_stprintf_s(tszFileCode, _T("XYRYUPVERCODE%d"), i);

		st_JsonLocalObject["ModuleVersion"] = (Json::UInt64)m_nFileVer;
		st_JsonLocalObject["ModuleCode"] = tszFileCode;
		st_JsonLocalObject["ModuleName"] = stl_ListIterator->tszFileName;
		st_JsonLocalObject["ModulePath"] = stl_ListIterator->tszFilePath;
		TCHAR tszDlPath[1024];
		memset(tszDlPath, '\0', sizeof(tszDlPath));
		//删除指定字符串
		int nPathType = 0;
		BaseLib_OperatorString_GetPath(stl_ListIterator->tszFilePath, &nPathType);
		if (1 == nPathType)
		{
			TCHAR tszDelPath[MAX_PATH];
			TCHAR tszSubPath[MAX_PATH];

			memset(tszDelPath, '\0', MAX_PATH);
			memset(tszSubPath, '\0', MAX_PATH);

			_tcscpy(tszDelPath, lpszPath);
			_tcscpy(tszSubPath, stl_ListIterator->tszFilePath);

			tszDelPath[_tcslen(tszDelPath) - 2] = '\0';
			BaseLib_OperatorString_DelLastForChar(tszDelPath, '\\');
			_tcscat(tszDelPath, "\\");

			BaseLib_OperatorString_DelSub(tszSubPath, tszDelPath);
			BaseLib_OperatorString_FixPath(tszSubPath, 2);
			_stprintf_s(tszDlPath, _T("%s%s%s"), lpszDlUrl, tszSubPath, stl_ListIterator->tszFileName);
		}
		else
		{
			_stprintf_s(tszDlPath, _T("%s%s%s"), lpszDlUrl, stl_ListIterator->tszFilePath + 2, stl_ListIterator->tszFileName);
		}
		st_JsonRemoteObject["ModuleRun"] = false;
		st_JsonRemoteObject["ModuleVersion"] = (Json::UInt64)m_nFileVer;
		st_JsonRemoteObject["ModuleCode"] = tszFileCode;
		st_JsonRemoteObject["ModuleName"] = stl_ListIterator->tszFileName;
		st_JsonRemoteObject["ModuleDownload"] = tszDlPath;
		st_JsonRemoteArray.append(st_JsonRemoteObject);
		st_JsonLocalArray.append(st_JsonLocalObject);
	}
	st_JsonLocalRoot["FileList"] = st_JsonLocalArray;

	Json::Value st_JsonRemoteDel;
	st_JsonRemoteRoot["UPList"] = st_JsonRemoteArray;
	st_JsonRemoteRoot["DelList"] = st_JsonRemoteDel;
	st_JsonBuilder["emitUTF8"] = true;
	//写到JSON文件
	*pInt_LocalLen = Json::writeString(st_JsonBuilder, st_JsonLocalRoot).length();
	*pInt_RemoteLen = Json::writeString(st_JsonBuilder, st_JsonRemoteRoot).length();
	memcpy(ptszLocalBuffer, Json::writeString(st_JsonBuilder, st_JsonLocalRoot).c_str(), *pInt_LocalLen);
	memcpy(ptszRemoteBuffer, Json::writeString(st_JsonBuilder, st_JsonRemoteRoot).c_str(), *pInt_RemoteLen);

	return TRUE;
}

int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	LPCTSTR lpszPath = _T("D:\\libxengine\\*");
	LPCTSTR lpszLocalFile = _T("D:\\XEngine_Storage\\XEngine_APPClient\\Debug\\LocalFile.txt");
	LPCTSTR lpszPostFile = _T("http://192.168.1.8:5102/api?filename=xengine/upfile.txt&storeagekey=storagekey1");
	LPCTSTR lpszDownload = _T("http://192.168.1.8:5101/storagekey1/xengine/");
	int nLocalLen = 1024 * 1024 * 10;
	int nRemoteLen = 1024 * 1024 * 10;
	TCHAR* ptszLocalBuffer = (TCHAR*)malloc(nLocalLen);
	TCHAR* ptszRemoteBuffer = (TCHAR*)malloc(nRemoteLen);

	memset(ptszLocalBuffer, '\0', nLocalLen);
	memset(ptszRemoteBuffer, '\0', nRemoteLen);

	if (!HelpModule_Api_BuildVer(ptszLocalBuffer, ptszRemoteBuffer, &nLocalLen, &nRemoteLen, lpszPath, lpszDownload))
	{
		printf("erron\n");
		return 0;
	}
	SystemApi_File_SaveBuffToFile(lpszLocalFile, ptszLocalBuffer, nLocalLen);
	//如果服务器支持,可以直接把更新数据提交到服务器
	if (!APIHelp_HttpRequest_Custom(_T("POST"), lpszPostFile, ptszRemoteBuffer, NULL, NULL, &nRemoteLen))
	{
		printf("erron\n");
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}