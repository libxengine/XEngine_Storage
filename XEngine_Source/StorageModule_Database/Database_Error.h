#pragma once
/********************************************************************
//    Created:     2022/03/29  14:21:56
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Error.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database
//    File Base:   Database_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库操作导出错误
//    History:
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