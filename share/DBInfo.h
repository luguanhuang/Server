#ifndef __DBINFO_H__
#define __DBINFO_H__

struct DBInfo {
	DBInfo(): m_threads(1) {}

	std::string m_ip;
	std::string m_user;
	std::string m_pswd;
	std::string m_port;
	std::string m_database;
	UINT32 m_threads;
};

namespace tinyxml2
{
	class XMLElement;
}

bool ReadDBInfo(DBInfo &info, tinyxml2::XMLElement *element);

#endif // __DBINFO_H__