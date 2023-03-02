#include "pch.h"
#include "dragonhelpconfig.h"
#include "teamrecord.h"
#include "unit/role.h"
#include "bagtransition.h"
#include "define/teamdef.h"

INSTANCE_SINGLETON(DragonHelpConfig)


DragonHelpConfig::DragonHelpConfig()
{

}

DragonHelpConfig::~DragonHelpConfig()
{

}

bool DragonHelpConfig::Init()
{
	return LoadFile();
}

void DragonHelpConfig::Uninit()
{

}

void DragonHelpConfig::OnRoleLogin(Role* pRole)
{
	CheckRoleHint(pRole);
}

void DragonHelpConfig::CheckRoleHint(Role* pRole)
{
	//if(NULL == pRole)
	//{
	//	return;
	//}
	//KKSG::DHRRes tmpRes;
	//FillRewardState(pRole, tmpRes);
	//bool flag = false;
	//for(int i = 0; i < tmpRes.rewstate_size(); ++i)
	//{
	//	if(tmpRes.rewstate(i).state() == KKSG::DHR_CAN_HAVEHOT)
	//	{
	//		flag = true;
	//		break;
	//	}
	//}
	//if(flag)
	//{
	//	pRole->HintNotify(SysDragonHelpReward, false);
	//}
}

void DragonHelpConfig::FillRewardState(Role* pRole, KKSG::DHRRes& roRes)
{
	for(size_t i = 0; i < m_table.Table.size(); ++i)
	{
		auto pConf = m_table.Table[i];
		int id = pConf->ID;
		KKSG::DHRState state = GetOneRewardState(pRole, pConf);

		auto& rs = *roRes.add_rewstate();
		rs.set_id(id);
		rs.set_state(state);
	}
}

KKSG::ErrorCode DragonHelpConfig::HandleFetchReward(Role* pRole, int id)
{
	auto pConf = GetConf(id);
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	if(GetOneRewardState(pRole, pConf) != KKSG::DHR_CAN_HAVEHOT)
	{
		return KKSG::ERR_FAILED;
	}

	pRole->Get<CTeamRecord>()->AddDragonHelpFetchedReward(id);

	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Team, ItemFlow_Team_DragonHelp);
	for(size_t i = 0; i < pConf->Reward.size(); ++i)
	{
		const auto& seq = pConf->Reward[i];
		give.GiveItem(UINT32(seq[0]), UINT32(seq[1]), true);	
	}
	give.NotifyClient();
	
	return KKSG::ERR_SUCCESS;
}

KKSG::DHRState DragonHelpConfig::GetOneRewardState(Role* pRole, DargonReward::RowData* pConf)
{
	if(NULL == pRole || NULL == pConf)
	{
		return KKSG::DHR_CANNOT;
	}
	
	UINT32 helpCount = pRole->Get<CTeamRecord>()->GetHelpCount(DRAGON_TEAM_SCENE);
	if(pConf->CompleteValue.empty())
	{
		return KKSG::DHR_CANNOT;
	}
	if(helpCount < UINT32(pConf->CompleteValue[0]))
	{
		return KKSG::DHR_CANNOT;
	}
	else
	{
		if(!pRole->Get<CTeamRecord>()->HaveDragonHelpFetchedReward(pConf->ID))	
		{
			return KKSG::DHR_CAN_HAVEHOT;
		}
		else
		{
			return KKSG::DHR_CAN_HAVE;
		}
	}
	return KKSG::DHR_CANNOT;
}

DargonReward::RowData* DragonHelpConfig::GetConf(int id)
{
	for(size_t i = 0; i < m_table.Table.size(); ++i)
	{
		if(m_table.Table[i]->ID == id)
		{
			return m_table.Table[i];
		}
	}
	return NULL;
}

void DragonHelpConfig::Print(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	KKSG::DHRRes tmpRes;
	FillRewardState(pRole, tmpRes);
	for(int i = 0; i < tmpRes.rewstate_size(); ++i)
	{
		SSInfo << " id = " << tmpRes.rewstate(i).id() << " state = " << tmpRes.rewstate(i).state() << END;
	}
}

bool DragonHelpConfig::LoadFile()
{
	DargonReward tmpTable;
	if(!ReadConf(tmpTable))
	{
		return false;
	}
	return ReadConf(m_table);
}

bool DragonHelpConfig::ReadConf(DargonReward& table)
{
	table.Clear();
	if(!table.LoadFile("table/DargonReward.txt"))
	{
		SSError << " load DargonReward.txt failed " << END;
		return false;
	}
	SSInfo << " load DargonReward.txt success " << END;
	return true;
}
