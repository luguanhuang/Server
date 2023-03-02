#include "pch.h"
#include "dbutil.h"
#include "imysql.h"
#include "util.h"
#include "baseconfig.h"


bool RunSqlScript(const char* pszFilePath, IMysqlConn* poMysqlConn)
{
	if(pszFilePath == NULL) return false;

	std::string dbscript = Config::GetConfig()->ReadFileContent(pszFilePath);
	std::vector<std::string> sqls = Split(dbscript, ';');
	for (UINT32 i = 0; i < sqls.size(); ++i)
	{
		std::string s = Trim(sqls[i]);
		if (s.empty() || s[0] == '#' || s[0] == '-' || s[0] == '/')
			continue;

		bool tolerateError = false;
		if (s[0] == '?')
		{
			tolerateError = true;
			s[0] = ' ';
		}

		if(!poMysqlConn->Query(s.c_str(), s.size(), NULL))
		{
			LogWarn("run sql [%s] failed!", s.c_str());
		}
	}

	return true;
}