#include "pch.h"
#include "scenecastle.h"
#include "scene.h"
#include "guild/guildmgr.h"
#include "guildcastle/ptcg2m_gcfsyng2m.h"
#include "mslink.h"
#include "guildcastle/ptcg2c_gcfsyng2cntf.h"
#include "gamelogic/rolesummarymgr.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "guilfcastle/ptcg2c_gcfzhanlingnotify.h"
#include "pb/enum.pb.h"
#include "gamelogic/reportdatarecord.h"

SceneCastle::SceneCastle(Scene* scene):SceneVsBase(scene)
{
	m_isFightEnd = false;
}

SceneCastle::~SceneCastle()
{

}

bool SceneCastle::Init(const KKSG::CreateBattleParam& roParam)
{
	m_data = roParam.guildcastle();

	SSInfo << " sceneid = " << GetScene()->GetSceneID() << " fightid = " << m_data.uid() << " iswait = " << m_data.iswait() << END;
	for(int i = 0; i < m_data.guilds_size(); ++i)
	{
		SSInfo << " guild = " << m_data.guilds(i).guildid() << " group = " << m_data.guilds(i).group() << END;	
	}

	InitState();
	InitJvDian();

	return true;
}

UINT32 SceneCastle::GetGroup(UINT64 roleid)
{
	UINT64 gid = GuildSimpleMgr::Instance()->GetGuildId(roleid);
	for(int i = 0; i < m_data.guilds_size(); ++i)
	{
		if(m_data.guilds(i).guildid() == gid)
		{
			return m_data.guilds(i).group();
		}
	}
	SSWarn << " find group failed gid = " << gid << " role = " << roleid << " fightid = " << m_data.uid() << END;
	return 1;
}

UINT64 SceneCastle::GetFightGroupUId(UINT64 roleid)
{
	if(m_data.iswait())//等待场景
	{
		return UINT64(-1);
	}
	return (UINT64)(GetGroup(roleid));
}

bool SceneCastle::OnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if(NULL == pRole)
	{
		return false;
	}

	if(pRole->IsWatcher())
	{
		return false;
	}

	switch(event.type)
	{
	case VS_EVENT_ROLE_QUIT:
		{
			OnLeave(event.roleid);
		}
		break;
	case VS_EVENT_ROLE_DIE:
		{
			UINT64 killID = pRole->GetKillerID();
			UINT64 deadID = pRole->GetID();

			OnKill(killID, deadID);
			m_deadRoles[deadID] = time(NULL);
		}
		break;
	case VS_EVENT_ROLE_LOADED:
		{
			AllCountToMe(event.roleid);
			AllCountToOther(event.roleid);

			//记录(会重复调用)
			pRole->Get<CReportDataRecord>()->UpdateWeekData(KKSG::WeekReportData_GuildTerryitory);
		}
		break;
	case VS_EVENT_ROLE_REVIVE:
		{
			m_deadRoles.erase(event.roleid);
		}
		break;
	default:
		break;
	}

	return true;
}

void SceneCastle::Update()
{
	m_statemgr.Update();

	UpdateDeadRole();

	UpdateJvDian();
}

