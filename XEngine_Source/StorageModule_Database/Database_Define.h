#pragma once
/********************************************************************
//    Created:     2022/03/29  14:21:46
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Define.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database
//    File Base:   Database_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库操作导出函数
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
///                        导出数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct
{
    XENGINE_PROTOCOL_FILE st_ProtocolFile;
    CHAR tszBuckKey[MAX_PATH];                                             //路径的关键字
    CHAR tszTableName[64];                                                 //日期表名称,插入:表示自定义插入日期表,获取:表示导出这个文件所属日期表
}XSTORAGECORE_DBFILE, *LPXSTORAGECORE_DBFILE;
typedef struct
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
extern "C" DWORD Database_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的数据库帮助函数                         */
/************************************************************************/
/********************************************************************
函数名称：Database_Help_Insert
函数功能：数据插入函数
 参数.一：ptszSQLBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的插入语句
 参数.二：pSt_DBFile
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Database_Help_Insert(TCHAR* ptszSQLBuffer, XSTORAGECORE_DBFILE* pSt_DBFile);
/********************************************************************
函数名称：Database_Help_Delete
函数功能：删除语句
 参数.一：ptszSQLBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出打好包的查询语句
 参数.二：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表名称
 参数.三：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入BUCKET名
 参数.四：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件名称
 参数.五：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件HASH值
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Database_Help_Delete(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL);
/********************************************************************
函数名称：Database_Help_Query
函数功能：数据库查询打包函数
 参数.一：ptszSQLBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出打好包的查询语句
 参数.二：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表名称
 参数.三：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入BUCKET名
 参数.四：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件路径
 参数.五：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件名称
 参数.六：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件HASH值
 参数.七：lpszFileUser
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件所属用户
 参数.八：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件开始时间范围
 参数.九：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件结束时间范围
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Database_Help_Query(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFilePath = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL, LPCTSTR lpszFileUser = NULL, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
/************************************************************************/
/*                         导出的数据库操作函数                         */
/************************************************************************/
/********************************************************************
函数名称：Database_File_Init
函数功能：初始化存储服务数据库管理器
 参数.一：pSt_DBConnector
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：数据MYSQL数据库连接信息
 参数.二：nTimeDay
  In/Out：In
  类型：整数型
  可空：N
  意思：输入文件数据保留时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Database_File_Init(DATABASE_MYSQL_CONNECTINFO * pSt_DBConnector, int nTimeMonth = 1);
/********************************************************************
函数名称：Database_File_Destory
函数功能：销毁数据库管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Database_File_Destory();
/********************************************************************
函数名称：Database_File_FileInsert
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
extern "C" BOOL Database_File_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage);
/********************************************************************
函数名称：Database_File_FileDelete
函数功能：删除一个数据库文件信息
 参数.一：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：所属BUCK名称
 参数.二：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件全路径
 参数.三：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：参数不能全为空,不会删除文件
*********************************************************************/
extern "C" BOOL Database_File_FileDelete(LPCTSTR lpszBuckKey = NULL, LPCSTR lpszFile = NULL, LPCSTR lpszHash = NULL);
/********************************************************************
函数名称：Database_File_FileQuery
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
 参数.五：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查询的BUCK名
 参数.六：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的名称
 参数.七：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的文件HASH
 参数.八：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表明,为NULL所有
返回值
  类型：逻辑型
  意思：是否成功
备注：返回假可能没有查找到,这条记录不存在.参数lpszFile和lpszMD5不能全为空
*********************************************************************/
extern "C" BOOL Database_File_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCSTR lpszTimeStart = NULL, LPCSTR lpszTimeEnd = NULL, LPCTSTR lpszBuckKey = NULL, LPCSTR lpszFile = NULL, LPCSTR lpszHash = NULL, LPCTSTR lpszTableName = NULL);
/************************************************************************/
/*                         导出的客户端操作函数                         */
/************************************************************************/
/********************************************************************
函数名称：Database_Client_Init
函数功能：初始化客户端数据库系统
 参数.一：lpszSQLFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的SQL文件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Database_Client_Init(LPCTSTR lpszSQLFile);
/********************************************************************
函数名称：Database_Client_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL Database_Client_Destory();
/********************************************************************
函数名称：Database_Client_FileInsert
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
extern "C" BOOL Database_Client_FileInsert(XSTORAGECORE_DBFILE* pSt_DBManage);
/********************************************************************
函数名称：Database_Client_FileDelete
函数功能：删除一个数据库文件信息
 参数.一：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：所属BUCK名称
 参数.二：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件全路径
 参数.三：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：参数不能全为空,不会删除文件
*********************************************************************/
extern "C" BOOL Database_Client_FileDelete(LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
/********************************************************************
函数名称：Database_Client_FileQuery
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
 参数.五：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查询的BUCK名
 参数.六：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的名称
 参数.七：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的文件HASH
 参数.八：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表明,为NULL所有
返回值
  类型：逻辑型
  意思：是否成功
备注：返回假可能没有查找到,这条记录不存在.参数lpszFile和lpszHash不能全为空
*********************************************************************/
extern "C" BOOL Database_Client_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);