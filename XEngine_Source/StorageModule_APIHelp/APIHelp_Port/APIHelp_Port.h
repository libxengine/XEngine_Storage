#pragma once
/********************************************************************
//    Created:     2026/09/11  14:46:00
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Port\APIHelp_Port.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Port
//    File Base:   APIHelp_Port
//    File Ext:    h
//    Project:     XEngine_Storage
//    Repository:  D:\XEngine_Storage (branch: develop)
//    Author:      qyt
//    Purpose:     端口处理类
//    History:     
*********************************************************************/
typedef struct  
{
	int nPort;
	bool bUsed;
}MODULEHELP_PORTINFO;

class CAPIHelp_Port
{
public:
	CAPIHelp_Port();
	~CAPIHelp_Port();
public:
	bool APIHelp_Port_Init(int nPortStart = 54000, int nPortEnd = 55000);
	bool APIHelp_Port_Destroy();
	bool APIHelp_Port_Get(int* pInt_Port);
	bool APIHelp_Port_Free(int nPort);
private:
	std::mutex st_Locker;
private:
	std::list<MODULEHELP_PORTINFO> stl_ListPort;
};