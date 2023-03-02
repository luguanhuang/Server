#include "pch.h"
#include "scenepktwo.h"
#include "pk/ptcg2c_pkroleinfontf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scenebase.h"
#include "scene.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/pkmgr.h"
#include "utility/tlogger.h"
#include "gamelogic/robotconfig.h"
#include "gamelogic/designationMgr.h"
#include "live/livemanager.h"

ScenePkTwo::ScenePkTwo(Scene* scene)
	:ScenePkBase(scene)
{

}

ScenePkTwo::~ScenePkTwo()
{

}

bool ScenePkTwo::Init(const KKSG::CreateBattleParam& roData)
{
	m_data = roData.kmatchfightdata();

	InfoPrint();

	InitRobotInfo(roData.kmatchfightdata());

	InfoPrint();

	if(m_data.roles_size() != GetVsCount() * 2)
	{
		return false;
	}

	InitDRoleData();

	InitState();

	m_strTransTag = TTag::Instance()->GetTagString();

	bool haverobot = false;
	std::vector<UINT64> roleIDs;
	for (int i = 0; i < m_data.roles_size(); ++i)
	{
		if (Unit::GetUnitType(m_data.roles(i).roleid()) != KKSG::Category_Role)
		{
			haverobot = true;
			break;
		}
		roleIDs.push_back(m_data.roles(i).roleid());
	}
	if (!haverobot)
	{
		LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_PVP2, GetScene()->GetSceneID(), 0, 0, &roleIDs);
	}

	return true;
}

void ScenePkTwo::InitRobotInfo(const KKSG::KMatchFightData& tmpData)
{
	if(tmpData.roles_size() != GetVsCount())
	{
		return;
	}

	for(int i = 0; i < tmpData.roles_size(); ++i)
	{
		if(tmpData.roles(i).pkmatchstage_size() > 0)
		{
			int lookupID = tmpData.roles(i).pkmatchstage(0).lookupid();
			KKSG::RoleSmallInfo robotInfo;
			if(!AddRobot(tmpData.roles(i).rolebrief(), lookupID, robotInfo))
			{
				return;
			}
			auto& robotData = *m_data.add_roles();
			robotData.set_roleid(robotInfo.roleid());
			robotData.set_serverid(0);//机器人填0
			*robotData.mutable_rolebrief() = robotInfo;
			UINT32 point = GetRobotPoint(tmpData.roles(i).pkrec().point());
			robotData.mutable_pkrec()->set_point(point);
		}
		else
		{
			SSError << " scene = " << GetScene()->GetSceneID() << " pk matchstage = 0" << END;
			return;
		}

	}
}

void ScenePkTwo::InitDRoleData()
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		const auto& rd = m_data.roles(i);
		auto& dd = m_roles[rd.roleid()];
		dd.roleID = rd.roleid();
		if(Unit::GetUnitType(rd.roleid()) == KKSG::Category_DummyRole)
		{
			dd.state = VS_ROLE_LOADED;//机器人直接loaded
		}
		else
		{
			dd.state = VS_ROLE_WAITLOAD;
		}
		if(i < GetVsCount())
		{
			dd.group = 1;
		}
		else
		{
			dd.group = 2;
		}
	}
}

void ScenePkTwo::OnEnterBeforeNtf(Role* role)
{
	PtcG2C_PkRoleInfoNtf ptc;

	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		auto& rd = m_data.roles(i);
		auto& info = *ptc.m_Data.add_pkroleinfo();
		*info.mutable_pkrec() = rd.pkrec();
		*info.mutable_rolebrief() = rd.rolebrief();
		info.set_serverid(rd.serverid());	
	}

	role->Send(ptc);
}

UINT32 ScenePkTwo::GetGroup(UINT64 roleid)
{
	auto pData = GetDRoleData(roleid);
	if(pData)
	{
		return pData->group;
	}
	return 1;
}

bool ScenePkTwo::OnEvent(const VsEvent& event)
{
	//忽略玩家观战
	if(Unit::GetUnitType(event.roleid) == KKSG::Category_Role)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);	
		if(pRole && pRole->IsWatcher())
		{
			return false;
		}
	}

	auto pData = GetDRoleData(event.roleid);
	if(NULL == pData)
	{
		//SSError << " find droledata failed roleid = " << event.roleid << END;
		return false;
	}

	switch(event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			pData->state = VS_ROLE_LOADED;
		}
		break;
	case VS_EVENT_ROLE_QUIT:
	case VS_EVENT_ROLE_LOADING_QUIT:
		{
			pData->state = VS_ROLE_QUIT;
			Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
			if(pRole)
			{
				OnRoleLeave(pRole);
			}
		}
		break;
	case VS_EVENT_ROLE_DIE:
		{
			pData->state = VS_ROLE_DIE;
		}
		break;
	case VS_EVENT_ROLE_REVIVE:
		{
			if(VS_ROLE_DIE == pData->state)//复活则恢复上一个状态
			{
				pData->state = VS_ROLE_LOADED;
			}
		}
		break;
	default:
		break;
	}

	return true;
}

