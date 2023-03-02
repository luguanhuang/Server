#include "pch.h"
#include "sqlbuilder.h"

namespace SqlBuilder
{

void BuildUpdateSql(IMysqlConn* poMysqlConn, std::string& sql, const std::string& tableName, const SqlJoin& columns, const std::string& condition)
{
	std::ostringstream oss;
	oss << "update " << tableName << " set ";
	for (size_t i = 0; i < columns.m_pairs.size(); ++i) {
		const SqlPair& p = columns.m_pairs[i];
		if (i != 0)
		{
			oss << ",";
		}
		oss << p.m_key << "=";

		if (p.m_type == SqlPair::DB_INT)
		{
			oss << p.m_value;
		}
		else
		{
			std::string temp;
			temp.resize(p.m_value.size() * 2 + 1);
			poMysqlConn->EscapeString(p.m_value.c_str(), p.m_value.size(), (char*)temp.c_str(), temp.size());
			temp.resize(strlen(temp.c_str()));
			oss << "'" << temp.c_str() << "'";
		}
	}
	oss << " " << condition;

	sql = oss.str();
}

void BuildUpdateSql(IMysqlConn* poMysqlConn, std::string& sql, const std::string& tableName, const SqlJoin& columns, const SqlJoin& cond)
{
	std::ostringstream oss;
	oss << "update " << tableName << " set ";
	for (size_t i = 0; i < columns.m_pairs.size(); ++i)
	{
		const SqlPair& p = columns.m_pairs[i];
		if (i != 0)
		{
			oss << ",";
		}
		oss << p.m_key << "=";
		if (p.m_type == SqlPair::DB_INT)
		{
			oss << p.m_value;
		} 
		else
		{
			std::string temp;
			temp.resize(p.m_value.size() * 2 + 1);
			poMysqlConn->EscapeString(p.m_value.c_str(), p.m_value.size(), (char*)temp.c_str(), temp.size());
			temp.resize(strlen(temp.c_str()));
			oss << "'" << temp.c_str() << "'";
		}
	}
	oss << " where ";
	for (size_t i = 0; i < cond.m_pairs.size(); ++i)
	{
		const SqlPair& p = cond.m_pairs[i];
		if (i != 0)
		{
			oss << " and ";
		}
		oss << p.m_key << "=";

		if (p.m_type == SqlPair::DB_INT)
		{
			oss << p.m_value;
		}
		else
		{
			std::string temp;
			temp.resize(p.m_value.size() * 2 + 1);
			poMysqlConn->EscapeString(p.m_value.c_str(), p.m_value.size(), (char*)temp.c_str(), temp.size());
			temp.resize(strlen(temp.c_str()));
			oss << "'" << temp.c_str() << "'";
		}
	}
	sql = oss.str();
}

void BuildInsertSql(IMysqlConn* poMysqlConn, std::string& sql, const std::string& tableName, const SqlJoin& columns)
{
	std::ostringstream oss;
	oss << "insert into " << tableName << "(";
	for (size_t i = 0; i < columns.m_pairs.size(); ++i)
	{
		const SqlPair& p = columns.m_pairs[i];
		if (i != 0)
		{
			oss << ",";
		} 
		oss << p.m_key;
	}
	oss << ") values(";
	for (size_t i = 0; i < columns.m_pairs.size(); ++i)
	{
		const SqlPair& p = columns.m_pairs[i];
		if (i != 0)
		{
			oss << "," ;
		}
		if (p.m_type == SqlPair::DB_INT)
		{
			oss << p.m_value;
		} 
		else
		{
			std::string temp;
			temp.resize(p.m_value.size() * 2 + 1);
			poMysqlConn->EscapeString(p.m_value.c_str(), p.m_value.size(), (char*)temp.c_str(), temp.size());
			temp.resize(strlen(temp.c_str()));
			oss << "'" << temp.c_str() << "'";
		}
	}
	oss << ")";
	sql = oss.str();	
}

void BuildReplaceIntoSql(IMysqlConn* poMysqlConn, std::string& sql, const std::string& tableName, const SqlJoin& columns)
{
	std::ostringstream oss;
	oss << "replace into " << tableName << "(";
	for (size_t i = 0; i < columns.m_pairs.size(); ++i)
	{
		const SqlPair& p = columns.m_pairs[i];
		if (i != 0)
		{
			oss << ",";
		} 
		oss << p.m_key;
	}
	oss << ") values(";
	for (size_t i = 0; i < columns.m_pairs.size(); ++i)
	{
		const SqlPair& p = columns.m_pairs[i];
		if (i != 0)
		{
			oss << "," ;
		}
		if (p.m_type == SqlPair::DB_INT)
		{
			oss << p.m_value;
		} 
		else
		{
			std::string temp;
			temp.resize(p.m_value.size() * 2 + 1);
			poMysqlConn->EscapeString(p.m_value.c_str(), p.m_value.size(), (char*)temp.c_str(), temp.size());
			temp.resize(strlen(temp.c_str()));
			oss << "'" << temp.c_str() << "'";
		}
	}
	oss << ")";
	sql = oss.str();	
}

}