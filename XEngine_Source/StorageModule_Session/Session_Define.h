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
	struct  
	{
		XCHAR tszBoundStr[MAX_PATH];                                          //boundary信息
		bool bBoundMode;                                                      //是否是bound上传模式
		bool bBoundStart;
	}st_Boundary;
	XCHAR tszFileDir[MAX_PATH];                                           //文件地址
	XCHAR tszClientAddr[128];                                             //操作的用户地址
	XCHAR tszFileHash[128];                                               //文件HASH值
	XCHAR tszBuckKey[128];                                                //BUCKKey
	__int64x ullCount;                                                    //总大小
	__int64x ullRWCount;                                                  //需要读取(写入)总大小
	__int64x ullRWLen;                                                    //已经读取(写入)的大小
	__int64x ullPosStart;                                                 //开始位置
	__int64x ullPosEnd;                                                   //结束位置
	bool bRewrite;                                                        //是否为覆写
	
	int nLimit;                                                           //限制工具
	XHANDLE xhToken;
	FILE* pSt_File;
}SESSION_STORAGEINFO;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Session_GetLastError(int *pInt_SysError = NULL);
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
extern "C" bool Session_DLStroage_Init(int nMaxConnect);
/********************************************************************
函数名称：Session_DLStroage_Destory
函数功能：销毁下载管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_DLStroage_Destory();
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
extern "C" bool Session_DLStroage_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszBuckKey, LPCXSTR lpszFileDir, __int64x * pInt_Count, __int64x * pInt_LeftCount, int nPosStart = 0, int nPostEnd = 0, LPCXSTR lpszFileHash = NULL, int nLimit = 0, XHANDLE xhToken = NULL);
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
extern "C" bool Session_DLStroage_GetBuffer(LPCXSTR lpszClientAddr, XCHAR * ptszMsgBuffer, int* pInt_MsgLen);
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
extern "C" bool Session_DLStroage_GetInfo(LPCXSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
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
extern "C" bool Session_DLStroage_GetCount(int* pInt_ListCount);
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
extern "C" bool Session_DLStorage_SetSeek(LPCXSTR lpszClientAddr, int nSeek);
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
extern "C" bool Session_DLStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
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
extern "C" bool Session_DLStroage_Delete(LPCXSTR lpszClientAddr);
/********************************************************************
函数名称：Session_DLStroage_MaxConnect
函数功能：判断一个地址是否超过连接数限制
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_DLStroage_MaxConnect(LPCXSTR lpszClientAddr);
/********************************************************************
函数名称：Session_UPStroage_Init
函数功能：初始化上传会话管理器
 参数.一：nMaxConnect
  In/Out：In
  类型：整数型
  可空：N
  意思：输入连接数限制
 参数.二：bUPResume
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用断点上传
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_UPStroage_Init(int nMaxConnect, bool bUPResume = false);
/********************************************************************
函数名称：Session_UPStroage_Destory
函数功能：销毁下载管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_UPStroage_Destory();
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
 参数.五：bRewrite
  In/Out：In
  类型：整数型
  可空：N
  意思：是否允许覆写
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
extern "C" bool Session_UPStroage_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszBuckKey, LPCXSTR lpszFileDir, __int64x nFileSize, bool bRewrite, int nPosStart = 0, int nPostEnd = 0);
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
extern "C" bool Session_UPStroage_GetInfo(LPCXSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
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
extern "C" bool Session_UPStroage_Write(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);
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
extern "C" bool Session_UPStroage_Exist(LPCXSTR lpszClientAddr);
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
extern "C" bool Session_UPStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
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
extern "C" bool Session_UPStroage_Delete(LPCXSTR lpszClientAddr);
/********************************************************************
函数名称：Session_UPStroage_Close
函数功能：关闭读写文件句柄
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要关闭的客户端会话
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_UPStroage_Close(LPCXSTR lpszClientAddr);
/********************************************************************
函数名称：Session_UPStroage_MaxConnect
函数功能：判断一个地址是否超过连接数限制
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_UPStroage_MaxConnect(LPCXSTR lpszClientAddr);
/********************************************************************
函数名称：Session_UPStroage_SetBoundary
函数功能：设置bound模式信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的地址
 参数.二：lpszBoundary
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入bound字符串
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_UPStroage_SetBoundary(LPCXSTR lpszClientAddr, LPCXSTR lpszBoundary);
/********************************************************************
函数名称：Session_UPStroage_SetBoundaryStart
函数功能：设置bound模式信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_UPStroage_SetBoundaryStart(LPCXSTR lpszClientAddr);
/************************************************************************/
/*                        存储会话导出的函数                            */
/************************************************************************/
/********************************************************************
函数名称：Session_Action_Insert
函数功能：插入一个会话到动作管理器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要插入的句柄
 参数.二：xhAction
  In/Out：In
  类型：句柄
  可空：N
  意思：输入下载器句柄
 参数.三：pSt_ActionInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Action_Insert(XNETHANDLE xhToken, XHANDLE xhAction, XENGINE_ACTIONINFO* pSt_ActionInfo);
/********************************************************************
函数名称：Session_Action_GetToken
函数功能：获得一个下载器句柄
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的动作句柄
返回值
  类型：句柄型
  意思：返回下载器句柄
备注：
*********************************************************************/
extern "C" XHANDLE Session_Action_GetToken(XNETHANDLE xhToken);
/********************************************************************
函数名称：Session_Action_GetInfo
函数功能：获取下载信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的动作句柄
 参数.二：pSt_ActionInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Action_GetInfo(XNETHANDLE xhToken, XENGINE_ACTIONINFO* pSt_ActionInfo);
/********************************************************************
函数名称：Session_Action_GetAll
函数功能：获得所有动作器句柄
 参数.一：pppxhToken
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的句柄列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Action_GetAll(XNETHANDLE*** pppxhToken, int* pInt_ListCount);
/********************************************************************
函数名称：Session_Action_Delete
函数功能：删除一个动作管理器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的动作句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Action_Delete(XNETHANDLE xhToken);
/************************************************************************/
/*                        WEBDAV会话导出的函数                          */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_Webdav_Insert
函数功能：插入一个会话
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要插入的名称
 参数.二：pSt_WDLocker
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入插入的内容
 参数.三：nTimeout
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入超时时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Webdav_Insert(LPCXSTR lpszFileName, XENGINE_WEBDAVLOCK* pSt_WDLocker, int nTimeout = 3600);
/********************************************************************
函数名称：Session_Webdav_Get
函数功能：获取信息
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的文件
 参数.二：pSt_WDLocker
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Webdav_Get(LPCXSTR lpszFileName, XENGINE_WEBDAVLOCK* pSt_WDLocker);
/********************************************************************
函数名称：Session_Webdav_Delete
函数功能：删除一个会话
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的文件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Session_Webdav_Delete(LPCXSTR lpszFileName);