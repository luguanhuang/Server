#include "pch.h"
#include "mysqlmgr.h"
#include "config.h"
#include "util.h"
#include "SQLStmt.h"
#include "timeutil.h"

#define MYSQL_ERROR(mysql) LogError("SQL failed: errno=%d : %s\n", mysql_errno(mysql), mysql_error(mysql))

INSTANCE_SINGLETON(MysqlMgr);

MysqlMgr::MysqlMgr()
{

}

MysqlMgr::~MysqlMgr()
{

}

bool MysqlMgr::Init()
{
	const DBInfo &dbInfo = LoginConfig::Instance()->GetDBInfo();

	m_sql = mysql_init(NULL);
	if(m_sql == NULL)
	{
		LogError("mysql_init failed");
		return false;
	}

	if (mysql_real_connect(m_sql,
		dbInfo.m_ip.c_str(),
		dbInfo.m_user.c_str(),
		dbInfo.m_pswd.c_str(),
		dbInfo.m_database.c_str(),
		convert<int>(dbInfo.m_port), 
		NULL, 0) == NULL)
	{
		MYSQL_ERROR(m_sql);
		mysql_close(m_sql);
		m_sql = NULL;

		LogWarn("connect to database %s:%s:%s failed", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str());
		return false;
	}
	else
	{
		///> 数据库连接断开自动重连
		my_bool reconnect = 1;
		mysql_options(m_sql, MYSQL_OPT_RECONNECT, &reconnect);
		mysql_set_character_set(m_sql, "utf8");
	}

	LogInfo("connect to database %s:%s:%s success", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str());

	RunInitScript();
	return true;
}

void MysqlMgr::Uninit()
{
	if (m_sql != NULL)
	{
		mysql_close(m_sql);
		m_sql = NULL;
	}
}

bool MysqlMgr::AddGMAccount(const std::string& account)
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}

	const char query[] = "insert into gmaccount(account) values(?)";

	SQLStmt<1,0> stmt(m_sql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}
	stmt.BindIn(const_cast<char*>(account.c_str()), account.size());

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}

	return true;
}

bool MysqlMgr::DelGMAccount(const std::string& account)
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}

	const char query[] = "delete from gmaccount where account = ?";

	SQLStmt<1,0> stmt(m_sql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}
	stmt.BindIn(const_cast<char*>(account.c_str()), account.size());

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}

	return true;
}


bool MysqlMgr::QueryGMAccount(std::set<std::string>& accounts)
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}

	const char query[] = "select account from gmaccount";

	SQLStmt<0,1> stmt(m_sql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}

	char account[128];

	stmt.BindOut(account, 128);

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}
	while (stmt.FetchResult())
	{
		accounts.insert(account);
	}
	return true;
}

bool MysqlMgr::CleanBanAccount()
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}
	const char query[] = "delete from banaccount where endtime <= ?";
	SQLStmt<1,0> stmt(m_sql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}
	INT32 nTime = TimeUtil::GetTime();
	stmt.BindIn(&nTime);

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}
	return true;
}

bool MysqlMgr::DelBanAccount(const std::string& openid)
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}

	const char query[] = "delete from banaccount where openid = ?";

	SQLStmt<1,0> stmt(m_sql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}
	stmt.BindIn(const_cast<char*>(openid.c_str()), openid.size());

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}
	return true;
}


bool MysqlMgr::QueryBanAccount(std::unordered_map<std::string, KKSG::BanAccount> &mapBanAccount)
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}
	const char query[] = "select `openid`,`endtime`,`reason` from banaccount";
	SQLStmt<0,3> stmt(m_sql);
	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}
	char openid[1024] = {0};
	INT32 endtime = 0;
	char reason[2048] = {0};

	stmt.BindOut(openid,1024);
	stmt.BindOut(&endtime);
	stmt.BindOut(reason,2048);
	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}

	while (stmt.FetchResult())
	{
		KKSG::BanAccount oData;
		UINT32 nTime = TimeUtil::GetTime();
		oData.set_openid(openid);
		oData.set_endtime(endtime);
		oData.set_reason(reason);
		if (nTime < endtime)
		{
			auto iter = mapBanAccount.find(openid);
			if (iter!=mapBanAccount.end())
			{
				iter->second = oData;
			}else
			{
				mapBanAccount.insert(std::pair<std::string,KKSG::BanAccount>(openid, oData));
			}
		}

	}
	return true;
}


