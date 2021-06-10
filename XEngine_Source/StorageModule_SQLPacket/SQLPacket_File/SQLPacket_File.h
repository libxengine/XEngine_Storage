#pragma once
/********************************************************************
//    Created:     2021/06/09  11:17:23
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_SQLPacket\SQLPacket_File\SQLPacket_File.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_SQLPacket\SQLPacket_File
//    File Base:   SQLPacket_File
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     文件管理
//    History:
*********************************************************************/

class CSQLPacket_File
{
public:
    CSQLPacket_File();
    ~CSQLPacket_File();
public:
	BOOL SQLPacket_File_Connect(LPCTSTR lpszSQLAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass);
	BOOL SQLPacket_File_Close();
public:
    BOOL SQLPacket_File_Insert(XENGINE_PROTOCOLFILE* pSt_ProtocolFile);
    BOOL SQLPacket_File_Delete(XENGINE_PROTOCOLFILE* pSt_ProtocolFile);
    BOOL SQLPacket_File_QueryAll(XENGINE_PROTOCOLFILE*** pppSt_ProtocolFile, int* pInt_ListCount);
private:
    XHDATA xhSQL;
};
