#include "pch.h"
#include "accountbanmgr.h"
#include "util.h"
#include "timeutil.h"
#include "idip/ptci2m_banaccountlistntf.h"


INSTANCE_SINGLETON(AccountBanMsMgr)

AccountBanMsMgr::AccountBanMsMgr():m_hTimer(INVALID_HTIMER)
{
}

AccountBanMsMgr::~AccountBanMsMgr()
{
}


bool AccountBanMsMgr::Init()
{
	CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);	
	return true;
}

void AccountBanMsMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_hTimer);
	m_hTimer = INVALID_HTIMER;
}


void AccountBanMsMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 nTime = TimeUtil::GetTime();
	for (auto iter = m_BanOpenIDs.begin(); iter!=m_BanOpenIDs.end();)
	{
		KKSG::BanAccount *pAcc = &(iter->second);
		if (nTime >= pAcc->endtime())
		{
			m_BanOpenIDs.erase(iter++);
		}else
		{
			iter++;
		}
	}
}

bool AccountBanMsMgr::IsOpenIDInBanList(const std::string &openid,UINT32 &nEnd,std::string &reason)
{
	auto iter = m_BanOpenIDs.find(openid);
	if (iter!=m_BanOpenIDs.end())
	{
		if (TimeUtil::GetTime()>=iter->second.endtime())
		{
			return false;
		}
		reason = iter->second.reason();
		nEnd   = iter->second.endtime(); 
		return true;
	}
	return  false;
}



void AccountBanMsMgr::AddAccount(const KKSG::BanAccount &acc)
{
	auto iter = m_BanOpenIDs.find(acc.openid());
	if (iter!=m_BanOpenIDs.end())
	{
		iter->second = acc;
	}else
	{
		m_BanOpenIDs.insert(std::pair<std::string,KKSG::BanAccount>(acc.openid(),acc));
	}
}

void AccountBanMsMgr::DelAccount(const std::string &openid)
{
	auto iter = m_BanOpenIDs.find(openid);
	if (iter!=m_BanOpenIDs.end())
	{
		m_BanOpenIDs.erase(iter++);
	}
}

