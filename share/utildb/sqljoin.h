#ifndef _UTIL_SQL_JOIN_H_
#define _UTIL_SQL_JOIN_H_

#include <iostream>
#include <vector>
#include <string>

#include "util.h"
#include "utildb/sqlbuilder.h"


struct SqlPair
{
	enum DB_TYPE
	{
		DB_INT = 0,	//不需要escape
		DB_STR = 1, //需要escape
	};

	DB_TYPE m_type;
	std::string m_key;
	std::string m_value;

	template <typename T>
	SqlPair(const std::string& key, const T& value)
	{
		m_key = "`" + key + "`";
		m_value = ToString(value);
		m_type = DB_INT;
	}

	SqlPair(const std::string& key)
	{
		m_key = "`" + key + "`";
		m_type = DB_INT;
	}

	SqlPair(const std::string& key, const std::string& value)
	{
		m_key = "`" + key + "`";
		m_value = value;
		m_type = DB_STR;
	}

	SqlPair(const std::string& key, const char* value)
	{
		m_key = "`" + key + "`";
		m_value = std::string(value);
		m_type = DB_STR;
	}
};

struct SqlJoin
{
	std::vector<SqlPair> m_pairs;

    template <typename T>
    void addPair(const std::string& key, const T& value)
	{
		addPair(SqlPair(key, value));
    }

	void addPair(const SqlPair& p)
	{
		m_pairs.push_back(p);
	}

	SqlJoin& operator << (const SqlPair& p)
	{
		m_pairs.push_back(p);
		return *this;
	}
};

#endif
