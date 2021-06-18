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
 参数.六：lpszFileMD5
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的文件MD5
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQQueryFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileMD5 /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQQUERY_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQQUERYFILE;
    st_ProtocolHdr.byVersion = 2;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

    st_JsonRoot["lpszTimeStart"] = lpszTimeStart;
    st_JsonRoot["lpszTimeEnd"] = lpszTimeEnd;
    if (NULL != lpszFileName)
    {
        st_JsonRoot["lpszFileName"] = lpszFileName;
    }
    if (NULL != lpszFileMD5)
    {
        st_JsonRoot["lpszFileMD5"] = lpszFileMD5;
    }
    st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Client_REQQueryUser
函数功能：客户端请求查询用户信息打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的数据缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查询的用户名
 参数.四：bQueryKey
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否查询的信息包含关键信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQQueryUser(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUserName, BOOL bQueryKey /* = FALSE */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQQUERY_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQQUERYUSER;
    st_ProtocolHdr.byVersion = 2;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

    st_JsonRoot["lpszUserName"] = lpszUserName;
    st_JsonRoot["bQueryKey"] = bQueryKey;
    st_ProtocolHdr.unPacketSize = (UINT)st_JsonRoot.toStyledString().length();
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);
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
 参数.四：lpszFileMD5
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要删除的文件MD5
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQDelete(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszFileName, LPCTSTR lpszFileMD5)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQDELETE_PARAMENT;
        return FALSE;
    }
    TCHAR tszFilePath[MAX_PATH];
    TCHAR tszFileName[MAX_PATH];
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    XENGINE_PROTOCOLFILE st_ProtocolFile;

    memset(tszFilePath, '\0', MAX_PATH);
    memset(tszFileName, '\0', MAX_PATH);
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    memset(&st_ProtocolFile, '\0', sizeof(XENGINE_PROTOCOLFILE));
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQDELETE;
    st_ProtocolHdr.byVersion = 1;
    st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLFILE);
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

    if (NULL != lpszFileName)
    {
        _tcscpy(st_ProtocolFile.tszFileName, lpszFileName);
    }
    if (NULL != lpszFileMD5)
    {
        _tcscpy(st_ProtocolFile.tszFileHash, lpszFileMD5);
    }
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLFILE);
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtocolFile, sizeof(XENGINE_PROTOCOLFILE));
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Client_REQLogin
函数功能：登录请求协议封装函数
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
 参数.三：pSt_ProtocolAuth
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的登录协议
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQLogin(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XENGINE_PROTOCOL_USERAUTH *pSt_ProtocolAuth)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen) || (NULL == pSt_ProtocolAuth))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQLOGIN_PARAMENT;
        return FALSE;
    }
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN;
    st_ProtocolHdr.byVersion = 1;
    st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_USERAUTH);
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOL_USERAUTH);
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_ProtocolAuth, sizeof(XENGINE_PROTOCOL_USERAUTH));
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Client_REQUPFile
函数功能：请求上传文件协议打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出请求协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：xhToken
  In/Out：In
  类型：网络句柄
  可空：N
  意思：输入上传数据的令牌
 参数.四：lpszFileMD5
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求的文件MD5,通过查询得到,下载请求不能为空
 参数.五：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入本地文件路径,上传请求参数5和6不能为空
 参数.六：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入服务器文件保存的位置
 参数.七：bUPFile
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：上传还是下载,默认上传
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQFile(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, LPCTSTR lpszFileMD5, LPCTSTR lpszFileName, LPCTSTR lpszFilePath, BOOL bUPFile)
{
    XStorage_IsErrorOccur = FALSE;

    if ((0 == xhToken) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQUP_PARAMENT;
        return FALSE;
    }
    TCHAR tszFileName[MAX_PATH];
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    XENGINE_PROTOCOLFILE st_ProtocolFile;

    memset(tszFileName, '\0', MAX_PATH);
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    memset(&st_ProtocolFile, '\0', sizeof(XENGINE_PROTOCOLFILE));
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    st_ProtocolHdr.xhToken = xhToken;
    st_ProtocolHdr.byVersion = 1;
    st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLFILE);
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
    if (bUPFile)
    {
        st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQUPDATE;
        //提取路径和文件名
        if (!BaseLib_OperatorString_GetFileAndPath(lpszFileName, NULL, tszFileName))
        {
            XStorage_IsErrorOccur = TRUE;
            XStorage_dwErrorCode = BaseLib_GetLastError();
            return FALSE;
        }
        //填充文件大小协议
        struct _tstat st_FileStat;
        memset(&st_FileStat, '\0', sizeof(struct _tstat));

        if (-1 == _tstat(lpszFileName, &st_FileStat))
        {
            XStorage_IsErrorOccur = TRUE;
            XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQUP_GETSIZE;
            return FALSE;
        }
        st_ProtocolFile.nFileSize = st_FileStat.st_size;
    }
    else
    {
        _tcscpy(st_ProtocolFile.tszFileHash, lpszFileMD5);
        st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQDOWN;
    }
    //填充文件信息
    _tcscpy(st_ProtocolFile.tszFileName, tszFileName);
    if (NULL != lpszFilePath)
    {
        _tcscpy(st_ProtocolFile.tszFilePath, lpszFilePath);
    }
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLFILE);
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtocolFile, sizeof(XENGINE_PROTOCOLFILE));
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Client_REQDirOperator
函数功能：请求创建或者删除一个文件夹
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
 参数.四：bCreate
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：真为创建,假为删除
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQDirOperator(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszUserDir,BOOL bCreate)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszUserDir) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQDIRCREATE_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

    st_JsonRoot["lpszUserDir"] = lpszUserDir;
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    if (bCreate)
    {
        st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQDIRCREATE;
    }
    else
    {
        st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQDIRDELETE;
    }
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
函数名称：XStorageProtocol_Client_REQDirQuery
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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQDirQuery(TCHAR *ptszMsgBuffer, int *pInt_MsgLen)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQDIRQUERY_PARAMENT;
        return FALSE;
    }
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQDIRQUERY;
    st_ProtocolHdr.byVersion = 2;
    st_ProtocolHdr.unPacketSize = 0;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR);
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Client_REQRegister
函数功能：用户注册
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出封装好的缓冲区信息
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出缓冲区大小
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要注册的用户名
 参数.四：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要注册的密码
 参数.五：lpszEMailAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：电子邮件
 参数.六：nPhoneNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：联系电话
 参数.七：nIDNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：身份证
 参数.八：nPerimission
  In/Out：In
  类型：整数型
  可空：N
  意思：权限级别
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Client::XStorageProtocol_Client_REQRegister(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, LPCTSTR lpszUser, LPCTSTR lpszPass, LPCTSTR lpszEMailAddr, __int64x nPhoneNumber, __int64x nIDNumber, int nPerimission)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen) || (NULL == lpszUser) || (NULL == lpszPass))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQREG_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    //填充协议头
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQREGISTER;
    st_ProtocolHdr.byVersion = 2;
    st_ProtocolHdr.unPacketSize = 0;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

    st_JsonRoot["lpszUser"] = lpszUser;
    st_JsonRoot["lpszPass"] = lpszPass;
    st_JsonRoot["lpszEMail"] = lpszEMailAddr;
    st_JsonRoot["nPhoneNumber"] = (Json::Value::UInt64)nPhoneNumber;
    st_JsonRoot["nIDNumber"] = (Json::Value::UInt64)nIDNumber;
    st_JsonRoot["nPerimission"] = nPerimission;

    st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();
    //打包输出信息
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize);
    return TRUE;
}
