#include "pch.h"
#include "DBInfo.h"
#include "tinyxml2.h"
#include "util.h"

bool ReadDBInfo(DBInfo &Info, tinyxml2::XMLElement *elem)
{
	if(elem == NULL) return false;

	const char* pszIP = elem->Attribute("ip");
	const char* pszUser = elem->Attribute("user");
	const char* pszPwd = elem->Attribute("password");
	const char* pszPort = elem->Attribute("port");
	const char* pszDB = elem->Attribute("database");
	const char* pszThread = elem->Attribute("threads");

	if(pszIP == NULL)
	{
		LogError("Can't find 'ip' attribute in DB config");
		return false;
	}
	if(pszUser == NULL)
	{
		LogError("Can't find 'user' attribute in DB config");
		return false;
	}
	if(pszPwd == NULL)
	{
		LogError("Can't find 'password' attribute in DB config");
		return false;
	}
	if(pszDB == NULL)
	{
		LogError("Can't find 'database' attribute in DB config");
		return false;
	}


	Info.m_ip = pszIP;
	Info.m_user = pszUser;
	Info.m_pswd = pszPwd;
	Info.m_database = pszDB;

	if(pszPort != NULL)
	{
		Info.m_port = pszPort;
	}
	else
	{
		Info.m_port = "3306";
	}

	if (pszThread != NULL)
	{
		Info.m_threads = convert<UINT32>(pszThread);
	}
	else
	{
		Info.m_threads = 1;
	}

	return true;
}