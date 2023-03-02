#ifndef _H_Mysql_Task_H__
#define _H_Mysql_Task_H__

#include "imysql.h"


enum EMysqlError
{
	MYSQL_ERR_SUCESS,
	MYSQL_ERR_FAILED,
	MYSQL_ERR_EXIST,
	MYSQL_ERR_NOT_EXIST,
};


class CMysqlTask
{
public:
	explicit CMysqlTask(const char* pszName)
		:m_qwBeginTime(0)
		,m_qwEndTime(0)
		,m_qwSqlBeginTime(0)
		,m_qwSqlEndTime(0)
		,m_nMysqlErr(MYSQL_ERR_SUCESS)
		,m_strName(pszName)
	{
	}
	virtual ~CMysqlTask()
	{
	}

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength) = 0;
	virtual void OnReply() = 0;


	void SetBeginTime(UINT64 qwTime) { m_qwBeginTime = qwTime; }
	void SetEndTime(UINT64 qwTime) { m_qwEndTime = qwTime; }
	void SetSqlBeginTime(UINT64 qwTime) { m_qwSqlBeginTime = qwTime; }
	void SetSqlEndTime(UINT64 qwTime) { m_qwSqlEndTime = qwTime; }
	void SetMysqlError(EMysqlError nErr) { m_nMysqlErr = nErr; }

	UINT32 GetTimeUsed() const { return m_qwEndTime > m_qwBeginTime ? (UINT32)(m_qwEndTime - m_qwBeginTime) : 0; }
	UINT32 GetSqlTimeUsed() const { return m_qwSqlEndTime > m_qwSqlBeginTime ? (UINT32)(m_qwSqlEndTime - m_qwSqlBeginTime) : 0; }

	EMysqlError GetMysqlError() const	{ return m_nMysqlErr; }
	const std::string& GetName() const	{ return m_strName; }
	std::stringstream& GetErrStream()	{ return m_oErrStream; }
public:
	UINT64		m_qwBeginTime;
	UINT64		m_qwEndTime;
	UINT64		m_qwSqlBeginTime;
	UINT64		m_qwSqlEndTime;

	EMysqlError	m_nMysqlErr;
	std::string	m_strName; 
	std::stringstream	m_oErrStream;
};


#endif