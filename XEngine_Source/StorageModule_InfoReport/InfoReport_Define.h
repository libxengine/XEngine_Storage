#pragma once
/********************************************************************
//    Created:     2024/04/15  15:39:50
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_InfoReport\InfoReport_Define.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_InfoReport
//    File Base:   InfoReport_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     信息报告导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG InfoReport_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                        APIMachine报告                                */
/************************************************************************/
/********************************************************************
函数名称：InfoReport_APIMachine_Send
函数功能：发送一条信息报告给API服务器
 参数.一：lpszAPIUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入请求地址
返回值
  类型：逻辑型
  意思：是否成功
备注：lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=machine&params1=0");
*********************************************************************/
extern "C" bool InfoReport_APIMachine_Send(LPCXSTR lpszAPIUrl);