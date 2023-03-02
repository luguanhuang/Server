#include "pch.h"
#include "URLAux.h"

typedef void (*KeyValueCB)(const std::string &key, const std::string &value, void *arg);

static void ParserQueryInner(const char *query, KeyValueCB cb, void *arg)
{
	enum parser_state
	{
		KEY,
		VALUE,
	};

	char c;
	std::string key,value;
	parser_state state = KEY;
	while ((c = *query) != 0)
	{
		switch (state)
		{
		case KEY:
			{
				if (c == '=')
				{
					state = VALUE;
				}
				else
				{
					key += c;
				}
				break;
			}
		case VALUE:
			{
				if (c == '&')
				{
					state = KEY;

					cb(key, value, arg);
					key = "";
					value = "";
				}
				else
				{
					value += c;
				}
				break;
			}
		default:
			break;
		}

		++query;
	}

	if (state == VALUE)
	{
		cb(key, value, arg);
	} 
	else if (state == KEY && !key.empty())
	{
		cb(key, value, arg);
	}
}

static void ListParamCB(const std::string &key, const std::string &value, void *arg)
{
	std::list<QueryParam> *p = (std::list<QueryParam> *)arg;
	QueryParam param;
	param.key = key;
	param.value = value;
	p->push_back(param);
}

void ParserQuery(std::list<QueryParam> &params, const char *query)
{
	ParserQueryInner(query, ListParamCB, &params);
}

static void DictParamCB(const std::string &key, const std::string &value, void *arg)
{
	std::map<std::string, std::string> *dict = (std::map<std::string, std::string> *)arg;
	dict->insert(std::make_pair(key, value));
}

void ParserQuery(std::map<std::string, std::string> &params, const char *query)
{
	ParserQueryInner(query, DictParamCB, &params);
}
