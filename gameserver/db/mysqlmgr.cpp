#include "pch.h"
#include <string>
#include <sstream>
#include "mysqlmgr.h"
#include "config.h"
#include "util.h"


INSTANCE_SINGLETON(CMysqlMgr)


CMysqlMgr::CMysqlMgr()
:m_poMysqlConn(NULL)
{ 
}

CMysqlMgr::~CMysqlMgr()
{
}

bool CMysqlMgr::Init()
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return true;
	}
	m_poMysqlConn = CreateMysqlConn();
	if(m_poMysqlConn == NULL)
	{
		return false;
	}

	const DBInfo& dbInfo = GSConfig::Instance()->GetDBInfo();
	if(!m_poMysqlConn->Connect(dbInfo.m_ip.c_str(), convert<UINT32>(dbInfo.m_port), dbInfo.m_user.c_str(), dbInfo.m_pswd.c_str(), dbInfo.m_database.c_str()))
	{
		LogError("Can't connect to database %s:%s:%s, ErrDesc: %s", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str(), m_poMysqlConn->GetError());
		return false;
	}

	LogInfo("Connect to %s:%s:%s succ.", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str());
	return true;
}

void CMysqlMgr::Uninit()
{
	if(m_poMysqlConn != NULL)
	{
		m_poMysqlConn->Close();
		m_poMysqlConn->Release();
		m_poMysqlConn = NULL;
	}
}