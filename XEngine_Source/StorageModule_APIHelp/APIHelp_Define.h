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
/*                                                                      */
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