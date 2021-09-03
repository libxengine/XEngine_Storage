#include "pch.h"
#include "XStorageProtocol_Core.h"
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
CXStorageProtocol_Core::CXStorageProtocol_Core()
{

}
CXStorageProtocol_Core::~CXStorageProtocol_Core()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorageProtocol_Core_REQQueryFile
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
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REQQueryFile(LPCTSTR lpszMsgBuffer, TCHAR *ptszTimeStart, TCHAR *ptszTimeEnd, TCHAR *ptszFileName /* = NULL */, TCHAR * ptszFileHash /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszTimeStart) || (NULL == ptszTimeEnd))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARSE;
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
函数名称：XStorageProtocol_Core_REPQueryFile
函数功能：查询回复打包协议
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
 参数.三：pppSt_DBFile
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入文件列表,参数内存由用户管理
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入文件列表个数
 参数.五：lpszRootDir
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：某些时候可能需要跳过字符串
 参数.六：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入查询请求的开始时间
 参数.七：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入查询请求的结束时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REPQueryFile(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int nListCount, LPCTSTR lpszRootDir, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen) || (NULL == pppSt_DBFile))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;
    //JSON赋值
    for (int i = 0; i < nListCount; i++)
    {
        Json::Value st_JsonObject;

        if (NULL == lpszRootDir)
        {
            st_JsonObject["tszFilePath"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath;
        }
        else
        {
			if (NULL == _tcsstr((*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath, lpszRootDir))
			{
				st_JsonObject["tszFilePath"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath;
			}
			else
			{
				st_JsonObject["tszFilePath"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath + _tcslen(lpszRootDir);
			}
        }
        st_JsonObject["tszFileName"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileName;
        st_JsonObject["tszFileUser"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileUser;
        st_JsonObject["tszFileHash"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileHash;
        st_JsonObject["tszFileTime"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileTime;
        st_JsonObject["nFileSize"] = (*pppSt_DBFile)[i]->st_ProtocolFile.nFileSize;
        //只有在P2P下取文件列表才有效
        if (_tcslen((*pppSt_DBFile)[i]->tszTableName) > 0)
        {
            st_JsonObject["tszTableName"] = (*pppSt_DBFile)[i]->tszTableName;
        }
        st_JsonArray.append(st_JsonObject);
    }
    st_JsonRoot["Count"] = nListCount;
    st_JsonRoot["List"] = st_JsonArray;
    if (NULL != lpszTimeStart)
    {
        st_JsonRoot["lpszTimeStart"] = lpszTimeStart;
    }
    if (NULL != lpszTimeEnd)
    {
        st_JsonRoot["lpszTimeEnd"] = lpszTimeEnd;
    }
    st_JsonRoot["Code"] = 0;
    st_JsonRoot["Msg"] = _T("ok");
    //打包输出信息
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Core_ReportFileParse
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
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_ReportFileParse(LPCTSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int *pInt_ListCount)
{
    XStorage_IsErrorOccur = FALSE;

	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARSE;
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
函数名称：XStorageProtocol_Core_REPInfo
函数功能：返回信息获取请求打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的缓冲区内容
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pppSt_DLInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：下载信息列表
 参数.四：pppSt_UPInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：上传信息列表
 参数.五：nDLCount
  In/Out：In
  类型：整数型
  可空：N
  意思：下载列表个数
 参数.六：nUPCount
  In/Out：In
  类型：整数型
  可空：N
  意思：上传列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REPInfo(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, SESSION_STORAGEINFO*** pppSt_DLInfo, SESSION_STORAGEINFO*** pppSt_UPInfo, int nDLCount, int nUPCount)
{
	XStorage_IsErrorOccur = FALSE;

	if ((NULL == pppSt_DLInfo) || (NULL == pppSt_UPInfo))
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonDLArray;
    Json::Value st_JsonUPArray;

	for (int i = 0; i < nDLCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["tszClientAddr"] = (*pppSt_DLInfo)[i]->tszClientAddr;
        st_JsonObject["tszFilePath"] = (*pppSt_DLInfo)[i]->tszFileDir;
        st_JsonObject["ullCount"] = (*pppSt_DLInfo)[i]->ullCount;
        st_JsonObject["ullRWCount"] = (*pppSt_DLInfo)[i]->ullRWCount;
        st_JsonObject["ullRWLen"] = (*pppSt_DLInfo)[i]->ullRWLen;
        st_JsonObject["ullPosStart"] = (*pppSt_DLInfo)[i]->ullPosStart;
        st_JsonObject["ullPosEnd"] = (*pppSt_DLInfo)[i]->ullPosEnd;
        st_JsonObject["nPoolIndex"] = (*pppSt_DLInfo)[i]->nPoolIndex;

        st_JsonDLArray.append(st_JsonObject);
	}
	for (int i = 0; i < nUPCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["tszClientAddr"] = (*pppSt_DLInfo)[i]->tszClientAddr;
		st_JsonObject["tszFilePath"] = (*pppSt_DLInfo)[i]->tszFileDir;
		st_JsonObject["ullCount"] = (*pppSt_DLInfo)[i]->ullCount;
		st_JsonObject["ullRWCount"] = (*pppSt_DLInfo)[i]->ullRWCount;
		st_JsonObject["ullRWLen"] = (*pppSt_DLInfo)[i]->ullRWLen;
		st_JsonObject["ullPosStart"] = (*pppSt_DLInfo)[i]->ullPosStart;
		st_JsonObject["ullPosEnd"] = (*pppSt_DLInfo)[i]->ullPosEnd;

        st_JsonUPArray.append(st_JsonObject);
	}
	st_JsonRoot["DLCount"] = st_JsonDLArray.size();
    st_JsonRoot["UPCount"] = st_JsonUPArray.size();

	st_JsonRoot["DLList"] = st_JsonDLArray;
    st_JsonRoot["UPList"] = st_JsonUPArray;

	st_JsonRoot["Code"] = 0;
	st_JsonRoot["Msg"] = _T("ok");
	//打包输出信息
	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Core_REQDirOperator
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
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REQDirOperator(LPCTSTR lpszMsgBuffer, TCHAR* ptszUserDir, int* pInt_Operator)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszUserDir))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    *pInt_Operator = st_JsonRoot["nOPerator"].asInt();
    _tcscpy(ptszUserDir, st_JsonRoot["lpszUserDir"].asCString());
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Core_REPQueryDir
函数功能：查询用户目录
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
 参数.三：pppszListEnum
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入查找到的文件夹,这个内存由用户管理
 参数.四：nListCount
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输入文件夹个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REPDirOperator(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, TCHAR*** pppszListEnum, int nListCount)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;

    for (int i = 0; i < nListCount; i++)
    {
        Json::Value st_JsonObject;
        st_JsonObject["tszFilePath"] = (*pppszListEnum)[i];
        st_JsonArray.append(st_JsonObject);
    }
    st_JsonRoot["Count"] = st_JsonArray.size();
    st_JsonRoot["List"] = st_JsonArray;
    st_JsonRoot["Code"] = 0;
    st_JsonRoot["Msg"] = _T("ok");
    //打包输出信息
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Core_REQUPEvent
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
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REQUPEvent(LPCTSTR lpszMsgBuffer, LPCTSTR lpszBoundary, TCHAR* ptszFileName, TCHAR* ptszFilePath, TCHAR* ptszFileHash, __int64x* pInt_FileSize)
{
    XStorage_IsErrorOccur = FALSE;

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