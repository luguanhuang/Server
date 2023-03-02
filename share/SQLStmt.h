#ifndef __SQLSTMT_H__
#define __SQLSTMT_H__
typedef bool my_bool;
#include <mysql.h>
#include "LogInit.h"

template <int InBind, int outBind>
class SQLStmt
{
public:
	SQLStmt(MYSQL *mysql)
	{
		m_inBindIdx = 0;
		m_outBindIdx = 0;
		m_sql = mysql;
		m_stmt = NULL;

		memset(m_inBind, 0, sizeof(m_inBind));
		memset(m_outBind, 0, sizeof(m_outBind));
		memset(m_inLength, 0, sizeof(m_inLength));
		memset(m_outLength, 0, sizeof(m_outLength));
		memset(m_isnull, 0, sizeof(m_isnull));
		memset(m_error, 0, sizeof(m_error));

		
	}

	~SQLStmt()
	{
		if (m_stmt != NULL)
		{
			mysql_stmt_close(m_stmt);
		}
	}

	bool SetQuery(const char *query)
	{
		return SetQuery(query, strlen(query));
	}

	bool SetQuery(const char *query, int len)
	{
		m_stmt = mysql_stmt_init(m_sql);
		if (m_stmt == NULL)
		{
			return false;
		}

		if (mysql_stmt_prepare(m_stmt, query, len) != 0)
		{
			SSWarn<<mysql_stmt_error(m_stmt)<<END;
			return false;
		}

		return true;
	}

	void BindIn(char *buffer, int len)
	{
		if (m_inBindIdx < InBind)
		{
			m_inLength[m_inBindIdx] = len;
			m_inBind[m_inBindIdx].buffer = (void *)buffer;
			m_inBind[m_inBindIdx].buffer_type = MYSQL_TYPE_STRING;
			m_inBind[m_inBindIdx].buffer_length = m_inLength[m_inBindIdx]+1;
			m_inBind[m_inBindIdx].is_null = 0;
			m_inBind[m_inBindIdx].length = &m_inLength[m_inBindIdx];
			++m_inBindIdx;
		}
	}

	void BindIn(unsigned short *pSH)
	{
		if (m_inBindIdx < InBind)
		{
			m_inLength[m_inBindIdx] = sizeof(unsigned short);
			m_inBind[m_inBindIdx].buffer = (void *)pSH;
			m_inBind[m_inBindIdx].buffer_type = MYSQL_TYPE_SHORT;
			m_inBind[m_inBindIdx].buffer_length = m_inLength[m_inBindIdx];
			m_inBind[m_inBindIdx].is_null = 0;
			m_inBind[m_inBindIdx].length = &m_inLength[m_inBindIdx];
			++m_inBindIdx;
		}
	}

	void BindIn(int *pInt)
	{
		if (m_inBindIdx < InBind)
		{
			m_inLength[m_inBindIdx] = sizeof(int);
			m_inBind[m_inBindIdx].buffer = (void *)pInt;
			m_inBind[m_inBindIdx].buffer_type = MYSQL_TYPE_LONG;
			m_inBind[m_inBindIdx].buffer_length = m_inLength[m_inBindIdx];
			m_inBind[m_inBindIdx].is_null = 0;
			m_inBind[m_inBindIdx].length = &m_inLength[m_inBindIdx];
			++m_inBindIdx;
		}
	}

	void BindIn(UINT32 *pInt)
	{
		if (m_inBindIdx < InBind)
		{
			m_inLength[m_inBindIdx] = sizeof(UINT32);
			m_inBind[m_inBindIdx].buffer = (void *)pInt;
			m_inBind[m_inBindIdx].buffer_type = MYSQL_TYPE_LONG;
			m_inBind[m_inBindIdx].buffer_length = m_inLength[m_inBindIdx];
			m_inBind[m_inBindIdx].is_null = 0;
			m_inBind[m_inBindIdx].length = &m_inLength[m_inBindIdx];
			++m_inBindIdx;
		}
	}

	void BindIn(UINT64 *pInt)
	{
		if (m_inBindIdx < InBind)
		{
			m_inLength[m_inBindIdx] = sizeof(UINT64);
			m_inBind[m_inBindIdx].buffer = (void *)pInt;
			m_inBind[m_inBindIdx].buffer_type = MYSQL_TYPE_LONGLONG;
			m_inBind[m_inBindIdx].buffer_length = m_inLength[m_inBindIdx];
			m_inBind[m_inBindIdx].is_null = 0;
			m_inBind[m_inBindIdx].length = &m_inLength[m_inBindIdx];
			++m_inBindIdx;
		}
	}

