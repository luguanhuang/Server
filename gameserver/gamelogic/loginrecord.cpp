#include "pch.h"
#include <time.h>
#include "loginrecord.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "reward/ptcg2c_fivedayrewardntf.h"
#include "unit/role.h"
#include "reward/ptcg2c_loginreward2cntf.h"
#include "spactivity/spactivityhandler.h"

MyLoginRecord::MyLoginRecord(Role* pRole)
{
	m_role = pRole;

	m_loginDayCount = 0;
	m_lastUpdateDay = 0;
	m_loginDayForLoginReward = 0;
	m_lroState = LROS_UNOPEN;
}

MyLoginRecord::~MyLoginRecord()
{
}

void MyLoginRecord::FirstInit(UINT32 roleNum)
{
	m_loginDayCount = 1;
	m_loginDayForLoginReward = 1;
	m_lastUpdateDay = XCommon::GetTodayUnique();
	m_IsModified.Set();
}

bool MyLoginRecord::Load( const KKSG::RoleAllInfo *poRoleAllInfo )
{
	if(poRoleAllInfo && poRoleAllInfo->has_loginrecord())
	{
		const KKSG::LoginRecord& loginRec = poRoleAllInfo->loginrecord();
		m_loginDayCount = loginRec.logindaycount();
		m_loginDayForLoginReward = loginRec.logindayforloginreward();

		int lrostate = loginRec.lrostate();
		if(LROS_UNOPEN <= lrostate && lrostate <= LROP_SHUTDOWN)
		{
			m_lroState = (LoginRewardOpenState)loginRec.lrostate();
		}
		else
		{
			SSWarn << __FUNCTION__ << " rolename = " << m_role->GetName() << " errstate = " << m_lroState << END;
			m_lroState = LROS_UNOPEN;
		}
		m_lastUpdateDay = loginRec.lastupdateday();

		for(int i = 0; i < loginRec.loginrewards_size(); ++i)
		{
			MyLoginReward myreward;
			myreward.day = loginRec.loginrewards(i).day();
			myreward.state = loginRec.loginrewards(i).state();
			for(int j = 0; j < loginRec.loginrewards(i).items_size(); ++j)
			{
				ItemDesc desc;
				desc.ToThis(loginRec.loginrewards(i).items(j));
				myreward.items.push_back(desc);
			}
			m_loginRewards.push_back(myreward);
		}
	}
	else//½¨ºÅ
	{
		UpdateLoginRewardOpenState();
	}

	//Update();
	return true;
}

void MyLoginRecord::Save( KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged )
{
	if(m_IsModified.TestAndReset())
	{
		KKSG::LoginRecord* record = poRoleAllInfo->mutable_loginrecord();
		roChanged.insert(record);
		record->Clear();

		record->set_logindaycount(m_loginDayCount);
		record->set_logindayforloginreward(m_loginDayForLoginReward);
		record->set_lrostate((int)m_lroState);
		record->set_lastupdateday(m_lastUpdateDay);

		for(size_t i = 0; i < m_loginRewards.size(); ++i)
		{
			KKSG::LoginReward *rew = record->add_loginrewards();
			rew->set_day(m_loginRewards[i].day);
			rew->set_state(m_loginRewards[i].state);
			for(size_t j = 0; j < m_loginRewards[i].items.size(); ++j)
			{
				m_loginRewards[i].items[j].ToKKSG(rew->add_items());
			}
		}
	}
}

void MyLoginRecord::Update()
{
	UINT32 curUniqueDay = XCommon::GetTodayUnique();
	if(m_lastUpdateDay != curUniqueDay)
	{
		m_lastUpdateDay = curUniqueDay;

		++m_loginDayCount;
		if(LROS_OPEN == m_lroState)
		{
			++m_loginDayForLoginReward;
			for(size_t i = 0; i < m_loginRewards.size(); ++i)
			{
				if(m_loginRewards[i].day <= m_loginDayForLoginReward
					&& m_loginRewards[i].state == KKSG::LOGINRS_CANNOT)
				{
					m_loginRewards[i].state = KKSG::LOGINRS_HAVEHOT;
					m_IsModified.Set();
				}
			}
			LoginRewardNtf();
		}
		SpActivityHandler::GlobalHandler.OnLoginOneDay(m_role);

		m_IsModified.Set();
	}
}

void MyLoginRecord::FillMyLoginRewards( KKSG::LoginRewardRet& ret)
{
	if(LROS_OPEN != m_lroState)
	{
		ret.set_open(false);
		return ;
	}
	ret.set_open(true);
	UINT32 sectoday = time(NULL) - XCommon::GetTodayBeginTime();
	ret.set_sectoday(sectoday);
	ret.set_logindayforreward(m_loginDayForLoginReward);
	for(size_t i = 0; i < m_loginRewards.size(); ++i)
	{
		KKSG::LoginReward *lr = ret.add_rewards();
		lr->set_day(m_loginRewards[i].day);
		lr->set_state(m_loginRewards[i].state);
		for(size_t j = 0; j < m_loginRewards[i].items.size(); ++j)
		{
			m_loginRewards[i].items[j].ToKKSG(lr->add_items());
		}
	}
}

void MyLoginRecord::SetLoginRewardState( int day, KKSG::LoginRewardState state )
{
	Update();
	for(size_t i = 0; i < m_loginRewards.size(); ++i)
	{
		if(day == m_loginRewards[i].day)
		{
			m_loginRewards[i].state = state;
		}
	}
	LoginRewardNtf();

	m_IsModified.Set();
}

