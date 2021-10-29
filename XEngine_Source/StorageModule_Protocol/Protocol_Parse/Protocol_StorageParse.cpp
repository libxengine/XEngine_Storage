#include "pch.h"
#include "Protocol_StorageParse.h"
/********************************************************************
//	Created:	2019/6/28   15:05
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Core\XStorageProtocol_Core.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Core
//	File Base:	XStorageProtocol_Core
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	协议处理函数
//	History:
*********************************************************************/
CProtocol_StorageParse::CProtocol_StorageParse()
{

}
CProtocol_StorageParse::~CProtocol_StorageParse()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Protocol_StorageParse_QueryFile
函数功能：文件查询请求解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要解析的缓冲区
 参数.二：ptszTimeStart
  In/Out：Out
  类型：字符指针
  可空：N
  意思：查询开始时间
 参数.三：ptszTimeEnd
  In/Out：Out
  类型：字符指针
  可空：N
  意思：查询结束时间
 参数.四：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：查询的文件名
 参数.五：ptszFileHash
  In/Out：Out
  类型：字符指针
  可空：N
  意思：查询的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_QueryFile(LPCTSTR lpszMsgBuffer, TCHAR *ptszTimeStart, TCHAR *ptszTimeEnd, TCHAR *ptszFileName /* = NULL */, TCHAR * ptszFileHash /* = NULL */)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszTimeStart) || (NULL == ptszTimeEnd))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    if (NULL != ptszTimeStart)
    {
		if (!st_JsonRoot["lpszTimeStart"].isNull())
		{
			_tcscpy(ptszTimeStart, st_JsonRoot["lpszTimeStart"].asCString());
		}
    }
    if (NULL != ptszTimeEnd)
    {
		if (!st_JsonRoot["lpszTimeEnd"].isNull())
		{
			_tcscpy(ptszTimeEnd, st_JsonRoot["lpszTimeEnd"].asCString());
		}
    }
    if (NULL != ptszFileName)
    {
        if (!st_JsonRoot["lpszFileName"].isNull())
        {
            _tcscpy(ptszFileName, st_JsonRoot["lpszFileName"].asCString());
        }
    }
    if (NULL != ptszFileHash)
    {
        if (!st_JsonRoot["lpszFileHash"].isNull())
        {
            _tcscpy(ptszFileHash, st_JsonRoot["lpszFileHash"].asCString());
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：Protocol_StorageParse_ReportFile
函数功能：解析文件报告协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的内容
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的内容大小
 参数.三：pppSt_DBFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析到的文件列表
 参数.四：pInt_ListCount
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_ReportFile(LPCTSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int *pInt_ListCount)
{
    Protocol_IsErrorOccur = FALSE;

	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

    int nCount = st_JsonRoot["Count"].asInt();
    Json::Value st_JsonArray = st_JsonRoot["List"];

    BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_DBFile, nCount, sizeof(XSTORAGECORE_DBFILE));
    for (int i = 0; i < nCount; i++)
    {
        if (st_JsonArray[i]["nFileSize"].isNull())
        {
            (*pppSt_DBFile)[i]->st_ProtocolFile.nFileSize = 0;
        }
        else
        {
            (*pppSt_DBFile)[i]->st_ProtocolFile.nFileSize = st_JsonArray[i]["nFileSize"].asInt64();
        }

        if (!st_JsonArray[i]["tszFileName"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileName, st_JsonArray[i]["tszFileName"].asCString());
        }
        if (!st_JsonArray[i]["tszFilePath"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath, st_JsonArray[i]["tszFilePath"].asCString());
        }
        if (!st_JsonArray[i]["tszFileHash"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileHash, st_JsonArray[i]["tszFileHash"].asCString());
        }
        if (!st_JsonArray[i]["tszFileTime"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileTime, st_JsonArray[i]["tszFileTime"].asCString());
        }
        if (!st_JsonArray[i]["tszFileUser"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileUser, st_JsonArray[i]["tszFileUser"].asCString());
        }
    }
    *pInt_ListCount = nCount;
    return TRUE;
}
/********************************************************************
函数名称：Protocol_StorageParse_DirOperator
函数功能：文件夹操作协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要解析的缓冲区
 参数.二：ptszUserDir
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的文件夹
 参数.三：pInt_Operator
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出操作类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_DirOperator(LPCTSTR lpszMsgBuffer, TCHAR* ptszUserDir, int* pInt_Operator)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszUserDir))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    *pInt_Operator = st_JsonRoot["nOPerator"].asInt();
    _tcscpy(ptszUserDir, st_JsonRoot["lpszUserDir"].asCString());
    return TRUE;
}
/********************************************************************
函数名称：Protocol_StorageParse_UPEvent
函数功能：NGINX代理上传事件处理函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的缓冲区
 参数.二：lpszBoundary
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入边界字符串
 参数.三：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件名
 参数.四：ptszFilePath
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件路径
 参数.五：ptszFileHash
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件HASH
 参数.六：pInt_FileSize
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出文件大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_UPEvent(LPCTSTR lpszMsgBuffer, LPCTSTR lpszBoundary, TCHAR* ptszFileName, TCHAR* ptszFilePath, TCHAR* ptszFileHash, __int64x* pInt_FileSize)
{
    Protocol_IsErrorOccur = FALSE;

    std::string::size_type nPos;
    std::string m_FindStr = lpszMsgBuffer;
    //扩展字符串以方便操作
    m_FindStr += lpszBoundary;

    unsigned int nBDLen = _tcslen(lpszBoundary);
    unsigned int nCount = m_FindStr.size();
    for (unsigned int i = 0; i < nCount; i++)
    {
        nPos = m_FindStr.find(lpszBoundary, i);
        if (nPos < nCount)
        {
            std::string m_Str = m_FindStr.substr(i, nPos - i);
            if (!m_Str.empty())
            {
                LPCTSTR lpszLineStr = _T("\r\n\r\n");
                LPCTSTR lpszContentText = _T("Content-Disposition");

                TCHAR tszMsgBuffer[1024];
                TCHAR tszKeyStr[MAX_PATH];

                memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
                memset(tszKeyStr, '\0', sizeof(tszKeyStr));
                _tcscpy(tszMsgBuffer, m_Str.c_str());

                TCHAR* ptszTokPos = NULL;
                TCHAR* ptszTokStr = _tcstok_s(tszMsgBuffer, lpszLineStr, &ptszTokPos);
                if (NULL != _tcsstr(ptszTokStr, lpszContentText))
                {
                    BaseLib_OperatorString_GetStartEnd(ptszTokStr, tszKeyStr, _T("\""), _T("\""));

                    LPCTSTR lpszKeyName = _T("file_name");
                    LPCTSTR lpszKeyPath = _T("file_path");
                    LPCTSTR lpszKeyHash = _T("file_md5");
                    LPCTSTR lpszKeySize = _T("file_size");
                    if (0 == _tcsncmp(lpszKeyName, tszKeyStr, _tcslen(lpszKeyName)))
                    {
                        ptszTokStr = _tcstok_s(NULL, lpszLineStr, &ptszTokPos);
                        if (NULL != ptszTokStr)
                        {
                            _tcscpy(ptszFileName, ptszTokStr);
                        }
                    }
                    if (0 == _tcsncmp(lpszKeyPath, tszKeyStr, _tcslen(lpszKeyPath)))
                    {
                        ptszTokStr = _tcstok_s(NULL, lpszLineStr, &ptszTokPos);
                        if (NULL != ptszTokStr)
                        {
                            _tcscpy(ptszFilePath, ptszTokStr);
                        }
                    }
                    if (0 == _tcsncmp(lpszKeyHash, tszKeyStr, _tcslen(lpszKeyHash)))
                    {
                        ptszTokStr = _tcstok_s(NULL, lpszLineStr, &ptszTokPos);
                        if (NULL != ptszTokStr)
                        {
                            _tcscpy(ptszFileHash, ptszTokStr);
                        }
                    }
                    if (0 == _tcsncmp(lpszKeySize, tszKeyStr, _tcslen(lpszKeySize)))
                    {
                        ptszTokStr = _tcstok_s(NULL, lpszLineStr, &ptszTokPos);
                        if (NULL != ptszTokStr)
                        {
                            *pInt_FileSize = _ttoi64(ptszTokStr);
                        }
                    }
                }
            }
            i = nPos + nBDLen - 1;
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：Protocol_StorageParse_ProxyNotify
函数功能：解析上传下载通知协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的内容
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：ptszClientAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件客户端地址
 参数.四：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件名称
 参数.五：ptszFileHash
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件HASH
 参数.六：pInt_FileSize
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出文件大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_ProxyNotify(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszClientAddr, TCHAR* ptszFileName, TCHAR* ptszFileHash, __int64x* pInt_FileSize)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszClientAddr))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = TRUE;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    _tcscpy(ptszClientAddr, st_JsonRoot["lpszClientAddr"].asCString());
    _tcscpy(ptszFileName, st_JsonRoot["lpszFileName"].asCString());
    _tcscpy(ptszFileHash, st_JsonRoot["lpszFileHash"].asCString());
    *pInt_FileSize = st_JsonRoot["nFileSize"].asInt64();

    return TRUE;
}