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
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REQQUERYFILE_PARAMENT;
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
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REQQUERYFILE_PARSEJSON;
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
		if (!st_JsonRoot["lpszTimeEnd"].asCString())
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
 参数.五：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入查询请求的开始时间
 参数.六：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入查询请求的结束时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REPQueryFile(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int nListCount, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen) || (NULL == pppSt_DBFile))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REPQUERYFILE_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;
    //JSON赋值
    for (int i = 0; i < nListCount; i++)
    {
        Json::Value st_JsonObject;
        st_JsonObject["tszFilePath"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath;
        st_JsonObject["tszFileName"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileName;
        st_JsonObject["tszFileUser"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileUser;
        st_JsonObject["tszFileHash"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileHash;
        st_JsonObject["tszFileTime"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileTime;
        st_JsonObject["nFileSize"] = (*pppSt_DBFile)[i]->st_ProtocolFile.nFileSize;
        st_JsonArray.append(st_JsonObject);
    }
    st_JsonRoot["Count"] = nListCount;
    st_JsonRoot["List"] = st_JsonArray;
    st_JsonRoot["lpszTimeStart"] = lpszTimeStart;
    st_JsonRoot["lpszTimeEnd"] = lpszTimeEnd;
    st_JsonRoot["Code"] = 0;
    st_JsonRoot["Msg"] = _T("ok");
    //打包输出信息
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Core_REPUPFile
函数功能：查询返回文件上传请求确认协议打包
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
 参数.三：xhToken
  In/Out：In
  类型：网络句柄
  可空：Y
  意思：输入客户端的令牌,如果有的话
 参数.四：bUPFile
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是上传还是下载,默认上传
 参数.五：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：返回的处理完毕CODE值
 参数.六：lpszCodeMsg
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入CODE的文字说明,如果需要的话
 参数.七：pSt_ProtcolFile
  In/Out：In
  类型：数据结构指针
  可空：Y
  意思：输入文件附加信息,如果需要的话!
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REPFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XNETHANDLE xhToken /* = 0 */, BOOL bUPFile /* = TRUE */, int nCode /* = 0 */, LPCTSTR lpszCodeMsg /* = NULL */, XENGINE_PROTOCOLFILE *pSt_ProtcolFile /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REPQUERYFILE_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    st_ProtocolHdr.byVersion = 1;
    st_ProtocolHdr.xhToken = xhToken;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
    if (bUPFile)
    {
        st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPUPDATE;
    }
    else
    {
        st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDOWN;
    }
    //填写返回信息
    st_JsonRoot["Code"] = nCode;
    if (NULL != lpszCodeMsg)
    {
        st_JsonRoot["Msg"] = lpszCodeMsg;
    }
    //判断是否有文件附加信息
    if (NULL != pSt_ProtcolFile)
    {
        st_JsonRoot["tszFilePath"] = pSt_ProtcolFile->tszFilePath;
        st_JsonRoot["tszFileName"] = pSt_ProtcolFile->tszFileName;
        st_JsonRoot["tszFileUser"] = pSt_ProtcolFile->tszFileUser;
        st_JsonRoot["tszFileHash"] = pSt_ProtcolFile->tszFileHash;
        st_JsonRoot["tszFileTime"] = pSt_ProtcolFile->tszFileTime;
        st_JsonRoot["nFileSize"] = (Json::Value::UInt64)pSt_ProtcolFile->nFileSize;
    }
    st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);
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
/********************************************************************
函数名称：XStorageProtocol_Core_REQCreateDir
函数功能：创建文件夹协议
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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REQCreateDir(LPCTSTR lpszMsgBuffer, TCHAR *ptszUserDir)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszUserDir))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_CREATEDIR_PARAMENT;
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
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_CREATEDIR_PARSEJSON;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

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
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REPQueryDir(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, TCHAR*** pppszListEnum, int nListCount)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REPDIRQUERY_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

    for (int i = 0; i < nListCount; i++)
    {
        Json::Value st_JsonObject;
        st_JsonObject["tszFilePath"] = (*pppszListEnum)[i];
        st_JsonArray.append(st_JsonObject);
    }
    st_JsonRoot["Count"] = st_JsonArray.size();
    st_JsonRoot["List"] = st_JsonArray;
    st_JsonRoot["Code"] = 0;
    st_JsonRoot["Msg"] = _T("查询用户目录成功");
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDIRQUERY;
    st_ProtocolHdr.byVersion = 2;
    st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Core_REQUserReg
函数功能：用户注册协议解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要解析的缓冲区
 参数.二：pSt_ProtocolRegister
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REQUserReg(LPCTSTR lpszMsgBuffer, XSTORAGECORE_USERINFO* pSt_ProtocolRegister)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolRegister))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_USERREG_PARAMENT;
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
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_USERREG_PARSEJSON;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    _tcscpy(pSt_ProtocolRegister->st_ProtocolUser.tszUserName, st_JsonRoot["lpszUser"].asCString());
    _tcscpy(pSt_ProtocolRegister->st_ProtocolUser.tszUserPass, st_JsonRoot["lpszPass"].asCString());
    _tcscpy(pSt_ProtocolRegister->st_ProtocolUser.tszEMailAddr, st_JsonRoot["lpszEMail"].asCString());
    pSt_ProtocolRegister->st_ProtocolUser.nIDNumber = st_JsonRoot["nIDNumber"].asUInt64();
    pSt_ProtocolRegister->st_ProtocolUser.nPhoneNumber = st_JsonRoot["nPhoneNumber"].asUInt64();
    pSt_ProtocolRegister->st_ProtocolUser.nUserLevel = st_JsonRoot["nUserLeave"].asUInt();
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Core_REQQueryUser
函数功能：用户查询信息请求解析
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的数据
 参数.二：ptszUserName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出用户名
 参数.三：pbKeyQuery
  In/Out：Out
  类型：逻辑型指针
  可空：N
  意思：输出是否允许关键信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REQQueryUser(LPCTSTR lpszMsgBuffer, TCHAR* ptszUserName, BOOL *pbKeyQuery)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszUserName))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_QUERYUSER_PARAMENT;
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
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_QUERYUSER_PARSEJSON;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    _tcscpy(ptszUserName, st_JsonRoot["lpszUserName"].asCString());
    *pbKeyQuery = st_JsonRoot["bQueryKey"].asInt();
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Core_REPQueryUser
函数功能：用户信息查询请求回复打包
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包的数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区打包大小
 参数.三：lParam
  In/Out：In
  类型：无类型指针
  可空：N
  意思：输入用户信息结构体
 参数.四：bKeyInfo
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否返回一些用户的关键信息,默认不返回
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Core::XStorageProtocol_Core_REPQueryUser(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPVOID lParam, BOOL bKeyInfo)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen) || (NULL == lParam))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_QUERYUSER_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    XSTORAGECORE_USERINFO st_UserInfo;

    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    memset(&st_UserInfo, '\0', sizeof(XSTORAGECORE_USERINFO));
    memcpy(&st_UserInfo, lParam, sizeof(XSTORAGECORE_USERINFO));

    st_JsonRoot["tszUserName"] = st_UserInfo.st_ProtocolUser.tszUserName;
    st_JsonRoot["nUserLeave"] = st_UserInfo.st_ProtocolUser.nUserLevel;
    st_JsonRoot["nFileCount"] = (Json::Value::UInt64)st_UserInfo.nFileCount;
    st_JsonRoot["nFileSize"] = (Json::Value::UInt64)st_UserInfo.nFileSize;
    st_JsonRoot["nUserState"] = st_UserInfo.st_ProtocolUser.nUserState;
    st_JsonRoot["tszCreateTime"] = st_UserInfo.st_ProtocolUser.tszCreateTime;
    st_JsonRoot["tszEMailAddr"] = st_UserInfo.st_ProtocolUser.tszEMailAddr;
    st_JsonRoot["tszIPAddr"] = st_UserInfo.tszIPAddr;
    if (bKeyInfo)
    {
        st_JsonRoot["tszUserPass"] = st_UserInfo.st_ProtocolUser.tszUserPass;
        st_JsonRoot["nPhoneNumber"] = (Json::Value::UInt64)st_UserInfo.st_ProtocolUser.nPhoneNumber;
        st_JsonRoot["nIDNumber"] = (Json::Value::UInt64)st_UserInfo.st_ProtocolUser.nIDNumber;
        st_JsonRoot["xhToken"] = (Json::Value::UInt64)st_UserInfo.xhToken;
    }
    st_JsonRoot["Code"] = 0;
    st_JsonRoot["Msg"] = _T("查询用户信息成功");
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPQUERYUSER;
    st_ProtocolHdr.byVersion = 2;
    st_ProtocolHdr.unPacketSize = (UINT)st_JsonRoot.toStyledString().length();
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);
    return TRUE;
}
