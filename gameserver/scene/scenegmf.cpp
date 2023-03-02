#include "pch.h"
#include "scenegmf.h"
#include "scene.h"
#include "unit/rolemanager.h"
#include "pk/ptcg2c_pkpreparentf.h"
#include "scenemanager.h"
#include "guild/ptcg2c_gmfonebattleendntf.h"
#include "guild/ptcg2c_gmfallfightendntf.h"
#include "guild/guildmgr.h"
#include "pb/project.pb.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "unit/rolemanager.h"
#include "sceneswitch.h"
#include "mslink.h"
#include "guild/ptcg2c_gmfbasedatantf.h"
#include "gamelogic/rolesummarymgr.h"
#include "guild/ptcg2c_gmfwaitotherload.h"
#include "guild/ptcg2c_gmfwaitfightbegin.h"
#include "gamelogic/noticemgr.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "guild/ptcg2m_gmfupdatedatag2m.h"
#include "vshandler.h"
#include "scene/ptcg2c_gmfkicknty.h"
#include "sceneinspire.h"
#include "scene/ptcg2c_gmfbattlestatentf.h"

SceneGMF::SceneGMF(Scene* scene) : SceneGWB(scene)
{
	m_roleID11 = 0;
	m_roleID22 = 0;
}

SceneGMF::~SceneGMF()
{

}

void SceneGMF::AllEndToClient(UINT64 winGuildID, UINT64 loseGuildID, int wintype)
{
	PtcG2C_GmfAllFightEndNtf gafe;
	FillGmfGuildBrief(*gafe.m_Data.mutable_winguild(), winGuildID);
	FillGmfGuildBrief(*gafe.m_Data.mutable_loseguild(), loseGuildID);
	if(KKSG::GMF_FINAL_WIN_TYPE_IsValid(wintype))
	{
		gafe.m_Data.set_wintype(KKSG::GMF_FINAL_WIN_TYPE(wintype));
	}

	if(GetScene())
	{
		GetScene()->Broadcast(gafe);
	}
	SSDebug << __FUNCTION__ << " " << winGuildID << " " << loseGuildID << " wintype = " << wintype << END;
}

void SceneGMF::MakeFightEnd()
{
	UINT64 winRoleID = GetWinID();	
	SSDebug << __FUNCTION__ << " winroleid = " << winRoleID << END;
	UINT64 winguildid = 0;
	UINT64 loseguildid = 0;
	KKSG::GMF_FINAL_WIN_TYPE battleWinType = KKSG::GMF_FWY_NORMAL;

	GMFHalfRoomType winhalftype = GMF_HRT_NONE;
	GMFHalfRoomType losehalftype = GMF_HRT_NONE;
	winhalftype = m_room.GetHalfRoomTypeByRoleID(winRoleID);

	if(GMF_HRT_NONE == winhalftype)//无人上阵
	{
		battleWinType = KKSG::GMF_FWY_RANK;
		SSDebug << __FUNCTION__ << " rank11 = " << m_room.GetPrestigeRank(GMF_HRT_ONE) << " rank22 = " << m_room.GetPrestigeRank(GMF_HRT_TWO) << END;
		if(m_room.GetPrestigeRank(GMF_HRT_ONE) <= m_room.GetPrestigeRank(GMF_HRT_TWO))	
		{
			winhalftype = GMF_HRT_ONE;
		}
		else
		{
			winhalftype = GMF_HRT_TWO;
		}
	}
	losehalftype = m_room.GetOpHalfRoomType(winhalftype);
	if(KKSG::GMF_FWY_RANK != battleWinType && 0 == m_room.GetRoleCount(losehalftype))
	{
		battleWinType = KKSG::GMF_FWY_OPNONE;
	}

	if(GMF_HRT_ONE == winhalftype)
	{
		winguildid = m_room.GetGuildID(GMF_HRT_ONE);	
		loseguildid = m_room.GetGuildID(GMF_HRT_TWO);
	}
	else
	{
		winguildid = m_room.GetGuildID(GMF_HRT_TWO);	
		loseguildid = m_room.GetGuildID(GMF_HRT_ONE);
	}

	AllEndToClient(winguildid, loseguildid, battleWinType);
	AllEndToMs(winguildid, loseguildid);

}

