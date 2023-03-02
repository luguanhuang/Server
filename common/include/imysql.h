#ifndef _IMySQL_H__
#define _IMySQL_H__


//mysql interface
enum EMysqlConnEvent
{
	MYSQL_CONN_LOST_EVENT,
	MYSQL_RECONNECT_TRY_MAX_EVENT,
	MYSQL_RECONNECT_SUCCESS_EVENT,
};
typedef void(*PMysqlEventCB)(EMysqlConnEvent nMysqlEvent);


class IMysqlRecordSet
{
public:
	virtual ~IMysqlRecordSet() {}


	// 获取记录集中记录条数
	virtual UINT32 GetRowNum() = 0;


	// 获取记录集中记录列数
	virtual UINT32 GetFieldNum() = 0;
	

	// 获取下一行记录
	virtual bool FetchRow() = 0;


	// 获取当前记录行的指定列的字段值 和 数据长度
	virtual bool GetFieldValue(UINT32 dwIndex, char** ppValue, UINT32& rdwLen) = 0;


	virtual void Release() = 0;
};


class IMysqlConn
{
public:
	virtual ~IMysqlConn() {}

	// 连接到指定主机的指定数据库
	virtual bool Connect(const char* pszHost, UINT32 dwPort, const char* pszUser, const char* pszPwd, const char* pszDB) = 0;


	// 关闭连接
	virtual bool Close() = 0;


	// 执行不带返回记录集的sql语句 (insert update)
	virtual bool Query(const char* pszSQL, UINT32 dwLen, UINT32* pdwAffectedRows) = 0;


	// 执行带返回结果记录集的sql语句 (select)
	virtual bool QueryWithRecord(const char* pszSQL, UINT32 dwLen, IMysqlRecordSet** ppRecordSet, bool bStoreResult = true) = 0;


	// 将pszSrc中的字符串编码为转义SQL字符串,将结果置于pszDest中 (nDestLen 至少为 nSrcLen*2+1 字节)
	// 返回转义后字符串的长度, 不包括结尾的'\0', -1表示失败，空串时返回0
	virtual INT32 EscapeString(const char* pszSrc, INT32 nSrcLen, char* pszDest, INT32 nDestLen) = 0;


	// 获取mysql错误码
	virtual UINT32 GetErrno() = 0;


	// 获取mysql错误描述信息
	virtual const char* GetError() = 0;


	// 获取最后一次insert 操作时的 自增ID
	virtual bool GetLastInsertID(UINT64& rqwLastID) = 0;


	// 设置Mysql连接丢失回调
	virtual void SetMysqlLostCallBack(UINT32 dwTryInterval, UINT32 dwMaxTryTimes, PMysqlEventCB pMysqEvtCB) = 0;


	virtual void Release() = 0;
};


IMysqlConn* CreateMysqlConn();

#endif
