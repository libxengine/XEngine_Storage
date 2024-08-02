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
bool CAPIHelp_Api::APIHelp_Api_ProxyAuth(XCHAR* ptszUser, XCHAR* ptszPass, XCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = false;

	int nAuthType = 0;
	int nAuthLen = MAX_PATH;

	XCHAR tszAuthStr[MAX_PATH];
	XCHAR tszSDBuffer[1024];

	memset(tszAuthStr, '\0', MAX_PATH);
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	//是否有验证信息
	if (!HttpProtocol_ServerHelp_GetAuthInfo(&pptszListHdr, nHdrCount, tszAuthStr, &nAuthLen, &nAuthType))
	{
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTAUTH;
		return false;
	}
	//是否是BASIC
	if (1 != nAuthType)
	{
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTSUPPORT;
		return false;
	}
	OPenSsl_Help_BasicDecoder(tszAuthStr, ptszUser, ptszPass);
	return true;
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
bool CAPIHelp_Api::APIHelp_Api_RangeFile(int* pInt_SPos, int* pInt_EPos, __int64x* pInt_Count, XCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = false;

	bool bFound = false;
	LPCXSTR lpszRangeStr = _X("Range");
	LPCXSTR lpszLengthStr = _X("Content-Length");
	XCHAR tszKeyStr[128];
	XCHAR tszValueStr[128];
	XCHAR tszFieldStr[128];

	memset(tszKeyStr, '\0', sizeof(tszKeyStr));
	memset(tszValueStr, '\0', sizeof(tszValueStr));
	memset(tszFieldStr, '\0', sizeof(tszFieldStr));
	//是否有范围
	if (HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, lpszRangeStr, tszFieldStr))
	{
		//是否没有找到
		int nBPos = 0;  //某些时候有个BYTE   
		if (NULL != _tcsxstr(tszFieldStr, _X("bytes=")))
		{
			nBPos = 6;
		}
		if (!BaseLib_OperatorString_GetKeyValue(tszFieldStr + nBPos, "-", tszKeyStr, tszValueStr))
		{
			APIHelp_IsErrorOccur = true;
			APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARSELEN;
			return false;
		}
		//得到 1/2 如果有的话
		XCHAR tszRangeEnd[128];
		XCHAR tszRangeCount[128];

		memset(tszRangeEnd, '\0', sizeof(tszRangeEnd));
		memset(tszRangeCount, '\0', sizeof(tszRangeCount));
		if (BaseLib_OperatorString_GetKeyValue(tszValueStr, "/", tszRangeEnd, tszRangeCount))
		{
			*pInt_SPos = _ttxoi(tszKeyStr);
			*pInt_EPos = _ttxoi(tszRangeEnd);
			*pInt_Count = _ttxoll(tszRangeCount);
		}
		else
		{
			*pInt_SPos = _ttxoi(tszKeyStr);
			*pInt_EPos = _ttxoi(tszValueStr);
		}
		bFound = true;
	}

	if (0 == *pInt_Count)
	{
		if (HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, lpszLengthStr, tszFieldStr))
		{
			*pInt_Count = _ttxoll(tszFieldStr);
			bFound = true;
		}
	}
	
	if (!bFound)
	{
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTLENGTH;
		return false;
	}
	
	return true;
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
bool CAPIHelp_Api::APIHelp_Api_VerHash(LPCXSTR lpszFileHash, XCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = false;

	LPCXSTR lpszKeyStr = _X("FileHash");
	XCHAR tszValueStr[MAX_PATH];
	memset(tszValueStr, '\0', MAX_PATH);

	if (!HttpProtocol_ServerHelp_GetField(&pptszListHdr, nHdrCount, lpszKeyStr, tszValueStr))
	{
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTHASH;
		return false;
	}
	if (0 != _tcsxnicmp(lpszFileHash, tszValueStr, _tcsxlen(lpszFileHash)))
	{
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTMATCH;
		return false;
	}
	return true;
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
bool CAPIHelp_Api::APIHelp_Api_GetIPInfo(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_IPADDRINFO* pSt_IPAddrInfo)
{
	APIHelp_IsErrorOccur = false;

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//解析
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARSE;
		return false;
	}
	//是否获取成功
	if (0 != st_JsonRoot["code"].asInt())
	{
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_CODE;
		return false;
	}
	Json::Value st_JsonObject = st_JsonRoot["data"];

	_tcsxcpy(pSt_IPAddrInfo->tszIPStart, st_JsonObject["tszIPStart"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPEnd, st_JsonObject["tszIPEnd"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPAddress, st_JsonObject["tszIPAddress"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPAddr, st_JsonObject["tszIPAddr"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPCity, st_JsonObject["tszIPCity"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPCountry, st_JsonObject["tszIPCountry"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPCounty, st_JsonObject["tszIPCounty"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPProvince, st_JsonObject["tszIPProvince"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPISP, st_JsonObject["tszIPISP"].asCString());
	_tcsxcpy(pSt_IPAddrInfo->tszIPTime, st_JsonObject["tszIPTime"].asCString());

	return true;
}
/********************************************************************
函数名称：APIHelp_Api_UrlParse
函数功能：URL参数解析函数
 参数.一：ppptszList
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要解析的列表
 参数.二：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
 参数.三：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件名
 参数.四：ptszKeyName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出存储的bucket
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Api::APIHelp_Api_UrlParse(XCHAR*** ppptszList, int nListCount, XCHAR* ptszFileName, XCHAR* ptszKeyName)
{
	APIHelp_IsErrorOccur = false;

	LPCXSTR lpszHDRFile = _X("filename");
	LPCXSTR lpszHDRKey = _X("storeagekey");

	for (int i = 0; i < nListCount; i++)
	{
		XCHAR tszKey[MAX_PATH];
		XCHAR tszValue[MAX_PATH];

		memset(tszKey, '\0', MAX_PATH);
		memset(tszValue, '\0', MAX_PATH);

		BaseLib_OperatorString_GetKeyValue((*ppptszList)[i], _X("="), tszKey, tszValue);

		if (0 == _tcsxnicmp(lpszHDRFile, tszKey, _tcsxlen(lpszHDRFile)))
		{
			//编码格式是utf8,需要转为ansi
#ifdef _MSC_BUILD
			XCHAR tszFileName[MAX_PATH];
			memset(tszFileName, '\0', MAX_PATH);

			OPenSsl_Codec_UrlDeCodec(tszValue, _tcsxlen(tszValue), tszFileName);

			int nLen = _tcsxlen(tszFileName);
			BaseLib_OperatorCharset_UTFToAnsi(tszFileName, ptszFileName, &nLen);
#else
			OPenSsl_Codec_UrlDeCodec(tszValue, _tcsxlen(tszValue), ptszFileName);
#endif
		}
		else if (0 == _tcsxnicmp(lpszHDRKey, tszKey, _tcsxlen(lpszHDRKey)))
		{
			_tcsxcpy(ptszKeyName, tszValue);
		}
	}
	return true;
}
/********************************************************************
函数名称：APIHelp_Api_Boundary
函数功能：boundary头解析,判断上传方法
 参数.一：ppptszList
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要解析的列表
 参数.二：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
 参数.三：ptszBoundStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出BOUND的字符串
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Api::APIHelp_Api_Boundary(XCHAR*** ppptszList, int nListCount, XCHAR* ptszBoundStr)
{
	APIHelp_IsErrorOccur = false;

	bool bRet = false;
	LPCXSTR lpszHDRContent = _X("Content-Type");
	//Content-Type: multipart/form-data; boundary=AaB03x
	for (int i = 0; i < nListCount; i++)
	{
		XCHAR tszKeyStr[MAX_PATH] = {};
		XCHAR tszVluStr[MAX_PATH] = {};

		BaseLib_OperatorString_GetKeyValue((*ppptszList)[i], _X(": "), tszKeyStr, tszVluStr);

		if (0 == _tcsxnicmp(lpszHDRContent, tszKeyStr, _tcsxlen(lpszHDRContent)))
		{
			XCHAR tszKeySub[MAX_PATH] = {};
			XCHAR tszVluSub[MAX_PATH] = {};
			//multipart/form-data; boundary=AaB03x
			if (BaseLib_OperatorString_GetKeyValue(tszVluStr, _X(";"), tszKeySub, tszVluSub))
			{
				//boundary=AaB03x
				ptszBoundStr[0] = '-';//要少一个字节
				if (BaseLib_OperatorString_GetKeyValue(tszVluSub, _X("="), tszKeyStr, ptszBoundStr + 1))
				{
					bRet = true;
					break;
				}
			}
		}
	}
	return bRet;
}
/********************************************************************
函数名称：APIHelp_Api_GetDIRSize
函数功能：获得目录大小
 参数.一：lpszDIRStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作s的目录
 参数.二：pInt_DIRSize
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出得到的目录大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Api::APIHelp_Api_GetDIRSize(LPCXSTR lpszDIRStr, __int64u* pInt_DIRSize)
{
	APIHelp_IsErrorOccur = false;

	int nListCount = 0;
	int nPathType = 0;
	__int64u nDirCount = 0;   //当前目录大小
	XCHAR** ppListFile;
	XCHAR tszFilePath[MAX_PATH] = {};
	
	_tcsxcpy(tszFilePath, lpszDIRStr);
	BaseLib_OperatorString_GetPath(tszFilePath, &nPathType);
	//判断是绝对路径还是相对路径
	if (1 == nPathType)
	{
		_tcsxcat(tszFilePath, _X("\\*"));
	}
	else if (2 == nPathType)
	{
		_tcsxcat(tszFilePath, _X("/*"));
	}
	SystemApi_File_EnumFile(tszFilePath, &ppListFile, &nListCount, true, 1);
	for (int i = 0; i < nListCount; i++)
	{
		struct _xtstat st_FStat;
		_xtstat(ppListFile[i], &st_FStat);
		nDirCount += st_FStat.st_size;
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppListFile, nListCount);
	*pInt_DIRSize = nDirCount;
	return true;
}
/********************************************************************
函数名称：APIHelp_Api_UrlStr
函数功能：获取URL的KEY
 参数.一：ptszKeyStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的数据
 参数.二：lpszUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Api::APIHelp_Api_UrlStr(XCHAR* ptszKeyStr, LPCXSTR lpszUrl)
{
	APIHelp_IsErrorOccur = false;

	XCHAR tszUrlStr[MAX_PATH] = {};
	_tcsxcpy(tszUrlStr, lpszUrl);
	// 查找第一个 '/' 的位置
	XCHAR *ptszFirstStr = _tcsxchr(tszUrlStr, '/');
	if (ptszFirstStr == NULL) 
	{
		return false;
	}
	// 查找第二个 '/' 的位置
	XCHAR* ptszSecondStr = _tcsxchr(ptszFirstStr + 1, '/');
	if (ptszSecondStr == NULL)
	{
		return false;
	}
	// 计算提取字符串的长度
	int nLen = ptszSecondStr - ptszFirstStr - 1;
	// 复制字符串
	_tcsxncpy(ptszKeyStr, ptszFirstStr + 1, nLen);
	// 添加字符串结束符
	ptszKeyStr[nLen] = '\0';

	return true;
}
/********************************************************************
函数名称：APIHelp_Api_UrlChange
函数功能：URL修改
 参数.一：ptszStr
  In/Out：In/Out
  类型：字符指针
  可空：N
  意思：输出修改后的内容,输入修改前的内容
 参数.二：lpszSourceStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要替换的字符串
 参数.三：lpszDestStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入修改后的字符串
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Api::APIHelp_Api_UrlChange(XCHAR* ptszStr, LPCXSTR lpszSourceStr, LPCXSTR lpszDestStr)
{
	APIHelp_IsErrorOccur = false;

	std::string m_Str = ptszStr;
	std::string m_StrSource = lpszSourceStr;
	std::string m_StrDest = lpszDestStr;

	size_t startPos = 0;
	while ((startPos = m_Str.find(m_StrSource, startPos)) != std::string::npos)
	{
		m_Str.replace(startPos, m_StrSource.length(), m_StrDest);
		startPos += m_StrDest.length(); // 防止在替换新字符串后陷入无限循环
	}
	_tcsxcpy(ptszStr, m_Str.c_str());

	return true;
}