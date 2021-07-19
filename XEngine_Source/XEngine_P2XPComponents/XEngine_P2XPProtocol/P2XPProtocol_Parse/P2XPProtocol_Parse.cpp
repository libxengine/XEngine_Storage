#include "pch.h"
#include "P2XPProtocol_Parse.h"
/********************************************************************
//    Created:     2021/07/19  09:36:54
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPProtocol\P2XPProtocol_Parse\P2XPProtocol_Parse.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPProtocol\P2XPProtocol_Parse
//    File Base:   P2XPProtocol_Parse
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     解析类
//    History:
*********************************************************************/
CP2XPProtocol_Parse::CP2XPProtocol_Parse()
{

}
CP2XPProtocol_Parse::~CP2XPProtocol_Parse()
{

}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：P2XPProtocol_Parse_Login
函数功能：登录解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：pSt_P2XPPeer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输出解析后的节点信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Parse::P2XPProtocol_Parse_Login(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPPEER_PROTOCOL *pSt_P2XPPeer)
{
	P2XPProtocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_P2XPPeer))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

	if (!st_JsonRoot["tszUserName"].isNull())
	{
		_tcscpy(pSt_P2XPPeer->tszUserName, st_JsonRoot["tszUserName"].asCString());
	}
	if (!st_JsonRoot["tszPrivateAddr"].isNull())
	{
		_tcscpy(pSt_P2XPPeer->tszPrivateAddr, st_JsonRoot["tszPrivateAddr"].asCString());
	}
	if (!st_JsonRoot["tszPublicAddr"].isNull())
	{
		_tcscpy(pSt_P2XPPeer->tszPublicAddr, st_JsonRoot["tszPublicAddr"].asCString());
	}
	if (!st_JsonRoot["dwConnectType"].isNull())
	{
		pSt_P2XPPeer->dwConnectType = st_JsonRoot["dwConnectType"].asInt();
	}
	return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Parse_List
函数功能：解析列表请求
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：ptszPubAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出公有地址
 参数.四：ptszPriAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出私有地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Parse::P2XPProtocol_Parse_List(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszPubAddr, TCHAR* ptszPriAddr)
{
	P2XPProtocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == ptszPubAddr) || (NULL == ptszPriAddr))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

	if (!st_JsonRoot["tszPublicAddr"].isNull())
	{
		_tcscpy(ptszPubAddr, st_JsonRoot["tszPublicAddr"].asCString());
	}
	if (!st_JsonRoot["tszPrivateAddr"].isNull())
	{
		_tcscpy(ptszPriAddr, st_JsonRoot["tszPrivateAddr"].asCString());
	}
	return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Parse_List
函数功能：解析列表请求
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：ptszUserName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出查询的用户名
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Parse::P2XPProtocol_Parse_User(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszUserName)
{
	P2XPProtocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == ptszUserName))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

	if (!st_JsonRoot["tszUserName"].isNull())
	{
		_tcscpy(ptszUserName, st_JsonRoot["tszUserName"].asCString());
	}
	return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Parse_Connect
函数功能：请求连接到指定地址
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：pSt_P2XPPeer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输出解析后的节点信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Parse::P2XPProtocol_Parse_Connect(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPIO_PROTOCOL* pSt_IOProtocol)
{
	P2XPProtocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_IOProtocol))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

	if (!st_JsonRoot["tszSourceUser"].isNull())
	{
		_tcscpy(pSt_IOProtocol->tszSourceUser, st_JsonRoot["tszSourceUser"].asCString());
	}
	if (!st_JsonRoot["tszDestUser"].isNull())
	{
		_tcscpy(pSt_IOProtocol->tszDestUser, st_JsonRoot["tszDestUser"].asCString());
	}
	if (!st_JsonRoot["tszConnectAddr"].isNull())
	{
		_tcscpy(pSt_IOProtocol->tszConnectAddr, st_JsonRoot["tszConnectAddr"].asCString());
	}
	if (!st_JsonRoot["nDestPort"].isNull())
	{
		pSt_IOProtocol->nDestPort = st_JsonRoot["nDestPort"].asInt();
	}
	if (!st_JsonRoot["bIsTcp"].isNull())
	{
		pSt_IOProtocol->bIsTcp = st_JsonRoot["bIsTcp"].asInt();
	}
	return TRUE;
}