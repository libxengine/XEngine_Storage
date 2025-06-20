﻿#pragma once
/********************************************************************
//    Created:     2024/03/13  14:35:22
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_BTorrent\BTorrent_Define.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_BTorrent
//    File Base:   BTorrent_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     BT下载导出
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                  导出枚举型
//////////////////////////////////////////////////////////////////////////
typedef enum
{
	ENUM_BTORRENT_EVENT_TYPE_OTHER = 0,                                    //其他
	ENUM_BTORRENT_EVENT_TYPE_COMPLETE = 1,                                 //下载完成
	ENUM_BTORRENT_EVENT_TYPE_ERROR = 2,                                    //下载错误
	ENUM_BTORRENT_EVENT_TYPE_DATAERROR = 3,                                //数据写入错误
	ENUM_BTORRENT_EVENT_TYPE_WRITETMP = 4,                                 //正在写入临时文件
	ENUM_BTORRENT_EVENT_TYPE_UPNPERROR = 5,                                //UPNP错误信息通知
	ENUM_BTORRENT_EVENT_TYPE_UPNPCOMPLETE = 6,                             //UPNP端口映射完成通知
	ENUM_BTORRENT_EVENT_TYPE_STATUS = 10                                   //下载状态
}ENUM_BTORRENT_EVENT_TYPE;
typedef enum
{
	ENUM_BTORRENT_DOWNLOAD_STATUS_OTHER = 0,                               //内部状态
	ENUM_BTORRENT_DOWNLOAD_STATUS_CHECKFILE = 1,                           //检查文件状态中
	ENUM_BTORRENT_DOWNLOAD_STATUS_DLMETADATA = 2,                          //从节点下载媒体数据中
	ENUM_BTORRENT_DOWNLOAD_STATUS_LOADING = 3,                             //下载中...
	ENUM_BTORRENT_DOWNLOAD_STATUS_FINISH = 4,                              //下载完成
	ENUM_BTORRENT_DOWNLOAD_STATUS_SEEDING = 5,                             //做种中,下载以完成.
	ENUM_BTORRENT_DOWNLOAD_STATUS_CHECKRESUME = 7                          //正在进行断点续传文件检查   
}ENUM_BTORRENT_DOWNLOAD_STATUS;
//////////////////////////////////////////////////////////////////////////
//                  导出数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct 
{
	XCHAR tszFileName[XPATH_MAX];                                          //文件名
	__int64x nDLoadTotal;                                                 //总大小
	__int64x nDLCount;                                                    //已下载大小
	__int64x nUPCount;                                                    //已上传大小
	int nDLStatus;                                                        //状态 = ENUM_BTORRENT_DOWNLOAD_STATUS
	int nDLoadRate;                                                       //下载平均速度,字节
	int nDLoadProcess;                                                    //下载进度. %
	int nDLPeers;                                                         //连接的下载节点个数
	int nUPPeers;                                                         //上传的节点个数 
}BTORRENT_DLOADER;
typedef struct
{
	XCHAR tszValue[XPATH_MAX];
	int nValue;
}BTORRENT_PARSEMAP;
typedef struct
{
	XCHAR tszFileHash[XPATH_MAX];                                          //文件HASH
	XCHAR tszFilePath[XPATH_MAX];                                          //文件路径
	XCHAR tszFileName[XPATH_MAX];                                          //文件名称
	XCHAR tszFileLink[XPATH_MAX];                                          //文件连接,如果有的话
	__int64x nFileOffset;                                                 //文件偏移
	__int64x nFileSize;                                                   //文件大小
	time_t nFileTime;                                                     //文件时间戳
	int nFileStart;                                                       //块索引范围
	int nFileEnd;                                                         //块索引范围
	XSHOT wFileAttr;                                                     //文件属性
}BTORRENT_FILEINFO;
//////////////////////////////////////////////////////////////////////////
//                  导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG BTorrent_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                          BT下载器导出函数                            */
/************************************************************************/
/********************************************************************
函数名称：BTorrent_DLoader_Create
函数功能：创建一个BT下载
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：下载的地址,可以是本地种子文件路径或者磁力链接
 参数.二：lpszSavePath
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要保存的地址,这个地址是一个本地文件夹.而不是文件名.
 参数.三：lpszTempFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：断点续传临时文件路径
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" XHANDLE BTorrent_DLoader_Create(LPCXSTR lpszAddr, LPCXSTR lpszSavePath, LPCXSTR lpszTempFile = NULL);
/********************************************************************
函数名称：BTorrent_DLoader_Query
函数功能：查询下载状态
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pppenEventList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出事件列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_DLoader_Query(XHANDLE xhToken, ENUM_BTORRENT_EVENT_TYPE*** pppenEventList, int* pInt_ListCount);
/********************************************************************
函数名称：BTorrent_DLoader_GetStatus
函数功能：获取下载状态
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pSt_DLStatus
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出查到的下载情况
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_DLoader_GetStatus(XHANDLE xhToken, BTORRENT_DLOADER * pSt_DLStatus);
/********************************************************************
函数名称：BTorrent_DLoader_SaveResume
函数功能：触发一次临时文件保存操作
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：每调用一次,将会触发一次事件信号,通知系统保存一次临时文件
*********************************************************************/
extern "C" bool BTorrent_DLoader_SaveResume(XHANDLE xhToken);
/********************************************************************
函数名称：BTorrent_DLoader_Close
函数功能：关闭一个种子下载器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_DLoader_Close(XHANDLE xhToken);
/********************************************************************
函数名称：BTorrent_DLoader_SetPause
函数功能：设置暂停或者恢复
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的
 参数.二：bPause
  In/Out：In
  类型：逻辑型
  可空：N
  意思：假为暂停,真为恢复下载
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_DLoader_SetPause(XHANDLE xhToken, bool bPause);
/********************************************************************
函数名称：BTorrent_DLoader_UPNPEnable
函数功能：启用或者关闭UPNP
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的
 参数.二：bEnable
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入启用还是关闭UPNP
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_DLoader_UPNPEnable(XHANDLE xhToken, bool bEnable = false);
/************************************************************************/
/*                          BT解析器导出函数                            */
/************************************************************************/
/********************************************************************
函数名称：BTorrent_Parse_Init
函数功能：初始化一个解析器
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：种子地址,可以是本地种子文件路径或者磁力链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" XHANDLE BTorrent_Parse_Init(LPCXSTR lpszAddr);
/********************************************************************
函数名称：BTorrent_Parse_GetNode
函数功能：获取节点信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pppSt_Parse
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析到的节点列表,此参数需要BaseLib_Memory_Free释放内存
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Parse_GetNode(XHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount);
/********************************************************************
函数名称：BTorrent_Parse_GetTracker
函数功能：获取TRACKER服务器列表
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pppSt_Parse
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析到的列表,此参数需要BaseLib_Memory_Free释放内存
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Parse_GetTracker(XHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount);
/********************************************************************
函数名称：BTorrent_Parse_GetSeeds
函数功能：获取种子列表
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pppSt_Parse
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析到的列表,此参数需要BaseLib_Memory_Free释放内存
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Parse_GetSeeds(XHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount);
/********************************************************************
函数名称：BTorrent_Parse_GetPiece
函数功能：获取块信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pInt_PieceLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出块大小
 参数.三：pInt_PieceCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出块个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Parse_GetPiece(XHANDLE xhToken, int* pInt_PieceLen, int* pInt_PieceCount);
/********************************************************************
函数名称：BTorrent_Parse_GetInfo
函数功能：获取块信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：ptszHash
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出HASH值
 参数.三：ptszCreator
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出创建者
 参数.四：ptszComment
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出种子的注释信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Parse_GetInfo(XHANDLE xhToken, XCHAR* ptszHash = NULL, XCHAR* ptszCreator = NULL, XCHAR* ptszComment = NULL);
/********************************************************************
函数名称：BTorrent_Parse_GetMagnet
函数功能：获取种子文件的磁力链接
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：ptszMagnet
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出种子文件的磁力链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Parse_GetMagnet(XHANDLE xhToken, XCHAR* ptszMagnet);
/********************************************************************
函数名称：BTorrent_Parse_GetFile
函数功能：获取文件信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：ptszFilePath
  In/Out：Out
  类型：字符指针
  可空：N
  意思：获得保存文件路径
 参数.三：pppSt_FileList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出文件列表
 参数.四：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：获取列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Parse_GetFile(XHANDLE xhToken, XCHAR* pszFilePath, BTORRENT_FILEINFO*** pppSt_FileList, int* pInt_ListCount);
/********************************************************************
函数名称：BTorrent_Parse_Destory
函数功能：销毁种子解析器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Parse_Destory(XHANDLE xhToken);
/************************************************************************/
/*                          BT种子制作导出函数                          */
/************************************************************************/
/********************************************************************
函数名称：BTorrent_Creator_Init
函数功能：初始化一个制作工具
 参数.一：lpszBTPath
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要制作的文件地址路径
 参数.二：nPieceSize
  In/Out：In
  类型：整数型
  可空：N
  意思：块大小,默认16K
返回值
  类型：句柄
  意思：导出一个句柄
备注：
*********************************************************************/
extern "C" XHANDLE BTorrent_Creator_Init(LPCXSTR lpszBTPath, int nPieceSize = 0);
/********************************************************************
函数名称：BTorrent_Creator_AddNode
函数功能：添加种子DHT节点地址
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入节点地址
 参数.三：nIndex
  In/Out：In
  类型：整数型
  可空：N
  意思：当前索引
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Creator_AddNode(XHANDLE xhToken, LPCXSTR lpszAddr, int nIndex);
/********************************************************************
函数名称：BTorrent_Creator_AddTracker
函数功能：设置TRACKER服务器地址
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入地址
 参数.三：nIndex
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入服务器索引
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Creator_AddTracker(XHANDLE xhToken, LPCXSTR lpszAddr, int nIndex = 0);
/********************************************************************
函数名称：BTorrent_Creator_AddSeeds
函数功能：添加URL种子到制作器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入地址
 参数.三：bSingle
  In/Out：In
  类型：逻辑型
  可空：N
  意思：多种子文件还是单种子文件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Creator_AddSeeds(XHANDLE xhToken, LPCXSTR lpszAddr, bool bSingle = false);
/********************************************************************
函数名称：BTorrent_Creator_SetInfo
函数功能：设置种子信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszCreator
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入制作器名称
 参数.三：lpszComment
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入附加信息
 参数.四：lpszCertBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入PEM证书缓冲区
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Creator_SetInfo(XHANDLE xhToken, LPCXSTR lpszCreator, LPCXSTR lpszComment = NULL, LPCXSTR lpszCertBuffer = NULL);
/********************************************************************
函数名称：BTorrent_Creator_GetTorrent
函数功能：获取中心信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszBTFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输出种子文件地址
 参数.三：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出种子信息到内存
 参数.四：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出内存大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Creator_GetTorrent(XHANDLE xhToken, LPCXSTR lpszBTFile = NULL, XCHAR * ptszMsgBuffer = NULL, int* pInt_MsgLen = NULL);
/********************************************************************
函数名称：BTorrent_Creator_Destory
函数功能：销毁
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool BTorrent_Creator_Destory(XHANDLE xhToken);