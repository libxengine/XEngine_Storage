#include "pch.h"
#include "XStorageProtocol_Comm.h"
/********************************************************************
//	Created:	2019/6/28   15:19
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Comm\XStorageProtocol_Comm.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Comm
//	File Base:	XStorageProtocol_Comm
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	存储协议公用类
//	History:
*********************************************************************/
CXStorageProtocol_Comm::CXStorageProtocol_Comm()
{

}
CXStorageProtocol_Comm::~CXStorageProtocol_Comm()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorageProtocol_Comm_ParseFile
函数功能：通用文件解析函数
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
 参数.三：byVersion
  In/Out：In
  类型：字节
  可空：N
  意思：输入解析协议的版本号,支持1结构体,2JSON
 参数.四：pSt_ProtocolFile
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出解析后的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Comm::XStorageProtocol_Comm_ParseFile(LPCTSTR lpszMsgBuffer, int nMsgLen, BYTE byVersion, XENGINE_PROTOCOLFILE* pSt_ProtocolFile)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == pSt_ProtocolFile))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_COMM_UPPARSE_PARAMENT;
        return FALSE;
    }
    //判断协议版本
    if (1 == byVersion)
    {
        memcpy(pSt_ProtocolFile, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLFILE));
    }
    else
    {
        Json::Value st_JsonRoot;
        Json::CharReaderBuilder st_JsonBuild;
        Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

        JSONCPP_STRING st_JsonError;

        if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
        {
            return FALSE;
        }
        delete pSt_JsonReader;
        pSt_JsonReader = NULL;

        if (!st_JsonRoot["tszFileName"].isNull())
        {
            _tcscpy(pSt_ProtocolFile->tszFileName, st_JsonRoot["tszFileName"].asCString());
        }
        if (!st_JsonRoot["tszFileMD5"].isNull())
        {
            _tcscpy(pSt_ProtocolFile->tszFileHash, st_JsonRoot["tszFileMD5"].asCString());
        }
        if (!st_JsonRoot["tszFileTime"].isNull())
        {
            _tcscpy(pSt_ProtocolFile->tszFileTime, st_JsonRoot["tszFileTime"].asCString());
        }
        if (!st_JsonRoot["nFileSize"].isNull())
        {
            pSt_ProtocolFile->nFileSize = st_JsonRoot["nFileSize"].asUInt64();
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Comm_Response
函数功能：通用回复协议打包函数
 参数.一：pSt_ProtocolHdr
  In/Out：In/Out
  类型：数据结构指针
  可空：N
  意思：输入要返回的协议,协议头的后续大小字段会被修改
 参数.二：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出组好包的缓冲区
 参数.三：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出缓冲区大小
 参数.四：nMsgCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入操作的结果,0为真
 参数.五：lpszMsgInfo
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入CODE字段解释,可以不填
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Comm::XStorageProtocol_Comm_Response(XENGINE_PROTOCOLHDR *pSt_ProtocolHdr, TCHAR *ptszMsgBuffer, int *pInt_MsgLen, int nMsgCode /* = 0 */, LPCTSTR lpszMsgInfo /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == pSt_ProtocolHdr) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_COMM_RESPONSE_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    //改写协议类型
    if (1 == pSt_ProtocolHdr->byVersion)
    {
        pSt_ProtocolHdr->byVersion = 2;
    }
    //组包JSON
    st_JsonRoot["Code"] = nMsgCode;
    if (NULL == lpszMsgInfo)
    {
        st_JsonRoot["Msg"];
    }
    else
    {
        st_JsonRoot["Msg"] = lpszMsgInfo;
    }
    pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();
    //打包协议
    *pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
    memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), pSt_ProtocolHdr->unPacketSize);
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Comm_ParseClient
函数功能：通用客户端解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：ptszMsgBuffer
  In/Out：Out
  类型：字节指针
  可空：N
  意思：输出获取到的后续数据
 参数.三：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出后续数据大小
 参数.三：pSt_ProtocolHdr
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：导出协议头
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Comm::XStorageProtocol_Comm_ParseClient(LPCTSTR lpszMsgBuffer, TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XENGINE_PROTOCOLHDR *pSt_ProtocolHdr)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_COMM_PARSECLIENT_PARAMENT;
        return FALSE;
    }
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
    memcpy(&st_ProtocolHdr, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR));
    //检查是否是专属协议
    if ((XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER != st_ProtocolHdr.wHeader) || (XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL != st_ProtocolHdr.wTail))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_COMM_PARSECLIENT_PROTO;
        return FALSE;
    }
    *pInt_MsgLen = st_ProtocolHdr.unPacketSize;
    memcpy(ptszMsgBuffer, lpszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_ProtocolHdr.unPacketSize);
    if (NULL != pSt_ProtocolHdr)
    {
        memcpy(pSt_ProtocolHdr, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    }

    if (st_ProtocolHdr.unPacketSize > 0)
    {
        Json::Value st_JsonRoot;
        Json::CharReaderBuilder st_JsonBuild;
        Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

        JSONCPP_STRING st_JsonError;
        //解析JSON
        if (pSt_JsonReader->parse(lpszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), lpszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize, &st_JsonRoot, &st_JsonError))
        {
            if (0 != st_JsonRoot["Code"].asInt())
            {
                XStorage_IsErrorOccur = TRUE;
                XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_COMM_PARSECLIENT_NOTCOMPLETE;
                return FALSE;
            }
        }
        delete pSt_JsonReader;
        pSt_JsonReader = NULL;
    }
    return TRUE;
}