	void BindOut(char *buffer, int len)
	{
		if (m_outBindIdx < outBind)
		{
			m_outLength[m_outBindIdx] = len;
			m_outBind[m_outBindIdx].buffer_type = MYSQL_TYPE_STRING;
			m_outBind[m_outBindIdx].buffer = buffer;
			m_outBind[m_outBindIdx].length = &m_outLength[m_outBindIdx];
			m_outBind[m_outBindIdx].is_null = &m_isnull[m_outBindIdx];
			m_outBind[m_outBindIdx].buffer_length = len;
			m_outBind[m_outBindIdx].error = &m_error[m_outBindIdx];
			++m_outBindIdx;
		}
	}

	void BindOut(int *pInt)
	{
		if (m_outBindIdx < outBind)
		{
			m_outLength[m_outBindIdx] = sizeof(int);
			m_outBind[m_outBindIdx].buffer_type = MYSQL_TYPE_LONG;
			m_outBind[m_outBindIdx].buffer = pInt;
			m_outBind[m_outBindIdx].length = &m_outLength[m_outBindIdx];
			m_outBind[m_outBindIdx].is_null = &m_isnull[m_outBindIdx];
			m_outBind[m_outBindIdx].error = &m_error[m_outBindIdx];
			m_outBind[m_outBindIdx].buffer_length = sizeof(int);
			++m_outBindIdx;
		}
	}

	void BindOut(UINT32 *pInt)
	{
		if (m_outBindIdx < outBind)
		{
			m_outLength[m_outBindIdx] = sizeof(UINT32);
			m_outBind[m_outBindIdx].buffer_type = MYSQL_TYPE_LONG;
			m_outBind[m_outBindIdx].buffer = pInt;
			m_outBind[m_outBindIdx].length = &m_outLength[m_outBindIdx];
			m_outBind[m_outBindIdx].is_null = &m_isnull[m_outBindIdx];
			m_outBind[m_outBindIdx].error = &m_error[m_outBindIdx];
			m_outBind[m_outBindIdx].buffer_length = sizeof(int);
			++m_outBindIdx;
		}
	}

	void BindOut(UINT64 *pInt)
	{
		if (m_outBindIdx < outBind)
		{
			m_outLength[m_outBindIdx] = sizeof(UINT64);
			m_outBind[m_outBindIdx].buffer_type = MYSQL_TYPE_LONGLONG;
			m_outBind[m_outBindIdx].buffer = pInt;
			m_outBind[m_outBindIdx].length = &m_outLength[m_outBindIdx];
			m_outBind[m_outBindIdx].is_null = &m_isnull[m_outBindIdx];
			m_outBind[m_outBindIdx].error = &m_error[m_outBindIdx];
			m_outBind[m_outBindIdx].buffer_length = sizeof(UINT64);
			++m_outBindIdx;
		}
	}

	bool Execute()
	{
		if (mysql_stmt_bind_param(m_stmt, m_inBind) != 0)
		{
			SSWarn<<mysql_stmt_error(m_stmt)<<END;
			return false;
		}

		if (outBind > 0)
		{
			if (mysql_stmt_bind_result(m_stmt, m_outBind) != 0)
			{
				return false;
			}
		}

		if (mysql_stmt_execute(m_stmt) != 0)
		{
			return false;
		}

		if (outBind > 0)
		{
			if (mysql_stmt_store_result(m_stmt) != 0)
			{
				return false;
			}
		}

		return true;
	}

	bool FetchResult()
	{
		if (mysql_stmt_fetch(m_stmt) == 0)
		{
			return true;
		}

		return false;
	}

	int  GetError()
	{
		return mysql_errno(m_sql);
	}

	void ReportError()
	{
		LogWarn("mysql_stmt_execute error: %d:%s", mysql_errno(m_sql), mysql_error(m_sql));
	}

private:
	int m_inBindIdx;
	int m_outBindIdx;
	MYSQL *m_sql;
	MYSQL_STMT *m_stmt;
	MYSQL_BIND m_inBind[InBind+1];
	MYSQL_BIND m_outBind[outBind+1];
	unsigned long m_inLength[InBind+1];
	unsigned long m_outLength[outBind+1];
	my_bool m_isnull[outBind+1];
	my_bool m_error[outBind+1];
};

#endif // __SQLSTMT_H__