#pragma once
/********************************************************************
//    Created:     2021/07/08  15:24:30
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed\APIHelp_Distributed.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed
//    File Base:   APIHelp_Distributed
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     分布式帮助函数
//    History:
*********************************************************************/


class CAPIHelp_Distributed
{
public:
	CAPIHelp_Distributed();
	~CAPIHelp_Distributed();
public:
	BOOL APIHelp_Distributed_IsMode(list<int>* pStl_ListMode, int nMode);
	BOOL APIHelp_Distributed_RandomAddr(list<tstring>* pStl_ListAddr, TCHAR* ptszAddr);
protected:
private:
};