#pragma once
/********************************************************************
//	Created:	2019/6/28   14:56
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageCore\XStorageCore_Error.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageCore
//	File Base:	XStorageCore_Error
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	X存储服务导出错误
//	History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的数据库错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_XENGINE_XSTROGE_CORE_DB_INIT_PARAMENT 0x130B0001          //初始化失败.参数错误
#define ERROR_XENGINE_XSTROGE_CORE_DB_INIT_THREAD 0x130B0002            //初始化线程失败
#define ERROR_XENGINE_XSTROGE_CORE_DB_INSERTFILE_PARAMENT 0x130B0010    //参数错误
#define ERROR_XENGINE_XSTROGE_CORE_DB_DELETEFILE_PARAMENT 0x130B0020    //参数错误
#define ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_PARAMENT 0x130B0030     //参数错误
#define ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_EMPTY 0x130B0031        //查找数据为空
#define ERROR_XENGINE_XSTROGE_CORE_DB_GETCOUNT_PARAMENT 0x130B0040      //参数错误
#define ERROR_XENGINE_XSTROGE_CORE_DB_GETCOUNT_BROKE 0x130B0041         //数据库被损坏
#define ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILETABLE_PARAMENT 0x130B00B1//参数错误
#define ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILETABLE_EMPTY 0x130B00B2   //空的表
#define ERROR_XENGINE_XSTROGE_CORE_DB_QUERYMD5_PARAMENT 0x130B00D0      //参数错误
#define ERROR_XENGINE_XSTROGE_CORE_DB_QUERYMD5_NOTFOUND 0x130B00D1      //没有找到