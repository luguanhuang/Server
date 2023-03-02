#ifndef _H_Redis_Task_H__
#define _H_Redis_Task_H__

#include "redis/hiredis.h"

enum ERedisError
{
	REDIS_ERR_SUCESS,
	REDIS_ERR_FAILED,
	REDIS_ERR_EXIST,
	REDIS_ERR_NOT_EXIST,
};

class RedisConn;
class CRedisTask
{
public:
	explicit CRedisTask(const char* pszName)
		:m_qwBeginTime(0)
		,m_qwEndTime(0)
		,m_qwRedisBeginTime(0)
		,m_qwRedisEndTime(0)
		,m_nRedisErr(REDIS_ERR_SUCESS)
		,m_strName(pszName)
	{
	}
	virtual ~CRedisTask()
	{
	}

	virtual void Execute(RedisConn* conn/*, char* pcBuffer, INT32 nLength*/) = 0;
	virtual void OnReply() = 0;


	void SetBeginTime(UINT64 qwTime) { m_qwBeginTime = qwTime; }
	void SetEndTime(UINT64 qwTime) { m_qwEndTime = qwTime; }
	void SetRedisBeginTime(UINT64 qwTime) { m_qwRedisBeginTime = qwTime; }
	void SetRedisEndTime(UINT64 qwTime) { m_qwRedisEndTime = qwTime; }
	void SetRedisError(ERedisError nErr) { m_nRedisErr = nErr; }

	UINT32 GetTimeUsed() const { return m_qwEndTime > m_qwBeginTime ? (UINT32)(m_qwEndTime - m_qwBeginTime) : 0; }
	UINT32 GetRedisTimeUsed() const { return m_qwRedisEndTime > m_qwRedisBeginTime ? (UINT32)(m_qwRedisEndTime - m_qwRedisBeginTime) : 0; }

	ERedisError GetRedisError() const	{ return m_nRedisErr; }
	const std::string& GetName() const	{ return m_strName; }
	std::stringstream& GetErrStream()	{ return m_oErrStream; }
public:
	UINT64		m_qwBeginTime;
	UINT64		m_qwEndTime;
	UINT64		m_qwRedisBeginTime;
	UINT64		m_qwRedisEndTime;

	ERedisError	m_nRedisErr;
	std::string	m_strName; 
	std::stringstream	m_oErrStream;
};


#endif