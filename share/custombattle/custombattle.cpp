#include "pch.h"
#include "custombattle.h"
#include "time.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "custombattle/custombattleconfig.h"

#define CUSTOMBATTLE_RANKLIMIT (GetGlobalConfig().CustomBattleRankLimit)

CustomBattle::CustomBattle()
{
	m_ischanged = false;
}

CustomBattle::CustomBattle(const KKSG::CustomBattleConfig& config, UINT64 roleid, UINT32 serverid)
{
	m_ischanged = false;

	m_data.set_uid(NewID(serverid));
	m_data.mutable_config()->set_creator(roleid);
	m_data.mutable_config()->CopyFrom(config);
	std::string token = CCustomBattleConfig::Instance()->GenerateToken();
	m_data.mutable_config()->set_token(token);
}

CustomBattle::~CustomBattle()
{

}

UINT32 custombattle_id = 0;
UINT64 CustomBattle::NewID(UINT32 serverid)
{
	return (((UINT64)MakeUINT32(serverid, ++custombattle_id)) << 32) + (UINT64)time(NULL);
}

void CustomBattle::ToKKSGBrief(KKSG::CustomBattleData* data)
{
	*data = m_data;
	///> 计算剩余时间
	data->mutable_config()->set_battletime((UINT32)GameTime::GetTime() < m_data.config().battletime() ?  m_data.config().battletime() - (UINT32)GameTime::GetTime(): 0);
	data->mutable_config()->set_readytime((UINT32)GameTime::GetTime() < m_data.config().readytime() ? m_data.config().readytime() - (UINT32)GameTime::GetTime(): 0);
	///> 已参加的人数
	data->mutable_config()->set_hasjoincount(Size());

	// password
	data->mutable_config()->set_haspassword(false);
	if (!data->mutable_config()->password().empty())
	{
		data->mutable_config()->set_haspassword(true);
		data->mutable_config()->clear_password();
	}
	// tag
	data->mutable_config()->set_tagmask(m_data.config().tagmask());
}

void CustomBattle::ToKKSGWithRank(KKSG::CustomBattleData* data)
{
	ToKKSGBrief(data);
	int count = 0;
	for (auto i = m_ranks.begin(); i != m_ranks.end(); ++i)
	{
		if (++count > CUSTOMBATTLE_RANKLIMIT)
		{
			break;
		}
		*data->add_rank() = *i;		
	}
}

void CustomBattle::ToKKSGDB(KKSG::CustomBattleData* data)
{
	*data = m_data;
	data->clear_rank();
	for (auto i = m_ranks.begin(); i != m_ranks.end(); ++i)
	{
		*data->add_rank() = *i;		
	}
}

void CustomBattle::ToThis(const KKSG::CustomBattleData& data)
{
	m_data = data;
	for (int i = 0; i < m_data.rank_size(); ++i)
	{
		KKSG::CustomBattleRank rank = data.rank(i);
		std::pair<std::set<KKSG::CustomBattleRank, RankCompare>::iterator, bool> ret = m_ranks.insert(rank);
		if (ret.second)
		{
			m_roleid2rank[rank.roleid()] = ret.first;
		}
	}
	m_data.clear_rank();
}

bool CustomBattle::IsLocalCreate(const KKSG::CustomBattleConfig& conf)
{
	if (!GetGlobalConfig().IsOpenCustomBattleCross)
	{
		return true;
	}
	return conf.scalemask() != 0;
}

void CustomBattle::SetState(KKSG::CustomBattleState state)
{
	m_data.mutable_config()->set_state(state);
	SetChanged();
}

KKSG::ErrorCode CustomBattle::CanMatch()
{
	if (KKSG::CustomBattle_Going == GetState())
	{
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		return KKSG::ERR_CUSTOMBATTLE_MATCHFAILED;
	}
}

void CustomBattle::SetPassword(const std::string& password)
{
	m_data.mutable_config()->set_password(password); 
	m_data.mutable_config()->set_haspassword(!password.empty()); 
	SetChanged();
}
