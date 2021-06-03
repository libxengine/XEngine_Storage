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
	__int64x ullCount;                                                    //总大小
	__int64x ullPos;                                                      //位置
	FILE* pSt_File;
}SESSION_STORAGEINFO;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Session_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                        存储会话导出的函数                            */
/************************************************************************/
/********************************************************************
函数名称：Session_DLStroage_Init
函数功能：初始化下载会话管理器
 参数.一：nPoolCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入最大运行多少个下载同时进行
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_Init(int nPoolCount = 1);
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
 参数.二：lpszFileDir
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件地址
 参数.三：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出文件大小
 参数.四：nPos
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入要移动的指针位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x* pInt_Count = NULL, int nPos = 0);
/********************************************************************
函数名称：Session_DLStroage_GetList
函数功能：获得下载器中的列表索引信息
 参数.一：nPool
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的队列
 参数.二：nIndex
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的队列索引
 参数.三：ptszClientAddr
  In/Out：In
  类型：整数型
  可空：N
  意思：输出客户端地址
 参数.四：ptszMsgBuffer
  In/Out：In
  类型：整数型
  可空：N
 参数.五：pInt_MsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输出获取的缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_GetList(int nPool, int nIndex, TCHAR* ptszClientAddr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
/********************************************************************
函数名称：Session_DLStroage_GetCount
函数功能：获取队列拥有的个数
 参数.一：nPool
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的队列
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出队列个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Session_DLStroage_GetCount(int nIndex, int* pInt_ListCount);
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