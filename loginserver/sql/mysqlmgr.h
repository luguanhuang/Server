#ifndef __MYSQLMGR_H__
#define __MYSQLMGR_H__

#include "mysql.h"
#include "define/platnotice_def.h"

class MysqlMgr
{
	MysqlMgr();
	~MysqlMgr();
	DECLARE_SINGLETON(MysqlMgr);
public:
	bool Init();
	void Uninit();
	MYSQL* GetMysqlConn() { return m_sql; }

	// white list
	bool AddGMAccount(const std::string& account);
	bool DelGMAccount(const std::string& account);
	bool QueryGMAccount(std::set<std::string>& accounts);

	bool CleanBanAccount();
	bool DelBanAccount(const std::string& openid);
	bool QueryBanAccount(std::unordered_map<std::string, KKSG::BanAccount> &mapBanAccount);
	// notice list;
	bool QueryNoticeList(std::vector<KKSG::PlatNotice>& list);
	bool UpdateBanAccount(const KKSG::BanAccount &acc);
	bool UpdateNoticeInfo(const KKSG::PlatNotice roInfo);
	//bespeak
	bool QueryBespeakList(std::map<std::string, std::string>& keyvalues, std::vector<UINT32>& vlist);
	bool InsertBespeakAccount(std::map<std::string, std::string>& keyvalues, UINT32 serverid);

private:
	void RunInitScript();

private:
	MYSQL* m_sql;
};

#endif