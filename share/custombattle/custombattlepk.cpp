#include "pch.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattleconfig.h"

//////////////////////////////////////////////////////////////////////////
CustomBattlePk::CustomBattlePk()
{

}

CustomBattlePk::CustomBattlePk(const KKSG::CustomBattleConfig& config, UINT64 roleid, UINT32 serverid)
:CustomBattle(config, roleid, serverid)
{
	SetState(KKSG::CustomBattle_Ready);
}

CustomBattlePk::~CustomBattlePk()
{

}

bool CustomBattlePk::StartNow()
{
	if (KKSG::CustomBattle_Ready == m_data.config().state())
	{
		int battletime = m_data.config().battletime() - m_data.config().readytime();
		if (battletime > 0)
		{
			m_data.mutable_config()->set_battletime((UINT32)GameTime::GetTime() + battletime);
		}
		m_data.mutable_config()->set_state(KKSG::CustomBattle_Going);
		SetChanged();
		return true;
	}
	return false;
}

void CustomBattlePk::Update()
{
	switch (m_data.config().state())
	{
	case KKSG::CustomBattle_Ready:
		{
			if (GameTime::GetTime() > m_data.config().readytime())
			{
				SetChanged();
				m_data.mutable_config()->set_state(KKSG::CustomBattle_Going);
				SSInfo<<"uid:"<<m_data.uid()<<" set state:"<<KKSG::CustomBattle_Going<<END;
			}
			break;
		}
	case KKSG::CustomBattle_Going:
		{
			if (GameTime::GetTime() > m_data.config().battletime())
			{
				SetChanged();
				///> 按积分排行结算发奖
				std::set<KKSG::CustomBattleRank, RankCompare> tempranks;
				UINT32 count = 0;
				for (auto i = m_ranks.begin(); i != m_ranks.end(); ++i)
				{
					KKSG::CustomBattleRank temprank = *i;
					UINT32 cd = (UINT32)GameTime::GetTime() + CCustomBattleConfig::Instance()->GetCustomRewardCD(GetConfigId(), count + 1);
					temprank.set_rewardcd(cd);
					tempranks.insert(temprank);
					++count;
				}

				m_ranks.clear();
				m_roleid2rank.clear();
				for (auto j = tempranks.begin(); j != tempranks.end(); ++j)
				{
					AddInfo(*j);
				}

				m_data.mutable_config()->set_state(KKSG::CustomBattle_End);
				SSInfo<<"uid:"<<m_data.uid()<<" set state:"<<KKSG::CustomBattle_End<<END;
			}
			break;
		}
	case KKSG::CustomBattle_End:
		{
			if (GameTime::GetTime() > (m_data.config().battletime() + GetGlobalConfig().CustomBattleExpireTime))
			{
				SetChanged();
				///> 过期从数据库中删除
				m_data.mutable_config()->set_state(KKSG::CustomBattle_Destory);
				SSInfo<<"uid:"<<m_data.uid()<<" set state:"<<KKSG::CustomBattle_Destory<<END;
			}
			break;
		}
	case KKSG::CustomBattle_Destory:
		{
			break;
		}
	}
}
KKSG::ErrorCode CustomBattlePk::CanJoin(const std::string& password)
{
	// join period
	if (KKSG::CustomBattle_End <= GetState())
	{
		return KKSG::ERR_CUSTOMBATTLE_JOINNOTINREADYTIME;
	}
	// join count limit
	if (IsFull())
	{
		return KKSG::ERR_CUSTOMBATTLE_JOINFULL;
	}
	// check password
	if (password != GetPassword())
	{
		return KKSG::ERR_CUSTOMBATTLE_JOININVALIDPASSWORD;
	}
	return KKSG::ERR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CustomBattlePk::IsIn(UINT64 roleid)
{
	return m_roleid2rank.end() != m_roleid2rank.find(roleid);
}

KKSG::ErrorCode CustomBattlePk::CanReward(UINT64 roleid)
{
	if (KKSG::CustomBattle_End != GetState())
	{
		return KKSG::ERR_FAILED;
	}
	auto i = m_roleid2rank.find(roleid);
	if (i == m_roleid2rank.end())
	{
		return KKSG::ERR_CUSTOMBATTLE_NOTJOIN;
	}
	else
	{
		if (i->second->istakenreward())
		{
			return KKSG::ERR_CUSTOMBATTLE_REWARDHASGET;
		}
		if (GameTime::GetTime() < i->second->rewardcd())
		{
			return KKSG::ERR_CUSTOMBATTLE_REWARDCD;
		}
		return KKSG::ERR_SUCCESS;
	}
}

bool CustomBattlePk::IsCD(UINT64 roleid)
{
	if (KKSG::CustomBattle_End != GetState())
	{
		return false;
	}
	auto i = m_roleid2rank.find(roleid);
	if (i == m_roleid2rank.end())
	{
		return false;
	}
	else
	{
		if (i->second->istakenreward())
		{
			return false;
		}
		if (GameTime::GetTime() < i->second->rewardcd())
		{
			return true;
		}
		return false;
	}
}

UINT32 CustomBattlePk::GetRank(UINT64 roleid)
{
	UINT32 count = 0;
	for (auto i = m_ranks.begin(); i != m_ranks.end(); ++i)
	{
		++count;
		if ((*i).roleid() == roleid)
		{
			return count;
		}
	}
	return (UINT32)(-1);
}

void CustomBattlePk::DoReward(UINT64 roleid)
{
	auto i = m_roleid2rank.find(roleid);
	if (i == m_roleid2rank.end())
	{
		SSWarn<<"roleid:"<<roleid<<" is not find, uid:"<<m_data.uid()<<END;
	}
	else
	{
		KKSG::CustomBattleRank info = *i->second;
		info.set_istakenreward(true);
		
		m_ranks.erase(i->second);
		m_roleid2rank.erase(i);
		AddInfo(info);

		SetChanged();
	}
}

void CustomBattlePk::ClearCD(UINT64 roleid)
{
	auto i = m_roleid2rank.find(roleid);
	if (i == m_roleid2rank.end())
	{
		SSWarn<<"roleid:"<<roleid<<" is not find, uid:"<<m_data.uid()<<END;
	}
	else
	{
		KKSG::CustomBattleRank info = *i->second;
		info.set_rewardcd(0);
		
		m_ranks.erase(i->second);
		m_roleid2rank.erase(i);
		AddInfo(info);

		SetChanged();
	}
}

void CustomBattlePk::AddPoint(UINT64 roleid, int point)
{
	auto i = m_roleid2rank.find(roleid);
	if (i != m_roleid2rank.end())
	{
		int result = ((int)i->second->point() < -point) ? 0 : (i->second->point() + point); 

		KKSG::CustomBattleRank info = *i->second;
		info.set_point(result);
		info.set_timestamp((UINT32)GameTime::GetTime());
		
		m_ranks.erase(i->second);
		m_roleid2rank.erase(i);
		AddInfo(info);

		SetChanged();

		SSInfo<<"uid:"<<m_data.uid()<<" add point:"<<point<<" result:"<<result<<" roleid:"<<roleid<<END;
	}
	else
	{
		SSWarn<<"uid:"<<m_data.uid()<<" not find roleid:"<<roleid<<" point:"<<point<<END;
	}
}


void CustomBattlePk::AddRole(UINT64 roleid, const std::string& name)
{
	auto i = m_roleid2rank.find(roleid);
	if (i != m_roleid2rank.end())
	{
		SSWarn<<"uid:"<<m_data.uid()<<" add roleid:"<<roleid<<" failed"<<END;
	}
	else
	{
		KKSG::CustomBattleRank rank;
		rank.set_roleid(roleid);
		rank.set_name(name);
		rank.set_point(1500);
		rank.set_timestamp((UINT32)GameTime::GetTime());
		AddInfo(rank);

		SetChanged();
		SSInfo<<"uid:"<<m_data.uid()<<" add roleid:"<<roleid<<END;
	}
}

void CustomBattlePk::DelRole(UINT64 roleid)
{
	///> 比赛已经结束了，保留记录不删
	if (m_data.config().state() >= KKSG::CustomBattle_End)
	{
		return;
	}
	auto i = m_roleid2rank.find(roleid);
	if (i != m_roleid2rank.end())
	{
		m_ranks.erase(i->second);
		m_roleid2rank.erase(i);

		SetChanged();
		SSInfo<<"uid:"<<m_data.uid()<<" del roleid:"<<roleid<<END;
	}
	else
	{
		SSWarn<<"uid:"<<m_data.uid()<<" del roleid:"<<roleid<<" failed"<<END;
	}
}

void CustomBattlePk::AddInfo(const KKSG::CustomBattleRank& info)
{
	std::pair<std::set<KKSG::CustomBattleRank, RankCompare>::iterator, bool> ret = m_ranks.insert(info);
	if (ret.second)
	{
		m_roleid2rank[info.roleid()] = ret.first;
	}
}

UINT32 CustomBattlePk::GetPoint(UINT64 roleid)
{
	auto i = m_roleid2rank.find(roleid);
	if (i == m_roleid2rank.end())
	{
		return (UINT32)(-1);
	}
	else
	{
		return i->second->point();
	}
}

bool CustomBattlePk::FillMatch(UINT64 uid, UINT64 roleid, KKSG::CustomBattleMatchInfo& info)
{
	info.mutable_battle()->set_battleuid(uid);
	info.mutable_battle()->set_issystem(false);
	info.mutable_battle()->set_type(m_data.config().fighttype());
	info.mutable_battle()->set_isfair(m_data.config().isfair());

	auto i = m_roleid2rank.find(roleid);
	if (i != m_roleid2rank.end())
	{
		info.mutable_role()->set_point(i->second->point());
		CCustomBattleConfig::Instance()->FillPkStage(i->second->point(), *info.mutable_role());
	}

	return true;
}

bool CustomBattlePk::FillRole(UINT64 uid, UINT64 roleid, KKSG::CustomBattleRole& datarole)
{
	if (GetID() != uid)
	{
		return false;
	}

	///> 数据在比赛中，从比赛里拿
	datarole.set_uid(GetID());
	datarole.set_configid(GetConfigId());
	UINT32 count = 0;
	for (auto i = m_ranks.begin(); i != m_ranks.end(); ++i)
	{
		++count;
		const KKSG::CustomBattleRank& rank = *i;
		if (roleid != rank.roleid())
		{
			continue;
		}

		datarole.set_point(rank.point());
		datarole.set_rank(count);
		switch (GetState())
		{
		case KKSG::CustomBattle_Ready:
			{
				datarole.set_state(KKSG::CustomBattle_RoleState_Ready);
				break;
			}
		case KKSG::CustomBattle_Going:
			{
				datarole.set_state(KKSG::CustomBattle_RoleState_Join);
				break;
			}
		case KKSG::CustomBattle_End:
			{
				// reward cd
				UINT32 rewardcd = rank.rewardcd();
				datarole.set_rewardcd(rewardcd > (UINT32)GameTime::GetTime() ? rewardcd - (UINT32)GameTime::GetTime() : 0);
				if (rank.istakenreward())
				{
					datarole.set_state(KKSG::Custombattle_RoleState_Taken);
				}
				else
				{
					datarole.set_state(KKSG::CustomBattle_RoleState_Reward);
				}
				break;
			}
		case KKSG::CustomBattle_Destory:
			{
				datarole.set_state(KKSG::Custombattle_RoleState_Taken);
				break;
			}
		default:
			break;
		}
	}
	return true;
}