void MyLoginRecord::GetLoginRewardItem( int day, std::vector<ItemDesc>& items )
{
	Update();
	items.clear();
	for(size_t i = 0; i < m_loginRewards.size(); ++i)
	{
		if(day == m_loginRewards[i].day)
		{
			items = m_loginRewards[i].items;
		}
	}
}

KKSG::LoginRewardState MyLoginRecord::GetLoginRewardState( int day )
{
	Update();
	for(size_t i = 0; i < m_loginRewards.size(); ++i)
	{
		if(day == m_loginRewards[i].day)
		{
			return m_loginRewards[i].state;
		}
	}
	return KKSG::LOGINRS_CANNOT;
}

void MyLoginRecord::TestPrint()
{
	Update();
	SSDebug << __FUNCTION__ << END;
	SSDebug << "dayCount = " << m_loginDayCount << END;
	SSDebug << " login reward day = " << m_loginDayForLoginReward << END;
	SSDebug << " login reward open state = " << (int)(m_lroState) << END;
	SSDebug << " lastupdate day = " << m_lastUpdateDay << END;
	for(size_t i = 0; i < m_loginRewards.size(); ++i)
	{
		SSDebug << "day = " << m_loginRewards[i].day << " : " << 
			"state = " << m_loginRewards[i].state << END;
		for(size_t j = 0; j < m_loginRewards[i].items.size(); ++j)
		{
			ItemDesc desc = m_loginRewards[i].items[j];
			SSDebug << "item = " << desc.itemID << " " << desc.itemCount << " " << desc.isbind << END;
		}
	}
}

UINT32 MyLoginRecord::GetRealLoginDayCount()
{
	Update();
	return m_loginDayCount;
}

void MyLoginRecord::UpdateLoginRewardOpenState()
{
	if(LROP_SHUTDOWN == m_lroState)
	{
		return;
	}
	if(LROS_UNOPEN == m_lroState)	
	{
		if(m_role->GetLevel() >=  GetGlobalConfig().LoginRewardOpenLevel)
		{
			OnLoginRewardOpened();
		}
	}
	else if(LROS_OPEN == m_lroState)
	{
		bool shutdown = true;
		for(size_t i = 0; i < m_loginRewards.size(); ++i)
		{
			if(KKSG::LOGINRS_HAVE != m_loginRewards[i].state)
			{
				shutdown = false;
			}
		}
		if(shutdown)
		{
			OnLoginRewardShutDown();
		}
	}
}

void MyLoginRecord::TestClear()
{
	m_loginDayCount = 0;
	m_loginDayForLoginReward = 0;
	m_lroState = LROS_UNOPEN;
	m_lastUpdateDay = 0;
	m_loginRewards.clear();

	UpdateLoginRewardOpenState();
	Update();

	TestPrint();
}

void MyLoginRecord::OnLoginRewardOpened()
{
	if( GetGlobalConfig().LoginRewardOpenLevel > 1)
	{
		m_loginDayForLoginReward = 1;
	}
	m_lroState = LROS_OPEN;

	SetLoginRewardConf();
	LoginRewardNtf();

	m_IsModified.Set();
}

void MyLoginRecord::OnDayZero()
{
	Update();
}

void MyLoginRecord::OnEnterScene()
{
	SetLoginRewardConf();
	LoginRewardNtf();
	m_IsModified.Set();
}

void MyLoginRecord::LoginRewardNtf()
{
	PtcG2C_LoginReward2CNtf ntf;
	FillMyLoginRewards(ntf.m_Data);
	m_role->Send(ntf);
}

void MyLoginRecord::OnLoginRewardShutDown()
{
	m_lroState = LROP_SHUTDOWN;
	m_loginRewards.clear();

	LoginRewardNtf();

	m_IsModified.Set();
}

void MyLoginRecord::TestAddLoginDay()
{
	Update();
	if(m_lastUpdateDay > 0)
	{
		--m_lastUpdateDay;
	}
	Update();
	TestPrint();
}

void MyLoginRecord::SetLoginRewardConf()
{
	if(LROS_OPEN != m_lroState)
	{
		return ;
	}
	for(size_t i = 0; i < m_loginRewards.size(); ++i)
	{
		if(KKSG::LOGINRS_HAVE != m_loginRewards[i].state)
		{
			m_loginRewards[i].items.clear();
		}
	}
	for(size_t i = 0; i <  GetGlobalConfig().LoginRewards.size(); ++i)
	{
		Sequence<UINT32,3>& seq =  GetGlobalConfig().LoginRewards[i];	
		int day = seq[0];
		if(day <= 0 )
		{
			continue;
		}
		if((day - 1)  >= (int)m_loginRewards.size())
		{
			m_loginRewards.resize(day);
		}

		MyLoginReward& mylr = m_loginRewards[day-1];
		if(KKSG::LOGINRS_HAVE == mylr.state)
		{
			continue;
		}
		mylr.day = day;
		mylr.state = KKSG::LOGINRS_CANNOT;
		if(m_loginDayForLoginReward >= mylr.day)
		{
			mylr.state = KKSG::LOGINRS_HAVEHOT;
		}
		ItemDesc desc(seq[1], seq[2], true);
		mylr.items.push_back(desc);
	}

}

