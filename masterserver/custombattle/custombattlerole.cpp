#include "pch.h"
#include "custombattlerole.h"
#include "network/worldlink.h"
#include "role/role.h"
#include "define/systemiddef.h"
#include "util/gametime.h"
#include "custombattle/custombattlemgr.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/ptcm2k_reportcustombattleresultworld.h"
#include "custombattle/rpcm2k_custombattlecheckvalid.h"
#include "common/ptcm2c_hintnotifyms.h"

SCustomBattleRole::SCustomBattleRole(UINT64 roleid)
{
	m_roleid = roleid;
	m_ischanged = false;
	m_battlejoin = NULL;
}

SCustomBattleRole::~SCustomBattleRole()
{
	if (NULL != m_battlejoin)
	{
		delete m_battlejoin;
		m_battlejoin = NULL;
	}
}

void SCustomBattleRole::ToKKSG(KKSG::CustombattleRoleSystem& system, KKSG::CustomBattleRole& custom)
{
	for (auto i = m_uid2battlesystem.begin(); i != m_uid2battlesystem.end(); ++i)
	{
		*system.add_system() = i->second;
	}
	if (NULL != m_battlejoin)
	{
		custom = *m_battlejoin;
	}
}

void SCustomBattleRole::ToThis(const KKSG::CustombattleRoleSystem& system, const KKSG::CustomBattleRole& custom)
{
	for (int i = 0; i < system.system_size(); ++i)
	{
		m_uid2battlesystem[system.system(i).uid()] = system.system(i);
	}
	if (0 != custom.uid())
	{
		m_battlejoin = new KKSG::CustomBattleRole;
		*m_battlejoin = custom;
	}
}

bool SCustomBattleRole::Join(UINT64 uid, bool issystem, UINT32 configid)
{
	if (!issystem)
	{
		if (NULL != m_battlejoin)
		{
			///> 最好默认成功，直接覆盖
			m_battlejoin->Clear();
			m_battlejoin->set_uid(uid);
			m_battlejoin->set_configid(configid);

			SSWarn<<"cover uid:"<<m_battlejoin->uid()<<" roleid:"<<m_roleid<<END;
			//return false;
		}
		else
		{
			m_battlejoin = new KKSG::CustomBattleRole();
			m_battlejoin->set_uid(uid);
			m_battlejoin->set_configid(configid);

			SSInfo<<"custom roleid:"<<m_roleid<<" join:"<<uid<<" configid:"<<configid<<END;
		}
		SetChanged();
	}
	else
	{
		auto i = m_uid2battlesystem.find(uid);
		if (i != m_uid2battlesystem.end())
		{
			SSWarn<<"roleid:"<<m_roleid<<" has join:"<<uid<<END;
			return false;
		}
		m_uid2battlesystem[uid].set_uid(uid);
		m_uid2battlesystem[uid].set_configid(configid);
		m_uid2battlesystem[uid].set_state(KKSG::CustomBattle_RoleState_Join);

		SSInfo<<"system roleid:"<<m_roleid<<" join:"<<uid<<" configid:"<<configid<<END;

		SetChanged();
	}
	return true;
}

bool SCustomBattleRole::UnJoin(UINT64 uid, bool issystem)
{
	if (!issystem)
	{
		if (NULL == m_battlejoin)
		{
			SSWarn<<"try unjoin uid:"<<uid<<" roleid"<<m_roleid<<END;
			return false;
		}
		if (m_battlejoin->uid() != uid)
		{
			SSWarn<<"has join:"<<m_battlejoin->uid()<<" try unjoin uid:"<<uid<<" roleid"<<m_roleid<<END;
			return false;
		}

		SSInfo<<"custom roleid:"<<m_roleid<<" unjoin:"<<uid<<END;

		delete m_battlejoin;
		m_battlejoin = NULL;
		
		SetChanged();
	}
	else
	{
		auto i = m_uid2battlesystem.find(uid);
		if (i == m_uid2battlesystem.end())
		{
			return false;
		}
		m_uid2battlesystem.erase(i);

		SSInfo<<"system roleid:"<<m_roleid<<" unjoin:"<<uid<<END;

		SetChanged();
	}
	return true;
}

bool SCustomBattleRole::IsJoin(UINT64 uid)
{
	if (NULL != m_battlejoin)
	{
		if (m_battlejoin->uid() == uid)
		{
			return true;
		}
	}
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		return true;
	}
	return false;
}