VsStateType ScenePkTwo::IsAllLoad()
{
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		if(it->second.state == VS_ROLE_WAITLOAD)
		{
			return VS_STATE_DUMMY;
		}
	}
	return VS_STATE_PREPARE321;
}

void ScenePkTwo::GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded)
{
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		UINT64 roleID = it->first;
		if(Unit::GetUnitType(roleID) != KKSG::Category_Role)
		{
			continue;
		}

		if(VS_ROLE_LOADED == (it->second).state)
		{
			Role* pRole = GetRole(roleID);
			if(pRole)
			{
				loaded.push_back(pRole);
			}
		}
		else if(VS_ROLE_WAITLOAD == (it->second).state)
		{
			unloaded.push_back(GetRoleName(roleID));
		}
	}
}

void ScenePkTwo::InfoPrint()
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		SSInfo << " role = " << m_data.roles(i).roleid() << " server = " << m_data.roles(i).serverid() << END;
		SSInfo << " point = " << m_data.roles(i).pkrec().point() << END;
	}
}

KKSG::KMatchRole* ScenePkTwo::GetKKSGRoleData(UINT64 roleID)
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		if(m_data.roles(i).roleid() == roleID)
		{
			return m_data.mutable_roles(i);
		}
	}
	return NULL;
}

BPkRoleData* ScenePkTwo::GetRoleBaseData(UINT64 roleID)
{
	 return GetDRoleData(roleID);
}

DPkRoleData* ScenePkTwo::GetDRoleData(UINT64 roleID)
{
	auto it = m_roles.find(roleID);
	if(it != m_roles.end())
	{
		return &it->second;
	}
	return NULL;
}

UINT32 ScenePkTwo::GetRolePoint(UINT64 roleID)
{
	auto pData = GetKKSGRoleData(roleID);	
	if(pData)
	{
		return pData->pkrec().point();
	}
	return 0;
}

std::string ScenePkTwo::GetRoleName(UINT64 roleID)
{
	auto pData = GetKKSGRoleData(roleID);
	if(pData)
	{
		return pData->rolebrief().rolename();
	}
	return "";
}

VsStateType ScenePkTwo::CheckFightEnd()
{
	UINT32 winGroup = 0;	
	
	UINT32 score1 = 0;
	UINT32 score2 = 0;

	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		auto& data = (it->second);
		if(data.state != VS_ROLE_LOADED)
		{
			if(data.group == 1)
			{
				score2 ++;
			}
			else if(data.group == 2)
			{
				score1 ++;
			}
		}
	}

	if(score1 == GetVsCount())
	{
		winGroup = 1;
	}
	else if(score2 == GetVsCount())
	{
		winGroup = 2;
	}

	if(!winGroup && GetStateStartTime() +  m_fightUpdateTime <= time(NULL))
	{
		if(score1 > score2)
		{
			winGroup = 1;
		}
		else if(score1 < score2)
		{
			winGroup = 2;
		}
		else
		{
			return VS_STATE_END;//平局
		}
	}

	if(winGroup)
	{
		for(auto it = m_roles.begin(); it != m_roles.end(); ++it )
		{
			auto& data = (it->second);
			if(data.isdone)//提前结算了
			{
				continue;
			}
			data.result = (data.group == winGroup)?KKSG::PkResult_Win:KKSG::PkResult_Lose;
		}
		return VS_STATE_END;
	}

	return VS_STATE_DUMMY;

}

UINT32 ScenePkTwo::GetVsCount()
{
	return 2;
}

void ScenePkTwo::OnStateEnd()
{
	GetScene()->SetDontDestory(false);

	MakeResult();

	BeginRankUpdateDelay();
}

void ScenePkTwo::MakeResult()
{
	SSDebug << __FUNCTION__ << END;
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(it->first);
		if(pRole)
		{
			EndRole(pRole);
		}
	}
}

void ScenePkTwo::FillRobots(std::list<UINT64>& robots)
{
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		UINT64 roleID = (it->first);
		if(Unit::GetUnitType(roleID) != KKSG::Category_Role)
		{
			robots.push_back(roleID);
		}
	}
}

UINT32 ScenePkTwo::GetAVGPoint(UINT32 group)
{
	UINT32 allPoint = 0;
	UINT32 roleCount = 0;
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		if(GetGroup(m_data.roles(i).roleid()) == group)
		{
			allPoint += m_data.roles(i).pkrec().point();
			roleCount ++;
		}
	}
	if(0 == roleCount)
	{
		return 0;
	}
	return (UINT32)(allPoint/roleCount);
}

