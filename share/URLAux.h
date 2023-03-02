#ifndef __URLAUX_H__
#define __URLAUX_H__

struct QueryParam
{
	std::string key;
	std::string value;
};

// �������ظ��ֶε�Query
extern void ParserQuery(std::list<QueryParam> &params, const char *query);

// ����û���ظ��ֶε�Query
extern void ParserQuery(std::map<std::string, std::string> &params, const char *query);

#endif