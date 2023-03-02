#include "pch.h"
#include "accountbanmgr.h"
#include "util.h"
#include "sql/mysqlmgr.h"
#include "timeutil.h"
#include "network/mslink.h"
#include "idip/ptci2m_banaccountlistntf.h"


INSTANCE_SINGLETON(AccountBanMgr)

AccountBanMgr::AccountBanMgr():m_hTimer(INVALID_HTIMER)
{
}

AccountBanMgr::~AccountBanMgr()
{
}


bool AccountBanMgr::Init()
{
	MysqlMgr::Instance()->CleanBanAccount();
	if(MysqlMgr::Instance()->QueryBanAccount(m_BanOpenIDs)==false)
	{
		return false;
	}
	m_hTimer = CTimerMgr::Instance()->SetTimer(this, 0, 60000, -1, __FILE__, __LINE__);	
	return true;
}

void AccountBanMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_hTimer);
	m_hTimer = INVALID_HTIMER;
}


void AccountBanMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	//10min清理一下过时数据。
	UINT32 nTime = TimeUtil::GetTime();
	for (auto iter = m_BanOpenIDs.begin(); iter!=m_BanOpenIDs.end();)
	{
		KKSG::BanAccount *pAcc = &(iter->second);
		if (nTime >= pAcc->endtime())
		{
			MysqlMgr::Instance()->DelBanAccount(pAcc->openid());
			m_BanOpenIDs.erase(iter++);
		}else
		{
			iter++;
		}
	}
}

bool AccountBanMgr::IsOpenIDInBanList(const std::string &openid)
{
	auto iter = m_BanOpenIDs.find(openid);
	if (iter!=m_BanOpenIDs.end())
	{
		if (TimeUtil::GetTime()>=iter->second.endtime())
		{
			return false;
		}
		return true;
	}
	return  false;
}


void AccountBanMgr::FillReason(const std::string &openid,KKSG::PlatBanAccount *pAcc)
{
	auto iter = m_BanOpenIDs.find(openid);
	if (iter!=m_BanOpenIDs.end())
	{
		pAcc->set_reason(iter->second.reason());
		pAcc->set_endtime(iter->second.endtime());
	}
}


void AccountBanMgr::AddAccount(KKSG::BanAccount &acc)
{
	auto iter = m_BanOpenIDs.find(acc.openid());
	if (iter!=m_BanOpenIDs.end())
	{
		iter->second = acc;
	}else
	{
		m_BanOpenIDs.insert(std::pair<std::string,KKSG::BanAccount>(acc.openid(),acc));
	}
	MysqlMgr::Instance()->UpdateBanAccount(acc);
}

void AccountBanMgr::DelAccount(const std::string &openid)
{
	auto iter = m_BanOpenIDs.find(openid);
	if (iter!=m_BanOpenIDs.end())
	{
		MysqlMgr::Instance()->DelBanAccount(openid);
		m_BanOpenIDs.erase(iter++);
	}
}

void AccountBanMgr::SyncBanList(UINT32 nLink)
{
	if (m_BanOpenIDs.size()==0)
		return;
	auto iter = m_BanOpenIDs.begin();
	UINT32  nMax  = 100;//
	UINT32 nTotal = 0;
	PtcI2M_BanAccountListNtf ntf;
	while(iter!=m_BanOpenIDs.end())
	{
		KKSG::BanAccount *pAcc = ntf.m_Data.add_acclist();
		pAcc->set_openid(iter->second.openid());
		pAcc->set_endtime(iter->second.endtime());
		pAcc->set_reason(iter->second.reason());
		nTotal++;
		if (nTotal >= nMax)
		{
			MsLink::Instance()->Send(nLink,ntf);
			ntf.m_Data.Clear();
			nTotal = 0;
		}
		iter++;
	}
	if (nTotal>0)
	{
		MsLink::Instance()->Send(nLink,ntf);
	}
}