UINT64 SCustomBattleRole::GetJoinId()
{
	return NULL == m_battlejoin ? 0 : m_battlejoin->uid();
}

UINT32 SCustomBattleRole::GetJoinConfigId()
{
	return NULL == m_battlejoin ? 0 : m_battlejoin->configid();
}

bool SCustomBattleRole::IsJoinPvpTwo()
{
	UINT32 configid = GetJoinConfigId();
	bool ispvptwo = CCustomBattleConfig::Instance()->IsPvpTwo(configid);	
	return ispvptwo;
}

void SCustomBattleRole::FillSystemRole(UINT64 uid, KKSG::CustomBattleRole* datarole)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		///> 数据放在角色身上
		*datarole = i->second;
		UINT32 rewardcd = i->second.rewardcd();
		datarole->set_rewardcd(rewardcd > (UINT32)GameTime::GetTime() ? rewardcd - (UINT32)GameTime::GetTime() : 0);
	}
}

bool SCustomBattleRole::FillSystemMatch(UINT64 uid, KKSG::CustomBattleMatchInfo& info)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		info.mutable_battle()->set_battleuid(uid);
		info.mutable_battle()->set_type(KKSG::CustomBattle_PK_Normal);
		info.mutable_battle()->set_issystem(true);
		info.mutable_battle()->set_isfair(true);
        info.mutable_battle()->set_matchid(GetWin(uid));
		return true;
	}
	return false;
}

// system
void SCustomBattleRole::UpdateWinOrLose(UINT64 uid, bool iswin)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		if (KKSG::CustomBattle_RoleState_Join != i->second.state())
		{
			SSWarn<<"battle uid:"<<i->second.uid()<<" roleid:"<<m_roleid<<" state:"<<i->second.state()<<END;
			return;
		}

		if (iswin)
		{
			UINT32 win = i->second.win();
			i->second.set_win(win + 1);

			SetChanged();
		}
		else
		{
			UINT32 lose = i->second.lose();
			i->second.set_lose(lose + 1);

			SetChanged();
		}
		SSInfo<<"uid:"<<uid<<" roleid:"<<m_roleid<<" all win:"<<i->second.win()<<" all lose:"<<i->second.lose()<<" is win:"<<iswin<<END;

		EndSystem(i->second);
	}
	else
	{
		SSWarn<<"not find system uid:"<<uid<<END;
	}
}

void SCustomBattleRole::EndSystem(KKSG::CustomBattleRole& battle) 
{
	if (battle.win() >= CCustomBattleConfig::Instance()->WinLimit((UINT32)battle.uid()))
	{
		battle.set_state(KKSG::CustomBattle_RoleState_Reward);
		battle.set_rewardcd((UINT32)GameTime::GetTime() + CCustomBattleConfig::Instance()->GetSystemRewardCD((UINT32)battle.uid(), battle.win()));

		SSInfo<<"uid:"<<battle.uid()<<" roleid:"<<m_roleid<<" win end"<<END;
		SetChanged();
	}
	else if (battle.lose() >= CCustomBattleConfig::Instance()->LoseLimit((UINT32)battle.uid()))
	{
		battle.set_state(KKSG::CustomBattle_RoleState_Reward);
		battle.set_rewardcd((UINT32)GameTime::GetTime() + CCustomBattleConfig::Instance()->GetSystemRewardCD((UINT32)battle.uid(), battle.win()));

		SSInfo<<"uid:"<<battle.uid()<<" roleid:"<<m_roleid<<" lose end"<<END;
		SetChanged();
	}
}

bool SCustomBattleRole::DropSystem(UINT64 uid)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		KKSG::CustomBattleRole& battle = i->second;
		battle.set_state(KKSG::CustomBattle_RoleState_Reward);
		battle.set_rewardcd((UINT32)GameTime::GetTime() + CCustomBattleConfig::Instance()->GetSystemRewardCD((UINT32)battle.uid(), battle.win()));

		SSInfo<<"uid:"<<battle.uid()<<" roleid:"<<m_roleid<<" win end"<<END;
		SetChanged();
		return true;
	}
	return false;
}

bool SCustomBattleRole::CanSystemMatch(UINT64 uid)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		return i->second.state() == KKSG::CustomBattle_RoleState_Join;
	}
	return false;
}

