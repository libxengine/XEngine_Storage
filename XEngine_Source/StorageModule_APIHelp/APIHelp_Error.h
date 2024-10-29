#pragma once
/********************************************************************
//    Created:     2021/07/08  15:37:34
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Error.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp
//    File Base:   APIHelp_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出错误
//    History:
*********************************************************************/
#define ERROR_STORAGE_MODULE_APIHELP_PARAMENT 0x0030001               //参数错误
#define ERROR_STORAGE_MODULE_APIHELP_PARSE 0x0030002                  //解析失败
#define ERROR_STORAGE_MODULE_APIHELP_NOTFOUND 0x0030003               //没有找到
#define ERROR_STORAGE_MODULE_APIHELP_DISABLE 0x0030004                //被禁用
#define ERROR_STORAGE_MODULE_APIHELP_NOTAUTH 0x0030010                //没有验证信息
#define ERROR_STORAGE_MODULE_APIHELP_NOTSUPPORT 0x0030011             //不支持
#define ERROR_STORAGE_MODULE_APIHELP_NOTLENGTH 0x0030012              //没有长度信息
#define ERROR_STORAGE_MODULE_APIHELP_PARSELEN 0x0030013               //解析长度失败
#define ERROR_STORAGE_MODULE_APIHELP_NOTHASH 0x0030014                //没有HASH值
#define ERROR_STORAGE_MODULE_APIHELP_NOTMATCH 0x0030015               //HASH不匹配
#define ERROR_STORAGE_MODULE_APIHELP_SIZE 0x0030016                   //目录大小已经满了
#define ERROR_STORAGE_MODULE_APIHELP_CODE 0x0030017                   //服务器返回失败
#define ERROR_STORAGE_MODULE_APIHELP_FORMAT 0x0030018                 //格式化失败