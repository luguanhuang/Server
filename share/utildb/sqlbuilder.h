#ifndef _UTIL_SQL_BUILDER_H_
#define _UTIL_SQL_BUILDER_H_

#include "sqljoin.h"
#include "imysql.h"


class IMysqlConn;
namespace SqlBuilder
{
	void BuildUpdateSql(IMysqlConn* poMysqlConn, std::string& sql, const std::string& tableName, const SqlJoin& columns, const std::string& condition);
	void BuildUpdateSql(IMysqlConn* poMysqlConn, std::string& sql, const std::string& tableName, const SqlJoin& columns, const SqlJoin& cond);

	void BuildInsertSql(IMysqlConn* poMysqlConn, std::string& sql, const std::string& tableName, const SqlJoin& columns);
	void BuildReplaceIntoSql(IMysqlConn* poMysqlConn, std::string& sql, const std::string& tableName, const SqlJoin& columns);
}
#endif