UINT64 SceneGMF::GetHpPrecentLessID()
{
	Role* pRole11 = RoleManager::Instance()->FindByRoleID(GetRoleID11());
	Role* pRole22 = RoleManager::Instance()->FindByRoleID(GetRoleID22());

	if(NULL == pRole11)
	{
		return GetRoleID11();
	}
	else if(NULL == pRole22)
	{
		return GetRoleID22();
	}

	if(pRole11->GetPreciseHpPercent() < pRole22->GetPreciseHpPercent())
	{
		return GetRoleID11();
	}

	if(pRole11->GetPreciseHpPercent() > pRole22->GetPreciseHpPercent())
	{
		return GetRoleID22();
	}

	//血量百分比一样  血少的输
	if(pRole11->GetAttr(TOTAL_CurrentHP) < pRole22->GetAttr(TOTAL_CurrentHP))
	{
		return GetRoleID11();
	}

	return GetRoleID22();

}

bool SceneGMF::SetFightRole()
{
	if(0 == GetRoleID11())
	{
		UINT64 roleid = m_room.GetNextFightRoleID(GetScene(), GMF_HRT_ONE);
		SetRoleID11(roleid);
	}
	if(0 == GetRoleID22())
	{
		UINT64 roleid = m_room.GetNextFightRoleID(GetScene(), GMF_HRT_TWO);
		SetRoleID22(roleid);
	}

	SSDebug << __FUNCTION__ << " role11 = " << GetRoleID11() << " role22 = " << GetRoleID22() << END;

	if(0 == GetRoleID11() || 0 == GetRoleID22())
	{
		return false;
	}

	m_fightRoles.clear();
	m_fightRoles.push_back(GetRoleID11());
	m_fightRoles.push_back(GetRoleID22());

	//先直接切
	Role* pRole = GetRole(GetRoleID11());
	if(pRole && pRole->IsWatcher())
	{
		ChangeWatcherState(pRole, false);
	}
	
	pRole = GetRole(GetRoleID22());
	if(pRole && pRole->IsWatcher())
	{
		ChangeWatcherState(pRole, false);
	}

	RoomStateToClient();

	return true;
}

KKSG::GuildMatchFightState ConverFailReasionToState(int type)
{
	switch(type)
	{
	case KKSG::GMF_FAIL_DIE:
	case KKSG::GMF_FAIL_QUIT:
	case KKSG::GMF_FAIL_TIMEOVER:
		{
			return KKSG::GUILD_MF_FAILED;
		}
		break;
	case KKSG::GMF_FAIL_REFRESE:
		{
			return KKSG::GUILD_MF_REFUSE;
		}
		break;
	default:
		{

		}
		break;
	}
	return KKSG::GUILD_MF_FAILED;
}

void SceneGMF::OnRoleFailed(UINT64 failID, int type)
{
	UINT64 winID = 0;
	if(GetRoleID11() == failID)
	{
		SetRoleID11(0);

		winID = GetRoleID22();
	}
	else if(GetRoleID22() == failID)
	{
		SetRoleID22(0);

		winID = GetRoleID11();
	}
	else
	{
		SSError << " error failed roleid = " << failID<< END;
	}

	MakePuppet(GetUnit(winID), true);
	if(type == KKSG::GMF_FAIL_TIMEOVER)
	{
		MakePuppet(GetUnit(failID), true);
	}

	OneFightEndNotice(winID, failID, m_room.GetGuildIDByRoleID(winID), m_room.GetGuildIDByRoleID(failID));
	TLogOneFightEnd(winID, failID, m_room.GetGuildIDByRoleID(winID), m_room.GetGuildIDByRoleID(failID));

	m_room.SetRoleMFState(failID, ConverFailReasionToState(type));

	RoomStateToClient();

	PtcG2C_GmfOneBattleEndNtf oben;
	oben.m_Data.set_reason(KKSG::GMFFailReason(type));
	FillGmfRoleBrief(*oben.m_Data.mutable_winrole(), winID);
	FillGmfRoleBrief(*oben.m_Data.mutable_loselrole(), failID);
	FillGmfGuildBrief(*oben.m_Data.mutable_winguild(), m_room.GetGuildIDByRoleID(winID));
	FillGmfGuildBrief(*oben.m_Data.mutable_loseguild(), m_room.GetGuildIDByRoleID(failID));

	GetScene()->Broadcast(oben, NULL);

	SSDebug << __FUNCTION__ << " notify win = " << oben.m_Data.winrole().rolename() << " lose = " << oben.m_Data.loselrole().rolename() << " type = " << type << END;
}

