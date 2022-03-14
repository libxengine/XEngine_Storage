#include "pch.h"
#include "APIHelp_Api.h"
/********************************************************************
//    Created:     2022/01/07  14:36:59
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Api\APIHelp_Api.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Api
//    File Base:   APIHelp_Api
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     帮助函数
//    History:
*********************************************************************/
CAPIHelp_Api::CAPIHelp_Api()
{

}
CAPIHelp_Api::~CAPIHelp_Api()
{

}
//////////////////////////////////////////////////////////////////////////
//                               公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：APIHelp_Api_ProxyAuth
函数功能：代理验证
 参数.一：ptszUser
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出用户名
 参数.二：ptszPass
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出密码
 参数.三：pptszListHdr
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入要解析的HTTP头
 参数.四：nHdrCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的HTTP头列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Api::APIHelp_Api_ProxyAuth(TCHAR* ptszUser, TCHAR* ptszPass, TCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = FALSE;

	int nAuthType = 0;
	int nAuthLen = MAX_PATH;

	TCHAR tszAuthStr[MAX_PATH];
	TCHAR tszSDBuffer[1024];

	memset(tszAuthStr, '\0', MAX_PATH);
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	//是否有验证信息
	if (!RfcComponents_HttpHelp_GetAuthInfo(&pptszListHdr, nHdrCount, tszAuthStr, &nAuthLen, &nAuthType))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTAUTH;
		return FALSE;
	}
	//是否是BASIC
	if (1 != nAuthType)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTSUPPORT;
		return FALSE;
	}
	OPenSsl_Help_BasicDecoder(tszAuthStr, ptszUser, ptszPass);
	return TRUE;
}
/********************************************************************
函数名称：APIHelp_Api_RangeFile
函数功能：获取HTTP的范围
 参数.一：pInt_SPos
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出起始范围
 参数.二：pInt_EPos
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出结束范围
 参数.三：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出总大小
 参数.四：pptszListHdr
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入HTTP协议头列表
 参数.五：nHdrCount
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Api::APIHelp_Api_RangeFile(int* pInt_SPos, int* pInt_EPos, __int64x* pInt_Count, TCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = FALSE;

	BOOL bFound = FALSE;
	LPCTSTR lpszRangeStr = _T("Range");
	LPCTSTR lpszLengthStr = _T("Content-Length");
	TCHAR tszKeyStr[128];
	TCHAR tszValueStr[128];
	TCHAR tszFieldStr[128];

	memset(tszKeyStr, '\0', sizeof(tszKeyStr));
	memset(tszValueStr, '\0', sizeof(tszValueStr));
	memset(tszFieldStr, '\0', sizeof(tszFieldStr));
	//是否有范围
	if (RfcComponents_HttpHelp_GetField(&pptszListHdr, nHdrCount, lpszRangeStr, tszFieldStr))
	{
		//是否没有找到
		int nBPos = 0;  //某些时候有个BYTE   
		if (NULL != _tcsstr(tszFieldStr, _T("bytes=")))
		{
			nBPos = 6;
		}
		if (!BaseLib_OperatorString_GetKeyValue(tszFieldStr + nBPos, "-", tszKeyStr, tszValueStr))
		{
			APIHelp_IsErrorOccur = TRUE;
			APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARSELEN;
			return FALSE;
		}
		//得到 1/2 如果有的话
		TCHAR tszRangeEnd[128];
		TCHAR tszRangeCount[128];

		memset(tszRangeEnd, '\0', sizeof(tszRangeEnd));
		memset(tszRangeCount, '\0', sizeof(tszRangeCount));
		if (BaseLib_OperatorString_GetKeyValue(tszValueStr, "/", tszRangeEnd, tszRangeCount))
		{
			*pInt_SPos = _ttoi(tszKeyStr);
			*pInt_EPos = _ttoi(tszRangeEnd);
			*pInt_Count = _ttoi64(tszRangeCount);
		}
		else
		{
			*pInt_SPos = _ttoi(tszKeyStr);
			*pInt_EPos = _ttoi(tszValueStr);
		}
		bFound = TRUE;
	}

	if (0 == *pInt_Count)
	{
		if (RfcComponents_HttpHelp_GetField(&pptszListHdr, nHdrCount, lpszLengthStr, tszFieldStr))
		{
			*pInt_Count = _ttoi64(tszFieldStr);
			bFound = TRUE;
		}
	}
	
	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTLENGTH;
		return FALSE;
	}
	
	return TRUE;
}
/********************************************************************
函数名称：APIHelp_Api_VerHash
函数功能：验证HASH值
 参数.一：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要比对的HASH值
 参数.二：pptszListHdr
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入HTTP协议头列表
 参数.三：nHdrCount
  In/Out：In
  类型：指向指针的指针
  可空：N
  意思：输入列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Api::APIHelp_Api_VerHash(LPCTSTR lpszFileHash, TCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = FALSE;

	LPCTSTR lpszKeyStr = _T("FileHash");
	TCHAR tszValueStr[MAX_PATH];
	memset(tszValueStr, '\0', MAX_PATH);

	if (!RfcComponents_HttpHelp_GetField(&pptszListHdr, nHdrCount, lpszKeyStr, tszValueStr))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTHASH;
		return FALSE;
	}
	if (0 != _tcsnicmp(lpszFileHash, tszValueStr, _tcslen(lpszFileHash)))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTMATCH;
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：APIHelp_Api_GetIPInfo
函数功能：获取IP信息
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的大小
 参数.三：pSt_IPAddrInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的IP信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Api::APIHelp_Api_GetIPInfo(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_IPADDRINFO* pSt_IPAddrInfo)
{
	APIHelp_IsErrorOccur = FALSE;

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//解析
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARSE;
		return FALSE;
	}
	//是否获取成功
	if (0 != st_JsonRoot["code"].asInt())
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_CODE;
		return FALSE;
	}
	Json::Value st_JsonObject = st_JsonRoot["data"];

	_tcscpy(pSt_IPAddrInfo->tszIPStart, st_JsonObject["tszIPStart"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPEnd, st_JsonObject["tszIPEnd"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPAddress, st_JsonObject["tszIPAddress"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPAddr, st_JsonObject["tszIPAddr"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPCity, st_JsonObject["tszIPCity"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPCountry, st_JsonObject["tszIPCountry"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPCounty, st_JsonObject["tszIPCounty"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPProvince, st_JsonObject["tszIPProvince"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPISP, st_JsonObject["tszIPISP"].asCString());
	_tcscpy(pSt_IPAddrInfo->tszIPTime, st_JsonObject["tszIPTime"].asCString());

	return TRUE;
}