void SceneCastle::InitState()
{
	m_statemgr.Init(this, &VsInFight::GlobalVsInFight);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

VsStateType SceneCastle::CheckFightEnd()
{
	if(IsFightEnd())
	{
		return VS_STATE_END;
	}

	//防止没有收到ms通知结束(5分钟)
	if(GetStateStartTime() + m_data.existtime() + (600) <= UINT32(time(NULL)))
	{
		return VS_STATE_END;
	}

	return VS_STATE_DUMMY;
}

void SceneCastle::OnStateEnd()
{
	SSDebug << __FUNCTION__ << END;
	GetScene()->SetDontDestory(false);
	SetEnd();
}

void SceneCastle::MakeOccupy(UINT64 roleID, KKSG::GCFJvDianType type)
{
	SSDebug << __FUNCTION__ << " role = " << roleID << " type = " << type << END;
	//goto ms
	PtcG2M_GCFSynG2M ptc2m;
	ptc2m.m_Data.set_type(KKSG::GCF_SYN_OCCUPY);
	ptc2m.m_Data.set_uid(m_data.uid());
	ptc2m.m_Data.set_roleid(roleID);
	ptc2m.m_Data.set_jvdian(type);
	MSLink::Instance()->SendTo(ptc2m);

	//goto c
	auto pGuild = GuildSimpleMgr::Instance()->GetGuildByRoleId(roleID);
	if(pGuild)
	{
		PtcG2C_GCFSynG2CNtf ptc2c;
		ptc2c.m_Data.set_type(KKSG::GCF_G2C_SYN_OCCUPY);
		ptc2c.m_Data.set_roleid(roleID);
		ptc2c.m_Data.mutable_jvdian()->set_guildname(pGuild->GetGuildName());
		ptc2c.m_Data.mutable_jvdian()->set_type(type);
		ptc2c.m_Data.set_mapid(GetScene()->GetSceneTemplateID());
		ptc2c.m_Data.set_territoryid(m_data.territoryid());
		GetScene()->Broadcast(ptc2c);
	}
}

void SceneCastle::BackToReady(UINT64 roleID)
{
	//goto ms
	PtcG2M_GCFSynG2M ptc;
	ptc.m_Data.set_type(KKSG::GCF_SYN_BACK_TO_READY);
	ptc.m_Data.set_uid(m_data.uid());
	ptc.m_Data.set_roleid(roleID);
	MSLink::Instance()->SendTo(ptc);
}

void SceneCastle::OnMultiPoint(UINT32 mapID, UINT32 multi)
{
	SSDebug << __FUNCTION__ << " map = " << mapID << " mul = " << multi << END;
	//goto c
	PtcG2C_GCFSynG2CNtf ptc;
	ptc.m_Data.set_type(KKSG::GCF_G2C_SYN_MUL_POINT);
	ptc.m_Data.set_territoryid(m_data.territoryid());
	ptc.m_Data.set_mapid(mapID);
	ptc.m_Data.set_mulpoint(multi);
	GetScene()->Broadcast(ptc);
}

void SceneCastle::OnKill(UINT64 roleID, UINT64 deadID)
{
	SSDebug << __FUNCTION__ << " kill = " << roleID << " dead = " << deadID << END;
	//goto ms
	PtcG2M_GCFSynG2M ptc;
	ptc.m_Data.set_type(KKSG::GCF_SYN_KILL);
	ptc.m_Data.set_uid(m_data.uid());
	ptc.m_Data.set_roleid(roleID);
	Role* pRole = GetRole(roleID);
	if(pRole)
	{
		ptc.m_Data.set_continuekill(pRole->Statistics().GetKillContinueCount());
	}
	MSLink::Instance()->SendTo(ptc);

	//goto c
	//Role* pKillRole = RoleManager::Instance()->FindByRoleID(roleID);
	//Role* pDeadRole = RoleManager::Instance()->FindByRoleID(deadID);
	//if(pKillRole && pDeadRole)
	//{
	//	PtcG2C_GCFSynG2CNtf ptc2c;
	//	ptc2c.m_Data.set_type(KKSG::GCF_G2C_SYN_KILL_ONE);
	//	ptc2c.m_Data.set_killname(pKillRole->GetName());
	//	ptc2c.m_Data.set_deadname(pDeadRole->GetName());
	//	GetScene()->Broadcast(ptc2c);
	//}

	VsKillNotify(deadID, 0);
}

void SceneCastle::OnLeave(UINT64 roleID)
{
	PtcG2M_GCFSynG2M ptc;
	ptc.m_Data.set_type(KKSG::GCF_SYN_LEAVE_BATTLE);
	ptc.m_Data.set_uid(m_data.uid());
	ptc.m_Data.set_roleid(roleID);
	ptc.m_Data.set_sceneid(GetScene()->GetSceneID());
	MSLink::Instance()->SendTo(ptc);
}

void SceneCastle::OnOccupyState(UINT64 roleID, int type, int state)
{
	SSDebug << __FUNCTION__ << " role = " << roleID << " type = " << type << " state = " << state << END;
	if(!KKSG::GCFZhanLingType_IsValid(state))
	{
		return ;
	}
	if(!KKSG::GCFJvDianType_IsValid(type))
	{
		return ;
	}



	PtcG2C_GCFZhanLingNotify ptc;
	ptc.m_Data.set_roleid(roleID);
	ptc.m_Data.set_zltype(KKSG::GCFZhanLingType(state));
	ptc.m_Data.set_jdtype(KKSG::GCFJvDianType(type));

	GetScene()->Broadcast(ptc);
	
	if(KKSG::GCFZL_END == state)
	{
		MakeOccupy(roleID, KKSG::GCFJvDianType(type));	
	}
}

void SceneCastle::UpdateDeadRole()
{
	time_t now = time(NULL);
	for(auto it = m_deadRoles.begin(); it != m_deadRoles.end(); )
	{
		if(it->second + 3 <= now)
		{
			BackToReady(it->first);
			m_deadRoles.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void SceneCastle::UpdateJvDian()
{
	for(size_t i = 0; i < m_jvDians.size(); ++i)
	{
		m_jvDians[i].Update();
	}
}

void SceneCastle::InitJvDian()
{
	if(IsWaitS())
	{
		return ;
	}

	auto& poss = GetGlobalConfig().GuildCastleJvDianPos;
	auto& radiuss = GetGlobalConfig().GuildCastleJvDianRadius; 

	for(size_t i = 0; i < poss.size() && i < radiuss.size(); ++i)
	{
		Vector3 tmppos(poss[i][0], poss[i][1], poss[i][2]);
		float tmpr = radiuss[i];

		SSDebug << " pos = " << tmppos.x << " " << tmppos.y << " " << tmppos.z << END;
		SSDebug << " r = " << tmpr << END;

		int type = (i+1);
		if(KKSG::GCFJvDianType_IsValid(type))
		{
			VsJvDian jvdian(type, tmppos, tmpr, this);
			AddJvdian(jvdian);
		}
	}
}

void SceneCastle::AddJvdian(const VsJvDian& jvdian)
{
	m_jvDians.push_back(jvdian);
}

void SceneCastle::AllCountChange(UINT64 roleID, UINT32 count)
{
	m_killCount[roleID] = count;
	AllCountToOther(roleID);
}

void SceneCastle::AllCountToMe(UINT64 roleID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
	if(NULL == pRole)
	{
		return ;
	}

	PtcG2C_GCFSynG2CNtf ptc;
	ptc.m_Data.set_type(KKSG::GCF_G2C_SYN_KILL_COUNT);
	for(auto it = m_killCount.begin(); it != m_killCount.end(); ++it)
	{
		if((it->first != roleID) && GetRole(it->first))
		{
			SSDebug << __FUNCTION__ << " role = " << roleID << " first = " << it->first << " second = " << it->second << END;
			ptc.m_Data.set_roleid(it->first);
			ptc.m_Data.set_killcount(it->second);
			pRole->Send(ptc);
		}
	}
}

void SceneCastle::AllCountToOther(UINT64 roleID)
{
	auto it = m_killCount.find(roleID);
	if(it == m_killCount.end())
	{
		return ;
	}

	SSDebug << __FUNCTION__ << " role = " << roleID << " count = " << (it->second) << END;

	PtcG2C_GCFSynG2CNtf ptc;
	ptc.m_Data.set_type(KKSG::GCF_G2C_SYN_KILL_COUNT);
	ptc.m_Data.set_roleid(roleID);
	ptc.m_Data.set_killcount(it->second);
	GetScene()->Broadcast(ptc);
}