UINT64 SceneGMF::GetWinID()
{
	if(GetRoleID11())
	{
		return GetRoleID11();
	}
	if(GetRoleID22())
	{
		return GetRoleID22();
	}
	return 0;
}

void SceneGMF::OnEnterWaitLoad()
{
	RoomStateToClient();
}

VsStateType SceneGMF::IsAllLoad()
{
	if(GetRoleID11() && !m_loadMgr.HaveRole(GetRoleID11()))
	{
		return VS_STATE_DUMMY;
	}
	if(GetRoleID22() && !m_loadMgr.HaveRole(GetRoleID22()))
	{
		return VS_STATE_DUMMY;
	}
	return VS_STATE_PREPARE321;
}

VsStateType SceneGMF::OnLoadTimeOut()
{
	return VS_STATE_AFTER_FIGHT;
}

VsStateType SceneGMF::CheckFightEnd()
{
	int reason = CheckRoleFail(GetRoleID11());
	if(KKSG::GMF_FAIL_NONE != reason)
	{
		return VS_STATE_AFTER_FIGHT;
	}
	reason = CheckRoleFail(GetRoleID22());
	if(KKSG::GMF_FAIL_NONE != reason)
	{
		return VS_STATE_AFTER_FIGHT;
	}

	if(0 == GetFightLeftTime())
	{
		return VS_STATE_AFTER_FIGHT;
	}
	return VS_STATE_DUMMY;
}

int SceneGMF::CheckRoleFail(UINT64 roleid)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleid);
	if(NULL == pRole || pRole->IsDead())
	{
		return KKSG::GMF_FAIL_DIE;
	}
	if(!m_loadMgr.HaveRole(roleid))
	{
		return KKSG::GMF_FAIL_QUIT;
	}
	return KKSG::GMF_FAIL_NONE;
}

void SceneGMF::OnStateEnd()
{
	SSDebug << __FUNCTION__ << END;

	RoomStateToClient();

	MakeFightEnd();	

	GetScene()->SetDontDestory(false);
	SetEnd();
}

void SceneGMF::OnEnterAfterFight()
{
	SSDebug << __FUNCTION__ << END;

	EndSkills(m_fightRoles);

	int reason = CheckRoleFail(GetRoleID11());
	if(KKSG::GMF_FAIL_NONE != reason)
	{
		OnRoleFailed(GetRoleID11(), reason);
		return ;
	}
	reason = CheckRoleFail(GetRoleID22());
	if(KKSG::GMF_FAIL_NONE != reason)
	{
		OnRoleFailed(GetRoleID22(), reason);
		return ;
	}

	OnRoleFailed(GetHpPrecentLessID(), KKSG::GMF_FAIL_TIMEOVER);

}

VsStateType SceneGMF::CheckAfterFightEnd()
{
	if(GetAfterFightLeftTime() > 0)
	{
		return VS_STATE_DUMMY;
	}

	bool flag = SetFightRole();
	if(!flag)
	{
		return VS_STATE_END;
	}

	return VS_STATE_WAIT_LOAD;
}

void SceneGMF::OneFightEndNotice(UINT64 winID, UINT64 loseID, UINT64 winGuildID, UINT64 loseGuildID)
{
	Role* winRole = RoleManager::Instance()->FindByRoleID(winID);
	Role* loseRole = RoleManager::Instance()->FindByRoleID(loseID);

	if(winRole && loseRole)
	{
		NoticeMgr::Instance()->NoticeGuildArenaOneFightEnd(winRole, loseRole, winGuildID, loseGuildID);
	}
}
	
void SceneGMF::TLogOneFightEnd(UINT64 winID, UINT64 loseID, UINT64 winGuildID, UINT64 loseGuildID)
{
	Role* winRole = RoleManager::Instance()->FindByRoleID(winID);
	if (winRole)
	{
		TGuildArenaFlow oLog(winRole);
		oLog.m_uGuildID = winGuildID;
		oLog.m_iOp = TGuildArena_Type2;
		oLog.m_uGuildID2 = loseGuildID;
		oLog.m_uRoleId2 = loseID;
		oLog.Do();
	}
}

void SceneGMF::OnJoinStart()
{
	m_enterStop = false;
	m_joinStop = false;
}

VsStateType SceneGMF::OnJoinStop()
{
	SSDebug << __FUNCTION__ << END;
	m_joinStop = true;

	bool flag = SetFightRole();
	if(!flag)
	{
		return VS_STATE_END;//直接没有人上阵
	}

	return VS_STATE_WAIT_LOAD;
}
