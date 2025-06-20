﻿#pragma once
/********************************************************************
//	Created:	2013/1/18  12:24
//	File Name: 	G:\U_DISK_Path\NetSocketEngine\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_Define.h
//	File Path:	G:\U_DISK_Path\NetSocketEngine\NetEngine_P2xp\NetEngine_P2XPProtocol
//	File Base:	P2XPProtocol_Define
//	File Ext:	h
//  Project:    NetSocketEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	P2XP协议构造分析器导出模块
//	History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Protocol_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                        解析协议导出                                  */
/************************************************************************/
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
 参数.四：ptszBuckKey
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出路径的KEY
 参数.五：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件名称
 参数.六：ptszFileHash
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件HASH
 参数.七：pInt_FileSize
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出文件大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StorageParse_ProxyNotify(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszClientAddr, XCHAR * ptszBuckKey, XCHAR * ptszFileName, XCHAR * ptszFileHash, __int64x * pInt_FileSize);
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
 参数.四：ptszBuckKey
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出文件所属BUCKET
 参数.五：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：查询的文件名
 参数.六：ptszFileHash
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：查询的文件HASH
 参数.七：pInt_Mode
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出查询返回模式
 参数.八：pxhToken
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出TOKEN
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StorageParse_QueryFile(LPCXSTR lpszMsgBuffer, XCHAR * ptszTimeStart, XCHAR * ptszTimeEnd, XCHAR * ptszBuckKey = NULL, XCHAR * ptszFileName = NULL, XCHAR * ptszFileHash = NULL, int* pInt_Mode = NULL, XNETHANDLE * pxhToken = NULL);
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
extern "C" bool Protocol_StorageParse_ReportFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE * **pppSt_DBFile, int* pInt_ListCount);
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
 参数.三：ptszBuckKey
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出桶的KEY
 参数.四：pInt_Operator
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出操作类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StorageParse_DirOperator(LPCXSTR lpszMsgBuffer, XCHAR * ptszUserDir, XCHAR * ptszBuckKey, int* pInt_Operator);
/********************************************************************
函数名称：Protocol_StorageParse_SpeedLimit
函数功能：速度限制请求解析函数
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
 参数.三：pInt_Code
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出回复值
 参数.四：pInt_Limit
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出回复的限制
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StorageParse_SpeedLimit(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_Code, int* pInt_Limit);
/********************************************************************
函数名称：Protocol_StorageParse_P2PToken
函数功能：P2P查找文件TOKEN匹配函数
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
 参数.三：pxhToken
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出解析到的TOKEN
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StorageParse_P2PToken(LPCXSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE* pxhToken);
/********************************************************************
函数名称：Protocol_StorageParse_Action
函数功能：动作解析器
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
 参数.三：pSt_ActionInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StorageParse_Action(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_ACTIONINFO * pSt_ActionInfo);
/********************************************************************
函数名称：Protocol_StorageParse_WDLock
函数功能：WEBDAV加锁协议解析函数
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
 参数.三：pSt_WDLock
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StorageParse_WDLock(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_WEBDAVLOCK* pSt_WDLock);
/********************************************************************
函数名称：Protocol_StorageParse_WDPropPatch
函数功能：解析proppatch协议
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
 参数.三：pStl_ListName
  In/Out：Out
  类型：LIST容器指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StorageParse_WDPropPatch(LPCXSTR lpszMsgBuffer, int nMsgLen, std::list<string>* pStl_ListName);
/************************************************************************/
/*                        打包协议导出                                  */
/************************************************************************/
/********************************************************************
函数名称：Protocol_StoragePacket_HTTPPacket
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
 参数.三：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入HTTP错误码
 参数.四：lpszMSGInfo
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入返回的消息信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_HTTPPacket(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCXSTR lpszMSGInfo = NULL);
/********************************************************************
函数名称：Protocol_StoragePacket_BasicAuth
函数功能：基本验证协议代理转发
 参数.一：lpszRequestType
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的业务类型
 参数.二：lpszMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：方法名称
 参数.三：lpszPostUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户提交的地址
 参数.四：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户发送的客户端地址
 参数.五：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户
 参数.六：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：密码
 参数.七：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：转发数据包
 参数.八：pInt_MsgLen
  In/Out：Out
  类型：整数型
  可空：N
  意思：导出数据包大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_BasicAuth(int nRequestType, LPCXSTR lpszMethod, LPCXSTR lpszPostUrl, LPCXSTR lpszClientAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR * ptszMsgBuffer, int* pInt_MsgLen);
/********************************************************************
函数名称：Protocol_StoragePacket_UPDown
函数功能：上传下载完成代理通知协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：数据包
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型
  可空：N
  意思：导出数据包大小
 参数.三：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件所属BUCKET
 参数.四：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：文件的地址
 参数.五：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户客户端地址
 参数.六：nFileSize
  In/Out：In
  类型：整数型
  可空：N
  意思：文件大小
 参数.七：bDown
  In/Out：In
  类型：逻辑型
  可空：N
  意思：是上传还是下载
 参数.八：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：文件的HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_UPDown(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszBuckKey, LPCXSTR lpszFileName, LPCXSTR lpszClientAddr, __int64x nFileSize, bool bDown, LPCXSTR lpszFileHash = NULL);
/********************************************************************
函数名称：Protocol_StorageParse_QueryFile
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
 参数.七：xhToken
  In/Out：In
  类型：句柄
  可空：Y
  意思：输入返回的TOKEN,如果需要的话
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_QueryFile(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XSTORAGECORE_DBFILE * **pppSt_DBFile, int nListCount, LPCXSTR lpszTimeStart = NULL, LPCXSTR lpszTimeEnd = NULL, XNETHANDLE xhToken = 0);
/********************************************************************
函数名称：Protocol_StoragePacket_Info
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
extern "C" bool Protocol_StoragePacket_Info(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, SESSION_STORAGEINFO * **pppSt_DLInfo, SESSION_STORAGEINFO * **pppSt_UPInfo, int nDLCount, int nUPCount);
/********************************************************************
函数名称：Protocol_StoragePacket_DirOperator
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
extern "C" bool Protocol_StoragePacket_DirOperator(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XCHAR * **pppszListEnum, int nListCount);
/********************************************************************
函数名称：Protocol_StoragePacket_REQFile
函数功能：查询文件请求函数
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
  意思：输入要查询的文件名
 参数.四：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的文件MD5
 参数.五：xhToken
  In/Out：In
  类型：句柄
  可空：Y
  意思：消息的TOKEN
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_REQFile(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName = NULL, LPCXSTR lpszFileHash = NULL, XNETHANDLE xhToken = 0);
/********************************************************************
函数名称：Protocol_StoragePacket_Action
函数功能：动作脚本封装器
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
  类型：句柄
  可空：N
  意思：输入管理器句柄
 参数.四：pSt_ActionInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_Action(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, XENGINE_ACTIONINFO* pSt_ActionInfo);
/********************************************************************
函数名称：Protocol_StoragePacket_Bucket
函数功能：获取bucket信息
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
 参数.三：pStl_ListBucket
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入要打包的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_Bucket(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, list<XENGINE_STORAGEBUCKET>* pStl_ListBucket);
/********************************************************************
函数名称：Protocol_StoragePacket_Notfound
函数功能：没有找到
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的XML数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出大小
 参数.三：lpszFileName
  In/Out：Out
  类型：常量字符指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_Notfound(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName);
/********************************************************************
函数名称：Protocol_StoragePacket_WDPropfind
函数功能：propfind协议打包处理函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的XML数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出大小
 参数.三：ppptszListFile
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入打包的文件列表
 参数.四：nFileCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
 参数.五：lpszBucketPath
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入BUCKET名称
 参数.六：lpszBucketKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入BUCKET的KEY
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_WDPropfind(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XCHAR*** ppptszListFile, int nFileCount, LPCXSTR lpszBucketPath, LPCXSTR lpszBucketKey);
/********************************************************************
函数名称：Protocol_StoragePacket_WDLock
函数功能：webdav锁协议封装
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
 参数.三：pSt_WDLock
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_WDLock(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_WEBDAVLOCK* pSt_WDLock);
/********************************************************************
函数名称：Protocol_StoragePacket_WDPropPatch
函数功能：webdavPROPPATCH协议封装
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
  可空：N
  意思：输入文件名
 参数.四：pStl_ListName
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入打包的属性列表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Protocol_StoragePacket_WDPropPatch(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName, std::list<string>* pStl_ListName);