#ifndef __URLAUX_H__
#define __URLAUX_H__

struct QueryParam
{
	std::string key;
	std::string value;
};

// 处理有重复字段的Query
extern void ParserQuery(std::list<QueryParam> &params, const char *query);

// 处理没有重复字段的Query
extern void ParserQuery(std::map<std::string, std::string> &params, const char *query);

#endif