// custom
void SCustomBattleRole::UpdatePoint(UINT64 uid, int point)
{

	CustomBattle* battle = CustomBattleMgr::Instance()->GetBattle(uid);
	if (NULL != battle)
	{
		battle->AddPoint(m_roleid, point);
		SSInfo<<"update point local, uid:"<<uid<<" point:"<<point<<" roleid:"<<m_roleid<<END;
	}
	else
	{
		// to world
		PtcM2K_ReportCustomBattleResultWorld report;
		report.m_Data.set_roleid(m_roleid);
		report.m_Data.set_point(point);
		report.m_Data.mutable_battle()->set_battleuid(uid);
		WorldLink::Instance()->SendTo(report);

		SSInfo<<"update point to world, uid:"<<uid<<" point:"<<point<<" roleid:"<<m_roleid<<END;
	}

	SetChanged();
}

KKSG::CustomBattleRole* SCustomBattleRole::GetSystem(UINT64 uid)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i == m_uid2battlesystem.end())
	{
		return NULL;
	}
	else
	{
		return &i->second;
	}
}

void SCustomBattleRole::ClearSystemRewardCD(UINT64 uid)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		i->second.set_rewardcd(0);

		SSInfo<<"clear system cd, uid:"<<uid<<" roleid:"<<m_roleid<<END;

		SetChanged();
	}
}

void SCustomBattleRole::ClearSystem(UINT64 uid)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		m_uid2battlesystem.erase(i);
		
		SSInfo<<"clear system, uid:"<<uid<<" roleid:"<<m_roleid<<END;

		SetChanged();
	}
}

KKSG::ErrorCode SCustomBattleRole::CanSystemReward(UINT64 uid)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		if (i->second.state() == KKSG::CustomBattle_RoleState_Reward)
		{
			if ((UINT32)GameTime::GetTime() < i->second.rewardcd())
			{
				return KKSG::ERR_CUSTOMBATTLE_REWARDCD;
			}
			else
			{
				return KKSG::ERR_SUCCESS;
			}
		}
		else
		{
			return KKSG::ERR_CUSTOMBATTLE_REWARDCANGET;
		}
	}
	return ERR_CUSTOMBATTLE_NOTJOIN;
}

bool SCustomBattleRole::IsSystemCD(UINT64 uid)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i != m_uid2battlesystem.end())
	{
		if (i->second.state() == KKSG::CustomBattle_RoleState_Reward)
		{
			if ((UINT32)GameTime::GetTime() < i->second.rewardcd())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}

void SCustomBattleRole::ClearBattle()
{
	if (NULL != m_battlejoin)
	{
		SSInfo<<"clear join uid:"<<m_battlejoin->uid()<<" roleid:"<<m_roleid<<END;
		delete m_battlejoin;
		m_battlejoin = NULL;

		SetChanged();
	}
}

UINT32 SCustomBattleRole::GetWin(UINT64 uid)
{
	auto i = m_uid2battlesystem.find(uid);
	if (i == m_uid2battlesystem.end())
	{
		SSWarn<<"not find uid:"<<uid<<" roleid:"<<m_roleid<<END;
		return 0;
	}
	else
	{
		return i->second.win();
	}
}

void SCustomBattleRole::OnLogin(CRole* role)
{
	for (auto i = m_uid2battlesystem.begin(); i != m_uid2battlesystem.end(); ++i)
	{
		if (i->second.state() == KKSG::CustomBattle_RoleState_Reward
			&& (UINT32)GameTime::GetTime() < i->second.rewardcd())
		{
			role->HintNotify(CustomBattle_System, false);
			break;
		}
	}

	UINT64 uid = GetJoinId();
	if (0 != uid)
	{
		CustomBattle* battle = CustomBattleMgr::Instance()->GetBattle(uid);
		if (NULL != battle)
		{
			if (KKSG::ERR_SUCCESS == battle->CanReward(role->GetID()))
			{
				role->HintNotify(CustomBattle_Custom, false);
			}
		}
		else
		{
			///> 本服没找到，去跨服看看
			RpcM2K_CustomBattleCheckValid* rpc = RpcM2K_CustomBattleCheckValid::CreateRpc();
			rpc->m_oArg.set_uid(uid);
			rpc->m_oArg.set_roleid(m_roleid);
			WorldLink::Instance()->SendTo(*rpc);
		}
	}
}
