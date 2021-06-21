#include "pch.h"
#include "XStorageProtocol_Client.h"
/********************************************************************
//	Created:	2019/6/28   16:15
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Client\XStorageProtocol_Client.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Client
//	File Base:	XStorageProtocol_Client
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	存储客户端协议类
//	History:
*********************************************************************/
CXStorageProtocol_Client::CXStorageProtocol_Client()
{

}
CXStorageProtocol_Client::~CXStorageProtocol_Client()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorageProtocol_Client_REQQueryFile
函数功能：查询请求函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出组好包的请求缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查询文件所属开始时间
 参数.四：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查询文件所属结束时间
 参数.五：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的文件名
 参数.六：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的文件MD5
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQQueryFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileHash /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQQUERY_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    st_JsonRoot["lpszTimeStart"] = lpszTimeStart;
    st_JsonRoot["lpszTimeEnd"] = lpszTimeEnd;
    if (NULL != lpszFileName)
    {
        st_JsonRoot["lpszFileName"] = lpszFileName;
    }
    if (NULL != lpszFileHash)
    {
        st_JsonRoot["lpszFileHash"] = lpszFileHash;
    }
    //打包输出信息
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Client_REQDelete
函数功能：删除文件请求函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出组好包的请求缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要删除的文件名
 参数.四：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQDelete(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszFileName, LPCTSTR lpszFileHash)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQDELETE_PARAMENT;
        return FALSE;
    }
	Json::Value st_JsonRoot;

	st_JsonRoot["lpszFileName"] = lpszFileName;
	st_JsonRoot["lpszFileHash"] = lpszFileHash;
	//打包输出信息
	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Client_REQDirOperator
函数功能：文件夹操作
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出组好包的协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出缓冲区大小
 参数.三：lpszUserDir
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的目录
 参数.四：nOperator
  In/Out：In
  类型：整数型
  可空：N
  意思：1 创建 2 删除 3 查询
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQDirOperator(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUserDir, int nOperator)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszUserDir) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQDIRCREATE_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;

    st_JsonRoot["lpszUserDir"] = lpszUserDir;
    st_JsonRoot["nOperator"] = nOperator;
    //打包输出信息
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return TRUE;
}