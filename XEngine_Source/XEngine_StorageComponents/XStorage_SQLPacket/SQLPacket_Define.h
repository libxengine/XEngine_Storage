#pragma once
/********************************************************************
//	Created:	2019/6/28   14:56
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageCore\XStorageCore_Define.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageCore
//	File Base:	XStorageCore_Define
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	X存储服务导出函数
//	History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
///                        导出数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct tag_XStorageCore_DBFILE
{
    XENGINE_PROTOCOL_FILE st_ProtocolFile;
    CHAR tszBuckKey[MAX_PATH];                                             //路径的关键字
    CHAR tszTableName[64];                                                 //日期表名称,插入:表示自定义插入日期表,获取:表示导出这个文件所属日期表
}XSTORAGECORE_DBFILE, *LPXSTORAGECORE_DBFILE;
typedef struct tag_XStorageCore_UserInfo
{
    XENGINE_PROTOCOL_USERINFO st_ProtocolUser;
    __int64x nFileCount;                                                   //拥有的文件个数
    __int64x nFileSize;                                                    //文件占用大小
    XNETHANDLE xhToken;                                                    //用户临时会话句柄
    CHAR tszIPAddr[64];                                                    //最后登录IP地址
}XSTORAGECORE_USERINFO, *LPXSTORAGECORE_USERINFO;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD XStorageDB_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的数据库操作函数                         */
/************************************************************************/
extern "C" BOOL XStorage_MySql_Init(DATABASE_MYSQL_CONNECTINFO * pSt_DBConnector, int nTimeMonth = 1);
extern "C" BOOL XStorage_MySql_Destory();
/********************************************************************
函数名称：XStorage_MySql_FileInsert
函数功能：插入一个文件数据到数据库中
 参数.一：pSt_DBManage
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的数据信息
返回值
  类型：逻辑型
  意思：是否成功
备注：这个结构所有值都必须填充
*********************************************************************/
extern "C" BOOL XStorage_MySql_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage);
/********************************************************************
函数名称：XStorage_MySql_FileDelete
函数功能：删除一个数据库文件信息
 参数.一：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件全路径
 参数.二：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：参数不能全为空,不会删除文件
*********************************************************************/
extern "C" BOOL XStorage_MySql_FileDelete(LPCSTR lpszFile = NULL, LPCSTR lpszHash = NULL);
/********************************************************************
函数名称：XStorage_MySql_FileQuery
函数功能：查询文件信息
 参数.一：pppSt_ListFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出查询到的文件列表,此函数需要调用基础库的内存释放函数
 参数.二：pInt_ListCount
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出文件个数
 参数.三：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查找开始时间,20190701
 参数.四：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查找结束时间,20190730
 参数.五：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的名称
 参数.六：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的文件MD5
返回值
  类型：逻辑型
  意思：是否成功
备注：返回假可能没有查找到,这条记录不存在.参数lpszFile和lpszMD5不能全为空
*********************************************************************/
extern "C" BOOL XStorage_MySql_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCSTR lpszTimeStart = NULL, LPCSTR lpszTimeEnd = NULL, LPCSTR lpszFile = NULL, LPCSTR lpszHash = NULL);
/********************************************************************
函数名称：XStorage_MySql_FileQueryForTable
函数功能：通过指定表名称查询所有文件
 参数.一：pppSt_ListFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出查询到的文件信息
 参数.二：pInt_ListCount
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出获取到的列表个数
 参数.三：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查询的表名称
返回值
  类型：逻辑型
  意思：是否成功
备注：参数一需要调用基础库的内存释放函数进行释放内存
*********************************************************************/
extern "C" BOOL XStorage_MySql_FileQueryForTable(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCSTR lpszTableName);
/********************************************************************
函数名称：XStorage_MySql_FileQueryForHash
函数功能：通过HASH查询文件信息
 参数.一：pSt_FileInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出查询到的文件信息
 参数.二：lpszFileMD5
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查询的文件MD5
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件所属用户
 参数.四：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入开始时间
 参数.五：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入结束时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL XStorage_MySql_FileQueryForHash(XSTORAGECORE_DBFILE* pSt_FileInfo, LPCSTR lpszFileMD5, LPCSTR lpszUser = NULL, LPCSTR lpszTimeStart = NULL, LPCSTR lpszTimeEnd = NULL);
/************************************************************************/
/*                         SQLITE数据库函数                             */
/************************************************************************/
/********************************************************************
函数名称：XStorage_SQLite_Init
函数功能：初始化SQLITE文件系统
 参数.一：lpszSQLFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的SQL文件
 参数.二：nTimeMonth
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入允许保存的时间,单位月
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL XStorage_SQLite_Init(LPCTSTR lpszSQLFile, int nTimeMonth = 1);
/********************************************************************
函数名称：XStorage_SQLite_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL XStorage_SQLite_Destory();
/********************************************************************
函数名称：XStorage_SQLite_FileInsert
函数功能：插入一个文件数据到数据库中
 参数.一：pSt_DBManage
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的数据信息
返回值
  类型：逻辑型
  意思：是否成功
备注：这个结构所有值都必须填充
*********************************************************************/
extern "C" BOOL XStorage_SQLite_FileInsert(XSTORAGECORE_DBFILE* pSt_DBManage);
/********************************************************************
函数名称：XStorage_SQLite_FileDelete
函数功能：删除一个数据库文件信息
 参数.一：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件全路径
 参数.二：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：参数不能全为空,不会删除文件
*********************************************************************/
extern "C" BOOL XStorage_SQLite_FileDelete(LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
/********************************************************************
函数名称：XStorage_SQLite_FileQuery
函数功能：查询文件信息
 参数.一：pppSt_ListFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出查询到的文件列表,此函数需要调用基础库的内存释放函数
 参数.二：pInt_ListCount
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出文件个数
 参数.三：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查找开始时间,20190701
 参数.四：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查找结束时间,20190730
 参数.五：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的名称
 参数.六：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：返回假可能没有查找到,这条记录不存在.参数lpszFile和lpszHash不能全为空
*********************************************************************/
extern "C" BOOL XStorage_SQLite_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
/********************************************************************
函数名称：XStorage_SQLite_FileQueryForTable
函数功能：通过指定表名称查询所有文件
 参数.一：pppSt_ListFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出查询到的文件信息
 参数.二：pInt_ListCount
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出获取到的列表个数
 参数.三：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查询的表名称
返回值
  类型：逻辑型
  意思：是否成功
备注：参数一需要调用基础库的内存释放函数进行释放内存
*********************************************************************/
extern "C" BOOL XStorage_SQLite_FileQueryForTable(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName);