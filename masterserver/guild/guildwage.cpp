
#include "pch.h"
#include "guildwage.h"
#include "guild.h"
#include "guildactivity.h"
#include "table/guildConfig.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/tlogenum.h"
#include "guild/ptcm2c_noticeguildwagereward.h"
#include <time.h>
#include "loghelper/tlogr.h"

#include "foreach.h"
#include "common/rpcm2g_msgiveitem.h"



CGuildWage::CGuildWage()
{
	m_guild = NULL;
	m_wageLvl = WAGE_MAX;
	m_lastScore = 0;
	m_lastGuildLvl = 0;
	//m_lastExp = 0;
}

CGuildWage::CGuildWage(Guild* pGuild)
{
	m_guild = pGuild;
	m_wageLvl = WAGE_MAX;
	m_lastScore = 0;
	m_lastGuildLvl = 0;
	//m_lastExp = 0;
}

CGuildWage::~CGuildWage()
{
	m_guild = NULL;
}


bool CGuildWage::IsChanged()
{
	return m_switch.TestAndReset();
}

bool CGuildWage::Load(const KKSG::GuildAllDBData & guilddata)
{
	KKSG::GuildWageSave data;
	if (!guilddata.has_guildwage())
	{
		return true;
	}

	if (guilddata.guildwage().empty())
	{
		return true;
	}

	if (!data.ParseFromString(guilddata.guildwage()))
	{
		return true;
	}

	m_base.m_exp = data.exp();
	m_base.m_rolenum = data.rolenum();
	m_base.m_prestige = data.prestige();
	m_base.m_activity = data.activity();
	m_lastGuildLvl = data.guildlvl();
	m_lastScore = data.lastscore();
	m_wageLvl = (WageLvl)data.wagelvl();

	for (UINT32 cnt = 0; cnt < data.wageunit_size(); ++cnt)
	{
		WageState& wageData = m_rewardRoleMap[data.wageunit(cnt).roleid()];
		wageData.m_position = data.wageunit(cnt).position();
		wageData.m_reward = data.wageunit(cnt).wagestate();
	}
	return true;
}

void CGuildWage::Save(KKSG::GuildAllDBData& guilddata)
{
	guilddata.mutable_guildwage()->assign(ToBinary());
}

const std::string CGuildWage::ToBinary()
{
	KKSG::GuildWageSave save;
	save.set_exp(m_base.m_exp);
	save.set_rolenum(m_base.m_rolenum);
	save.set_prestige(m_base.m_prestige);
	save.set_activity(m_base.m_activity);
	save.set_guildlvl(m_lastGuildLvl);
	save.set_wagelvl(m_wageLvl);
	save.set_lastscore(m_lastScore);

	foreach (i in m_rewardRoleMap)
	{
		KKSG::GuildWageUnit* pUnit = save.add_wageunit();
		pUnit->set_roleid(i->first);
		pUnit->set_position((UINT32)i->second.m_position);
		pUnit->set_wagestate((KKSG::WageRewardState)i->second.m_reward);
	}

	return save.SerializeAsString();
}

void CGuildWage::DoCheckWork()
{
	m_lastGuildLvl = m_guild->GetLevel();
	m_wageLvl = (WageLvl)_CalcWageLvl();
	m_base.m_exp = (UINT32)m_guild->GetAllExp();
	m_base.m_rolenum = m_guild->GetGuildMember().size();
	m_base.m_prestige = m_guild->GetPrestige();
	m_base.m_activity = m_guild->Get<CGuildActivity>()->GetActivity();
	//m_lastExp = (UINT32)m_guild->GetExp(); 

	UINT32 nowTime = time(NULL);
	m_rewardRoleMap.clear();
	const std::map<UINT64, GuildMember>& memberVec = m_guild->GetGuildMember();
	for (auto iter = memberVec.begin(); iter != memberVec.end(); ++iter)
	{
		WageState& state = m_rewardRoleMap[iter->first];
		state.m_position = iter->second.position;
		if ((nowTime - m_guild->GetJoinTime(iter->first)) >= GlobalConfig::GetInstance().GuildWageKeepDay * 24 * 60 * 60)
		{
			state.m_reward = 3;
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(iter->first);
			if (pRole != NULL)
			{
				PtcM2C_NoticeGuildWageReward msg;
				pRole->Send(msg);
			}
		}
		else
		{
			state.m_reward = 1;
		}
	}
	m_switch.Set();

	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	m_guild->DoTxLog(oLog);
	oLog.m_iWageLevel = m_wageLvl;
	oLog.m_iActType = TX_GUILD_ACT_TYPE_6;
	oLog.Do();
}

UINT32 CGuildWage::_CalcWageLvl()
{
	const GuildSalaryTable::RowData* pData = CGuildConfig::Instance()->GetGuildSalaryInfo(m_lastGuildLvl);
	if (pData == NULL || pData->NumberTransformation.size() < 2 || pData ->PrestigeTransformation.size() < 2 
		|| pData->ActiveTransformation.size() < 2 || pData->EXPTransformation.size() < 2)
	{
		return D;
	}

	UINT32 exp = m_guild->GetAllExp() > m_base.m_exp ? (m_guild->GetAllExp() - m_base.m_exp) : 0; 
	UINT32 expScore = exp >= pData->EXPTransformation[0].seq[1] ? 
		pData->EXPTransformation[1].seq[1] : ((float)pData->EXPTransformation[1].seq[1] * ((float)exp / (float)pData->EXPTransformation[0].seq[1]));

	UINT32 numScore = m_guild->GetGuildMember().size() >= pData->NumberTransformation[0].seq[1] ? 
		pData->NumberTransformation[1].seq[1] : ((float)pData->NumberTransformation[1].seq[1] * ((float)m_guild->GetGuildMember().size() / (float)pData->NumberTransformation[0].seq[1]));

	UINT32 prestige = m_guild->GetPrestige() > m_base.m_prestige ? (m_guild->GetPrestige() - m_base.m_prestige) : 0;
	UINT32 prestigeScore = prestige >= pData->PrestigeTransformation[0].seq[1] ? 
		pData->PrestigeTransformation[1].seq[1] : ((float)pData->PrestigeTransformation[1].seq[1] * ((float)prestige / (float)pData->PrestigeTransformation[0].seq[1]));

	UINT32 activityScore = m_guild->Get<CGuildActivity>()->GetActivity() >= pData->ActiveTransformation[0].seq[1] ? 
		pData->ActiveTransformation[1].seq[1] : ((float)pData->ActiveTransformation[1].seq[1] * ((float)m_guild->Get<CGuildActivity>()->GetActivity() / (float)pData->ActiveTransformation[0].seq[1]));

	UINT32 score = (expScore + numScore + prestigeScore + activityScore) / 4;
	m_lastScore = score;

	if (score < pData->GuildReview[0])
	{
		return D;
	}
	else if (score < pData->GuildReview[1])
	{
		return C;
	} 
	else if (score < pData->GuildReview[2])
	{
		return B;
	}
	else if (score < pData->GuildReview[3])
	{
		return A;
	}
	else
	{
		return S;
	}
}

bool CGuildWage::GetReward(CRole* pRole)
{
	if (pRole == NULL)
	{
		return false;
	}

	std::vector<ItemBrief> itemVec;
	_GetReward(pRole->GetID(), itemVec);
	if (itemVec.empty())
	{
		return false;
	}

	RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
	rpc->m_oArg.set_roleid(pRole->GetID());
	rpc->m_oArg.set_reason(ItemFlow_Guild);
	rpc->m_oArg.set_subreason(ItemFlow_Guild_Wage);
	foreach (i in itemVec)
	{
		KKSG::ItemBrief* pItem = rpc->m_oArg.add_items();
		pItem->set_itemid(i->itemid());
		pItem->set_itemcount(i->itemcount());
	}
	pRole->SendMsgToGS(*rpc);
	return true;
}

void CGuildWage::_GetReward(UINT64 roleId, std::vector<ItemBrief>& itemVec)
{
	auto iter = m_rewardRoleMap.find(roleId);
	if (iter == m_rewardRoleMap.end())
	{
		return ;
	}

	if (iter->second.m_reward != 3)
	{
		return ;
	}
	else
	{
		iter->second.m_reward = 2;
		m_switch.Set();
	}

	const GuildSalaryTable::RowData* pData = CGuildConfig::Instance()->GetGuildSalaryInfo(m_lastGuildLvl);
	if (pData == NULL)
	{
		return ;
	}

	UINT32 position = iter->second.m_position; 
	if (position == GPOS_MAX)
	{
		return ;
	}

	vector<Sequence<uint, 2>> rewardVec;
	switch (m_wageLvl)
	{
	case (S):
		{
			if (position == GPOS_LEADER)
			{
				rewardVec = pData->SSalary1; 
			}
			else if (position == GPOS_VICELEADER)
			{
				rewardVec = pData->SSalary2;
			}
			else if (position == GPOS_OFFICER)
			{
				rewardVec = pData->SSalary3;
			}
			else if (position == GPOS_ELITEMEMBER)
			{
				rewardVec = pData->SSalary4;
			}
			else if (position == GPOS_MEMBER)
			{
				rewardVec = pData->SSalary5;
			}
		}
		break;
	case (A):
		{
			if (position == GPOS_LEADER)
			{
				rewardVec = pData->ASalary1; 
			}
			else if (position == GPOS_VICELEADER)
			{
				rewardVec = pData->ASalary2;
			}
			else if (position == GPOS_OFFICER)
			{
				rewardVec = pData->ASalary3;
			}
			else if (position == GPOS_ELITEMEMBER)
			{
				rewardVec = pData->ASalary4;
			}
			else if (position == GPOS_MEMBER)
			{
				rewardVec = pData->ASalary5;
			}

		}
		break;
	case (B):
		{
			if (position == GPOS_LEADER)
			{
				rewardVec = pData->BSalary1; 
			}
			else if (position == GPOS_VICELEADER)
			{
				rewardVec = pData->BSalary2;
			}
			else if (position == GPOS_OFFICER)
			{
				rewardVec = pData->BSalary3;
			}
			else if (position == GPOS_ELITEMEMBER)
			{
				rewardVec = pData->BSalary4;
			}
			else if (position == GPOS_MEMBER)
			{
				rewardVec = pData->BSalary5;
			}

		}
		break;
	case (C):
		{
			if (position == GPOS_LEADER)
			{
				rewardVec = pData->CSalary1; 
			}
			else if (position == GPOS_VICELEADER)
			{
				rewardVec = pData->CSalary2;
			}
			else if (position == GPOS_OFFICER)
			{
				rewardVec = pData->CSalary3;
			}
			else if (position == GPOS_ELITEMEMBER)
			{
				rewardVec = pData->CSalary4;
			}
			else if (position == GPOS_MEMBER)
			{
				rewardVec = pData->CSalary5;
			}

		}
		break;
	case (D):
		{
			if (position == GPOS_LEADER)
			{
				rewardVec = pData->DSalary1; 
			}
			else if (position == GPOS_VICELEADER)
			{
				rewardVec = pData->DSalary2;
			}
			else if (position == GPOS_OFFICER)
			{
				rewardVec = pData->DSalary3;
			}
			else if (position == GPOS_ELITEMEMBER)
			{
				rewardVec = pData->DSalary4;
			}
			else if (position == GPOS_MEMBER)
			{
				rewardVec = pData->DSalary5;
			}

		}
		break;
	case (0):
		{
			if (position == GPOS_LEADER)
			{
				rewardVec = pData->DSalary1; 
			}
			else if (position == GPOS_VICELEADER)
			{
				rewardVec = pData->DSalary2;
			}
			else if (position == GPOS_OFFICER)
			{
				rewardVec = pData->DSalary3;
			}
			else if (position == GPOS_ELITEMEMBER)
			{
				rewardVec = pData->DSalary4;
			}
			else if (position == GPOS_MEMBER)
			{
				rewardVec = pData->DSalary5;
			}

		}
		break;
	default:
		break;
	}

	for (UINT32 cnt = 0; cnt < rewardVec.size(); ++cnt)
	{
		ItemBrief item;
		item.set_itemid(rewardVec[cnt].seq[0]);
		item.set_itemcount(rewardVec[cnt].seq[1]);
		itemVec.push_back(item);
	}
}

UINT32 CGuildWage::IsReward(UINT64 roleId)
{
	auto iter = m_rewardRoleMap.find(roleId); 
	if (iter != m_rewardRoleMap.end())
	{
		return iter->second.m_reward;
	}
	else
	{
		return 1;
	}
}

UINT32 CGuildWage::GetPosition(UINT64 roleId)
{
	auto iter = m_rewardRoleMap.find(roleId); 
	if (iter != m_rewardRoleMap.end())
	{
		return iter->second.m_position;
	}
	else
	{
		return GPOS_MAX;
	}
}

void CGuildWage::OnlineCheck(CRole* pRole)
{
	if (pRole == NULL)
	{
		return;
	}

	if (IsReward(pRole->GetID()) == 3)
	{
		PtcM2C_NoticeGuildWageReward msg;
		pRole->Send(msg);
	}
}

void CGuildWage::GMSetWageLvl(UINT32 lvl)
{
	if (lvl >= 1 && lvl <=5)
	{
		m_wageLvl = (WageLvl)lvl;
		m_switch.Set();
	}
}