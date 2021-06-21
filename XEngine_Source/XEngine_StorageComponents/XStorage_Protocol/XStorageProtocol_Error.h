#pragma once
/********************************************************************
//	Created:	2019/6/28   15:34
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Error.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol
//	File Base:	XStorageProtocol_Error
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	存储协议模块导出错误码定义
//	History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的公用协议错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_XENGINE_XSTROGE_PROTOCOL_COMM_PARAMENT 0x130C0000
//////////////////////////////////////////////////////////////////////////
//                        导出的客户端协议错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQQUERY_PARAMENT 0x130C1000 //参数错误
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQDELETE_PARAMENT 0x130C1010//参数错误
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CLIENT_REQDIRCREATE_PARAMENT 0x130C1100  //参数错误
//////////////////////////////////////////////////////////////////////////
//                        导出的服务端协议错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REQQUERYFILE_PARAMENT 0x130C2000   //参数错误
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REQQUERYFILE_PARSEJSON 0x130C2001  //解析JSON错误
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REPQUERYFILE_PARAMENT 0x130C2010   //参数错误
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_CREATEDIR_PARAMENT 0x130C2101      //参数错误
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_CREATEDIR_PARSEJSON 0x130C2102     //解析JSON错误,协议不正确
#define ERROR_XENGINE_XSTROGE_PROTOCOL_CORE_REPDIRQUERY_PARAMENT 0x130C2110    //参数错误
