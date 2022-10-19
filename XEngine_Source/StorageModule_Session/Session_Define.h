#pragma once
/********************************************************************
//    Created:     2021/05/08  14:07:44
//    File Name:   D:\XEngine_APPService\XEngine_Source\XEngine_NetHelp\NetHelp_Session\Session_Define.h
//    File Path:   D:\XEngine_APPService\XEngine_Source\XEngine_NetHelp\NetHelp_Session
//    File Base:   Session_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     会话导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	TCHAR tszFileDir[MAX_PATH];                                           //文件地址
	TCHAR tszClientAddr[128];                                             //操作的用户地址
	TCHAR tszFileHash[128];                                               //文件HASH值
	TCHAR tszBuckKey[128];                                                //BUCKKey
	__int64x ullCount;                                                    //总大小
	__int64x ullRWCount;                                                  //需要读取(写入)总大小
	__int64x ullRWLen;                                                    //已经读取(写入)的大小
	__int64x ullPosStart;                                                 //开始位置
	__int64x ullPosEnd;                                                   //结束位置

	int nLimit;                                                           //限制工具
	XHANDLE xhToken;
	FILE* pSt_File;
}SESSION_STORAGEINFO;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Session_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                        用户管理导出的函数                            */
/************************************************************************/
/********************************************************************
函数名称：Session_User_Init
函数功能：初始化用户管理器
 参数.一：lpszUserFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户文件地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_User_Init(LPCTSTR lpszUserFile);
/********************************************************************
函数名称：Session_User_Destory
函数功能：销毁管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_User_Destory();
/********************************************************************
函数名称：Session_User_Exist
函数功能：用户名和密码是否存在
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要判断的用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要判断的密码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_User_Exist(LPCTSTR lpszUser, LPCTSTR lpszPass);
/************************************************************************/
/*                        存储会话导出的函数                            */
/************************************************************************/
/********************************************************************
函数名称：Session_DLStroage_Init
函数功能：初始化下载会话管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_Init();
/********************************************************************
函数名称：Session_DLStroage_Destory
函数功能：销毁下载管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_Destory();
/********************************************************************
函数名称：Session_DLStroage_Insert
函数功能：插入一个会话到下载器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入BUCKET名称
 参数.三：lpszFileDir
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件地址
 参数.四：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出文件大小
 参数.五：pInt_LeftCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出需要读取大小
 参数.六：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入开始位置
 参数.七：nPostEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
 参数.八：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：文件的HASH值
 参数.九：xhToken
  In/Out：In
  类型：句柄
  可空：Y
  意思：输入限制器句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszBuckKey, LPCTSTR lpszFileDir, __int64x * pInt_Count, __int64x * pInt_LeftCount, int nPosStart = 0, int nPostEnd = 0, LPCTSTR lpszFileHash = NULL, int nLimit = 0, XHANDLE xhToken = NULL);
/********************************************************************
函数名称：Session_DLStroage_GetBuffer
函数功能：获得下载器中指定缓冲区
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：ptszMsgBuffer
  In/Out：In
  类型：字符指针
  可空：N
 参数.三：pInt_MsgLen
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出获取的缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_GetBuffer(LPCTSTR lpszClientAddr, TCHAR * ptszMsgBuffer, int* pInt_MsgLen);
/********************************************************************
函数名称：Session_DLStroage_GetInfo
函数功能：获取下载信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：pSt_StorageInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_GetInfo(LPCTSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
/********************************************************************
函数名称：Session_DLStroage_GetCount
函数功能：获取队列拥有的个数
 参数.一：pInt_ListCount
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出要发送的队列个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_GetCount(int* pInt_ListCount);
/********************************************************************
函数名称：Session_DLStorage_SetSeek
函数功能：移动文件指针
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：nSeek
  In/Out：In
  类型：整数型
  可空：N
  意思：输入文件位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStorage_SetSeek(LPCTSTR lpszClientAddr, int nSeek);
/********************************************************************
函数名称：Session_DLStorage_GetAll
函数功能：获取下载池的任务列表
 参数.一：pppSt_StorageInfo
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的下载信息列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
/********************************************************************
函数名称：Session_DLStroage_Delete
函数功能：删除一个队列
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_Delete(LPCTSTR lpszClientAddr);
/********************************************************************
函数名称：Session_UPStroage_Init
函数功能：初始化上传会话管理器
 参数.一：bUPResume
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用断点上传
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_UPStroage_Init(BOOL bUPResume = FALSE);
/********************************************************************
函数名称：Session_UPStroage_Destory
函数功能：销毁下载管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_UPStroage_Destory();
/********************************************************************
函数名称：Session_UPStroage_Insert
函数功能：插入一个会话到下载器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入BUCKET名称
 参数.三：lpszFileDir
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件地址
 参数.四：nFileSize
  In/Out：Out
  类型：整数型
  可空：N
  意思：输入文件大小
 参数.五：nLeftCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入需要写入的大小
 参数.六：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入起始位置
 参数.七：nPostEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_UPStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszBuckKey, LPCTSTR lpszFileDir, __int64x nFileSize, __int64x nLeftCount, int nPosStart = 0, int nPostEnd = 0);
/********************************************************************
函数名称：Session_UPStroage_GetInfo
函数功能：获取上传客户端信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：pSt_StorageInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_UPStroage_GetInfo(LPCTSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
/********************************************************************
函数名称：Session_UPStroage_Write
函数功能：写入数据到文件
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要写入的数据
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入写入大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_UPStroage_Write(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);
/********************************************************************
函数名称：Session_UPStroage_Exist
函数功能：客户端是否存在
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_UPStroage_Exist(LPCTSTR lpszClientAddr);
/********************************************************************
函数名称：Session_UPStorage_GetAll
函数功能：获取所有上传信息
 参数.一：pppSt_StorageInfo
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的上传信息列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_UPStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
/********************************************************************
函数名称：Session_UPStroage_Delete
函数功能：删除上传会话
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_UPStroage_Delete(LPCTSTR lpszClientAddr);