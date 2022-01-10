#pragma once
/********************************************************************
//    Created:     2021/07/08  15:37:18
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Define.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp
//    File Base:   APIHelp_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义
//    History:
*********************************************************************/
typedef struct  
{
	TCHAR tszMsgBuffer[4096];
	int nMsgLen;
}APIHELP_LBFILEINFO;
//////////////////////////////////////////////////////////////////////////
//                       导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD StorageHelp_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                       分布式函数                                     */
/************************************************************************/
/********************************************************************
函数名称：APIHelp_Distributed_IsMode
函数功能：判断负载模式是否为指定模式
 参数.一：pStl_ListMode
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入支持的模式列表
 参数.二：nMode
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要判断的模式
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL APIHelp_Distributed_IsMode(list<int>*pStl_ListMode, int nMode);
/********************************************************************
函数名称：APIHelp_Distributed_RandomAddr
函数功能：随机选择一个负载的重定向服务器地址
 参数.一：pStl_ListAddr
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入负载服务器列表
 参数.二：ptszAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的负载地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL APIHelp_Distributed_RandomAddr(list<string>* pStl_ListAddr, TCHAR* ptszAddr);
/********************************************************************
函数名称：APIHelp_Distributed_FileList
函数功能：解析所有解析到的内容并且打包成指定结构
 参数.一：pStl_ListParse
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入要解析的内容列表
 参数.二：pppSt_ListPacket
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出文件信息结构列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出文件列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL APIHelp_Distributed_FileList(list<APIHELP_LBFILEINFO>* pStl_ListParse, XSTORAGECORE_DBFILE*** pppSt_ListPacket, int* pInt_ListCount);
/********************************************************************
函数名称：APIHelp_Distributed_DLStorage
函数功能：通过URLKEY得到一个对应下载地址
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的URL
 参数.二：pStl_ListBucket
  In/Out：In
  类型：容器指针
  可空：N
  意思：输入要解析的列表
 参数.三：pSt_StorageBucket
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的可用存储
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL APIHelp_Distributed_DLStorage(LPCTSTR lpszMsgBuffer, list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket);
/********************************************************************
函数名称：APIHelp_Distributed_UPStorage
函数功能：通过分布式存储列表获得一个存储地址
 参数.一：pStl_ListBucket
  In/Out：In
  类型：容器指针
  可空：N
  意思：输入要解析的列表
 参数.二：pSt_StorageBucket
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的可用存储
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL APIHelp_Distributed_UPStorage(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket);
/************************************************************************/
/*                       帮助函数                                       */
/************************************************************************/
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
extern "C" BOOL APIHelp_Api_ProxyAuth(TCHAR* ptszUser, TCHAR* ptszPass, TCHAR** pptszListHdr, int nHdrCount);
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
extern "C" BOOL APIHelp_Api_RangeFile(int* pInt_SPos, int* pInt_EPos, __int64x* pInt_Count, TCHAR** pptszListHdr, int nHdrCount);
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
extern "C" BOOL APIHelp_Api_VerHash(LPCTSTR lpszFileHash, TCHAR** pptszListHdr, int nHdrCount);