bool MysqlMgr::QueryNoticeList(std::vector<KKSG::PlatNotice>& list)
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}

	const char query[] = "select `type`,`id`,`isopen`,`areaid`,`platid`,`content`,`updatetime` from notice";

	SQLStmt<0,7> stmt(m_sql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}

	INT32 iType = 0;
	INT32 iID = 0;
	INT32 iIsOpen = 1;
	INT32 iAreaID = 0;
	INT32 iPlatID = 0;
	char content[12800]= {0};
	INT32 iUpdateTime = 0;

	stmt.BindOut(&iType);
	stmt.BindOut(&iID);
	stmt.BindOut(&iIsOpen);
	stmt.BindOut(&iAreaID);
	stmt.BindOut(&iPlatID);
	stmt.BindOut(content, 12800);
	stmt.BindOut(&iUpdateTime);

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}
	while (stmt.FetchResult())
	{
		if (iIsOpen == 0)
		{
			KKSG::PlatNotice oData;
			oData.set_type(iType);
			oData.set_noticeid(iID);
			oData.set_isopen(true);
			oData.set_areaid(iAreaID);
			oData.set_platid(iPlatID);
			oData.set_content(content);
			oData.set_updatetime(iUpdateTime);
			oData.set_isnew(true);
			list.push_back(oData);
		}
	}
	return true;
}

bool MysqlMgr::UpdateBanAccount(const KKSG::BanAccount &acc)
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}
	const char query[] = "replace into banaccount(`openid`,`endtime`,`reason`) values(?,?,?)";

	SQLStmt<3,0> stmt(m_sql);
	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}
	INT32 nEndTime = acc.endtime();
	
	stmt.BindIn(const_cast<char*>(acc.openid().c_str()),acc.openid().size());
	stmt.BindIn(&nEndTime);
	stmt.BindIn(const_cast<char*>(acc.reason().c_str()),acc.reason().size());

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}
	return true;

}


bool MysqlMgr::UpdateNoticeInfo(const KKSG::PlatNotice roInfo)
{
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}

	const char query[] = "insert into notice(`type`,`id`,`isopen`,`areaid`,`platid`,`content`,`updatetime`) values(?,?,?,?,?,?,?) on duplicate key update `id`=?,`isopen`=?,`content`=?,`updatetime`=?";

	SQLStmt<11,0> stmt(m_sql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}
	INT32 iType = (INT32)roInfo.type();
	INT32 iID = (INT32)roInfo.noticeid();
	INT32 iIsOpen = roInfo.isopen()?0:1;
	INT32 iAreaID = INT32(roInfo.areaid());
	INT32 iPlatID = INT32(roInfo.platid());
	INT32 iUpdateTime = INT32(roInfo.updatetime());
	std::string content = roInfo.content();
	stmt.BindIn(&iType);
	stmt.BindIn(&iID);
	stmt.BindIn(&iIsOpen);
	stmt.BindIn(&iAreaID);
	stmt.BindIn(&iPlatID);
	stmt.BindIn(const_cast<char*>(content.c_str()), content.size());
	stmt.BindIn(&iUpdateTime);
	stmt.BindIn(&iID);
	stmt.BindIn(&iIsOpen);
	stmt.BindIn(const_cast<char*>(content.c_str()), content.size());
	stmt.BindIn(&iUpdateTime);

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}

	return true;
}
void MysqlMgr::RunInitScript()
{
	std::string dbscript = Config::GetConfig()->ReadFileContent("dbinit.sql");
	std::vector<std::string> sqls = Split(dbscript, ';');
	for (UINT32 i = 0; i < sqls.size(); ++i)
	{
		std::string s = Trim(sqls[i]);
		if (s.empty() || s[0] == '#' || s[0] == '-' || s[0] == '/')
			continue;

		if (mysql_real_query(m_sql, s.c_str(), s.size()) != 0)
		{
			LogWarn("run sql [%s] failed!", s.c_str());
		}
	}
}

bool MysqlMgr::QueryBespeakList(std::map<std::string, std::string>& keyvalues, std::vector<UINT32>& vlist)
{
	std::string account;
	if (keyvalues.find("account") == keyvalues.end())
	{
		return false;
	}
	account = keyvalues["account"];
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}
	const char query[] = "select serverid from bespeak where openid=?";
	SQLStmt<1,1> stmt(m_sql);
	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}

	int serverid = 0;
	stmt.BindIn(const_cast<char*>(account.c_str()), account.size());
	stmt.BindOut(&serverid);

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}

	while (stmt.FetchResult())
	{
		vlist.push_back(serverid);
	}
	return true;
}

bool MysqlMgr::InsertBespeakAccount(std::map<std::string, std::string>& keyvalues, UINT32 serverid)
{
	std::string account;
	if (keyvalues.find("account") == keyvalues.end())
	{
		return false;
	}
	account = keyvalues["account"];
	if (mysql_select_db(m_sql, LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		return false;
	}

	std::ostringstream ss;
	ss << "insert into bespeak(`openid`,`serverid`) values('" << account << "'," << serverid << ") on duplicate key update `openid`='" << account << "'";
	SQLStmt<3,0> stmt(m_sql);

	if (!stmt.SetQuery(ss.str().c_str(), ss.str().size()))
	{
		stmt.ReportError();
		return false;
	}
	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}

	return true;
}