void ScenePkTwo::OnRoleLeave(Role* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	auto pData = GetDRoleData(pRole->GetID());
	if(NULL == pData)
	{
		return;
	}
	if(pData->isdone)
	{
		return;
	}

	pData->result = KKSG::PkResult_Lose;//提前退出判负

	EndRole(pRole);

	SendRankReq(pRole, 0);//故意添0 协议回来不处理
}

void ScenePkTwo::EndRole(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}

	auto pData = GetDRoleData(pRole->GetID());
	if(NULL == pData)
	{
		return;
	}

	if(pData->isdone)
	{
		return;
	}

	UINT32 myGroup = GetGroup(pRole->GetID());	
	UINT32 otherGroup = (1 == myGroup)?2:1;
	UINT32 myPoint = GetRolePoint(pRole->GetID());//GetAVGPoint(myGroup);	
	if(0 == myPoint)
	{
		SSWarn << "point = 0 role = " << pRole->GetID() << END;
	}
	UINT32 otherPoint = GetAVGPoint(otherGroup);
	int pointSpan = (int(myPoint)) - (int(otherPoint));
	int cpoint = PkMgr::Instance()->GetPoint(myPoint, pointSpan, pData->result);

	SSInfo << " role = " << pRole->GetID() << " mygroup = " << myGroup << " othergroup = " << otherGroup 
		<< " mypoint = " << myPoint << " otherpoint = " << otherPoint << " point = " << cpoint << END;

	pData->point = cpoint;

	KKSG::PkNVNType nvnType = KKSG::PK_2v2;
	CPkRecord* record = pRole->Get<CPkRecord>();
	record->Update();
	// point & record & honorpoint
	//UINT32 honorpoint = 0;
	//UINT32 dragoncount = 0;
	//if(record->GetRewardLeftCount(nvnType) > 0)
	//{
	//	record->AddRewardCount(nvnType);

	//	honorpoint = GetGlobalConfig().PkDayReward[1];
	//	record->AddHonorPoint(honorpoint);

	//	dragoncount = GetGlobalConfig().PkDayReward[2];
	//	BagGiveItemTransition give(pRole);
	//	give.SetReason(ItemFlow_Stage, ItemFlow_Stage_Pk);
	//	give.GiveItem(DRAGON_COIN, dragoncount);
	//	give.NotifyClient();
	//}
	record->AddPoint(nvnType, pData->point);


	KKSG::PkOneRec oneRec;
	oneRec.set_ret(pData->result);
	oneRec.set_cpoint(pData->point);
	FillOneRec(myGroup, oneRec);
	record->AddResult(nvnType, oneRec);

	if (pData->result == KKSG::PkResult_Win)
	{
		pRole->OnSceneComplete(GetScene());
		DesignationMgr::Instance()->On2V2SkyLadderScore(pRole,record->GetPoint(nvnType));
	}
	else if (pData->result == KKSG::PkResult_Lose)
	{
		pRole->OnSceneFailed(GetScene());
	}
	else	
	{
		pRole->OnSceneFailed(GetScene());
	}
	//暂时没有
	//pData->honorpoint = honorpoint;
	//pData->dragoncount = dragoncount;

	pData->isdone = true;

	//tlog
	TPkTwoFlow oLog(pRole);
	oLog.SetTransTag(m_strTransTag);
	oLog.m_Result = pData->result;
	oLog.m_Group = myGroup;
	oLog.Do();
}

void ScenePkTwo::FillUnitIDs(std::vector<UINT64>& unitIDs)
{
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		unitIDs.push_back(it->first);
	}
}

void ScenePkTwo::FillOneRec(UINT32 myGroup, KKSG::PkOneRec& rec)
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		UINT64 roleID = m_data.roles(i).roleid();
		KKSG::PvpRoleBrief* pBrief = NULL;
		if(GetGroup(roleID) == myGroup)
		{
			pBrief = rec.add_myside();
		}
		else
		{
			pBrief = rec.add_opside();
		}
		if(pBrief)
		{
			auto& tmp = m_data.roles(i).rolebrief();
			pBrief->set_roleid(tmp.roleid());
			pBrief->set_rolename(tmp.rolename());
			pBrief->set_rolelevel(tmp.rolelevel());
			pBrief->set_roleprofession(tmp.roleprofession());
			pBrief->set_roleserverid(m_data.roles(i).serverid());
		}
	}
}

UINT64 ScenePkTwo::GetWin()
{
	for (auto i = m_roles.begin(); i != m_roles.end(); ++i)
	{
		if (KKSG::PkResult_Win == i->second.result)
		{
			return i->first;
		}
	}
	return 0;
}
