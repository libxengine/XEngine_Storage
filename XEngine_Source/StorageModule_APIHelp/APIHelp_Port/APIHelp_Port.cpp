#include "pch.h"
#include "APIHelp_Port.h"
/********************************************************************
//    Created:     2026/09/11  14:46:00
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Port\APIHelp_Port.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Port
//    File Base:   APIHelp_Port
//    File Ext:    cpp
//    Project:     XEngine_Storage
//    Repository:  D:\XEngine_Storage (branch: develop)
//    Author:      qyt
//    Purpose:     端口处理类
//    History:
*********************************************************************/
CAPIHelp_Port::CAPIHelp_Port()
{
}
CAPIHelp_Port::~CAPIHelp_Port()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：APIHelp_Port_Init
函数功能：初始化端口列表
 参数.一：nPortStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：可用起始端口
 参数.二：nPortEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：可用结束端口
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Port::APIHelp_Port_Init(int nPortStart /* = 54000 */, int nPortEnd /* = 55000 */)
{
	APIHelp_IsErrorOccur = false;

	for (int i = nPortStart; i < nPortEnd; i++)
	{
		MODULEHELP_PORTINFO st_PortInfo = {};
		st_PortInfo.nPort = i;
		st_PortInfo.bUsed = false;

		stl_ListPort.push_back(st_PortInfo);
	}
    return true;
}
/********************************************************************
函数名称：APIHelp_Port_Destroy
函数功能：销毁端口
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Port::APIHelp_Port_Destroy()
{
	APIHelp_IsErrorOccur = false;

	stl_ListPort.clear();
	return true;
}
/********************************************************************
函数名称：APIHelp_Port_Get
函数功能：获取一个没有使用的端口
 参数.一：pInt_Port
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的端口
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Port::APIHelp_Port_Get(int* pInt_Port)
{
	APIHelp_IsErrorOccur = false;

	if (NULL == pInt_Port)
	{
		APIHelp_IsErrorOccur = true;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return false;
	}
	std::lock_guard<std::mutex> st_Lock(st_Locker);
	for (auto stl_ListIterator = stl_ListPort.begin(); stl_ListIterator != stl_ListPort.end(); stl_ListIterator++)
	{
		if (!stl_ListIterator->bUsed)
		{
			stl_ListIterator->bUsed = true;
			*pInt_Port = stl_ListIterator->nPort;
			break;
		}
	}
	return true;
}
/********************************************************************
函数名称：APIHelp_Port_Free
函数功能：还原一个端口
 参数.一：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要还原的端口号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CAPIHelp_Port::APIHelp_Port_Free(int nPort)
{
	APIHelp_IsErrorOccur = false;

	std::lock_guard<std::mutex> st_Lock(st_Locker);
	for (auto stl_ListIterator = stl_ListPort.begin(); stl_ListIterator != stl_ListPort.end(); stl_ListIterator++)
	{
		if (nPort == stl_ListIterator->nPort)
		{
			stl_ListIterator->bUsed = false;
		}
	}
	return true;
}