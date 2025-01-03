#include "pch.h"
#include "APIHelp_Cryption.h"
/********************************************************************
//    Created:     2024/10/29  11:43:48
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Cryption\APIHelp_Cryption.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Cryption
//    File Base:   APIHelp_Cryption
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     加解密帮助函数
//    History:
*********************************************************************/
CAPIHelp_Cryption::CAPIHelp_Cryption()
{
}
CAPIHelp_Cryption::~CAPIHelp_Cryption()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：APIHelp_Cryption_BasicEncoder
函数功能：HTTP基本验证加密函数
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.三：ptszMsgBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出加密后的缓冲区
 参数.四：bADD
  In/Out：In
  类型：逻辑型
  可空：N
  意思：是否添加验证算法类型
返回值
  类型：逻辑型
  意思：是否成功
备注：格式:Basic BASE64
*********************************************************************/
bool CAPIHelp_Cryption::APIHelp_Cryption_BasicEncoder(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszMsgBuffer, bool bADD /* = true */)
{
    APIHelp_IsErrorOccur = false;

	if ((NULL == lpszUser) || (NULL == lpszPass) || (NULL == ptszMsgBuffer))
	{
        APIHelp_IsErrorOccur = true;
        APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
        return false;
	}
    XCHAR tszMsgBuffer[2048];
	XCHAR tszBaseBuffer[2048];

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
    memset(tszBaseBuffer, '\0', sizeof(tszBaseBuffer));

    int nLen = _xstprintf(tszMsgBuffer, _X("%s:%s"), lpszUser, lpszPass);
	if (!Cryption_Codec_Base64(tszMsgBuffer, tszBaseBuffer, &nLen))
	{
		return false;
	}
    if (bADD)
    {
        _xstprintf(ptszMsgBuffer, _X("Basic %s"), tszBaseBuffer);
    }
    else
    {
        _tcsxcpy(ptszMsgBuffer, tszBaseBuffer);
    }
    return true;
}
/********************************************************************
函数名称：APIHelp_Cryption_BasicDecoder
函数功能：HTTP基本验证解密函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解密的缓冲区
 参数.二：ptszUser
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出用户名
 参数.三：ptszPass
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出密码
返回值
  类型：逻辑型
  意思：是否成功
备注：HttpProtocol_ServerHelp_GetAuthInfo 返回的内容
      格式:Basic BASE64
*********************************************************************/
bool CAPIHelp_Cryption::APIHelp_Cryption_BasicDecoder(LPCXSTR lpszMsgBuffer, XCHAR* ptszUser, XCHAR* ptszPass)
{
    APIHelp_IsErrorOccur = false;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszUser) || (NULL == ptszPass))
    {
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return false;
    }
    XCHAR tszBaseBuffer[2048];
    memset(tszBaseBuffer, '\0', sizeof(tszBaseBuffer));
    //是否有Basic
    if (NULL == _tcsxstr(lpszMsgBuffer, _X("Basic")))
    {
		int nMsgLen = _tcsxlen(lpszMsgBuffer);
		if (!Cryption_Codec_Base64(lpszMsgBuffer, tszBaseBuffer, &nMsgLen, false))
		{
			return false;
		}
    }
    else
    {
		int nMsgLen = _tcsxlen(lpszMsgBuffer) - 6;
		if (!Cryption_Codec_Base64(lpszMsgBuffer + 6, tszBaseBuffer, &nMsgLen, false))
		{
			return false;
		}
    }
	//分割用户名和密码
	XCHAR* ptszPassString = NULL;
	XCHAR* ptszUserString = _tcsxtok_s(tszBaseBuffer, _X(":"), &ptszPassString);
	if (NULL == ptszUserString)
	{
        APIHelp_IsErrorOccur = true;
        APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_FORMAT;
		return false;
	}
	_tcsxcpy(ptszUser, ptszUserString);
	_tcsxcpy(ptszPass, ptszPassString);

    return true;
}
/********************************************************************
函数名称：APIHelp_Cryption_Digest
函数功能：摘要计算函数
 参数.一：ptszResponseStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出计算后的RESPONSE字符串
 参数.二：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.三：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.四：lpszRealm
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入作用域,服务器发送的
 参数.五：lpszMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求的方法
 参数.六：lpszUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求的URL地址
 参数.七：lpszNonce
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：服务器生成并且发送的
 参数.八：lpszCNonce
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：客户端生成的随机字符串
 参数.九：lpszNC
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：验证的次数,建议为NULL.
返回值
  类型：逻辑型
  意思：是否成功
备注：信息摘要支持QOP=AUTH 算法MD5的验证模式
*********************************************************************/
bool CAPIHelp_Cryption::APIHelp_Cryption_Digest(XCHAR* ptszResponseStr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszRealm, LPCXSTR lpszMethod, LPCXSTR lpszUrl, LPCXSTR lpszNonce, LPCXSTR lpszCNonce, LPCXSTR lpszNC /* = NULL */)
{
    APIHelp_IsErrorOccur = false;

    XCHAR tszResponseStr[2048];
    XBYTE tszResponseMD5[2048];
    XBYTE tszUserMD5[MAX_PATH];
    XBYTE tszUrlMD5[MAX_PATH];
    XCHAR tszUserStr[MAX_PATH];
    XCHAR tszUrlStr[MAX_PATH];
    XCHAR tszUserHex[MAX_PATH];
    XCHAR tszUrlHex[MAX_PATH];

    memset(tszResponseStr, '\0', sizeof(tszResponseStr));
    memset(tszResponseMD5, '\0', sizeof(tszResponseMD5));
    memset(tszUserMD5, '\0', MAX_PATH);
    memset(tszUrlMD5, '\0', MAX_PATH);
    memset(tszUserStr, '\0', MAX_PATH);
    memset(tszUrlStr, '\0', MAX_PATH);
    memset(tszUserHex, '\0', MAX_PATH);
    memset(tszUrlHex, '\0', MAX_PATH);

	//计算HA1 的MD5字符串 MD5(USER:PASS:REAL)
	int nRet = _xstprintf(tszUserStr, _X("%s:%s:%s"), lpszUser, lpszRealm, lpszPass);
    Cryption_Api_Digest(tszUserStr, tszUserMD5, &nRet);
	BaseLib_String_StrToHex((XCHAR*)tszUserMD5, nRet, tszUserHex);
	//计算HA2 的MD5字符串,根据验证模式来计算
    nRet = _xstprintf(tszUrlStr, _X("%s:%s"), lpszMethod, lpszUrl);
    Cryption_Api_Digest(tszUrlStr, tszUrlMD5, &nRet);
	BaseLib_String_StrToHex((XCHAR*)tszUrlMD5, nRet, tszUrlHex);
	//计算RESPONSE值
    nRet = _xstprintf(tszResponseStr, _X("%s:%s:%s:%s:auth:%s"), tszUserHex, lpszNonce, lpszNC, lpszCNonce, tszUrlHex);
    Cryption_Api_Digest(tszResponseStr, tszResponseMD5, &nRet);
	BaseLib_String_StrToHex((XCHAR*)tszResponseMD5, nRet, ptszResponseStr);
    return true;
}