#include "pch.h"
#include "APIHelp_Distributed.h"
/********************************************************************
//    Created:     2021/07/08  15:26:13
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed\APIHelp_Distributed.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed
//    File Base:   APIHelp_Distributed
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     分布式帮助函数
//    History:
*********************************************************************/
CAPIHelp_Distributed::CAPIHelp_Distributed()
{

}
CAPIHelp_Distributed::~CAPIHelp_Distributed()
{

}
BOOL CAPIHelp_Distributed::APIHelp_Distributed_IsMode(list<int>* pStl_ListMode, int nMode)
{
	BOOL bFound = FALSE;
	list<int>::const_iterator stl_ListIterator = pStl_ListMode->begin();
	for (; stl_ListIterator != pStl_ListMode->end(); stl_ListIterator++)
	{
		if (nMode == *stl_ListIterator)
		{
			bFound = TRUE;
			break;
		}
	}
	return bFound;
}
BOOL CAPIHelp_Distributed::APIHelp_Distributed_RandomAddr(list<tstring>* pStl_ListAddr, TCHAR* ptszAddr)
{
	BOOL bFound = FALSE;
	XNETHANDLE xhToken = 0;

	BaseLib_OperatorHandle_Create(&xhToken, 1, pStl_ListAddr->size());
	list<tstring>::const_iterator stl_ListIterator = pStl_ListAddr->begin();
	for (XNETHANDLE i = 0; stl_ListIterator != pStl_ListAddr->end(); stl_ListIterator++, i++)
	{
		if (xhToken == i)
		{
			bFound = TRUE;
			_tcscpy(ptszAddr, stl_ListIterator->c_str());
			break;
		}
	}
	return bFound;
}