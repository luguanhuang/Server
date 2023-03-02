#include "pch.h"
#include "scenepk.h"
#include "scene.h"
#include "time.h"
#include "unit/rolemanager.h"
#include "unit/enemymanager.h"
#include "unit/role.h"
#include "define/itemdef.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/bagtransition.h"
#include "pk/ptcg2c_pkpreparentf.h"
#include "table/globalconfig.h"
#include "gamelogic/randomnamemgr.h"
#include "pk/ptcg2c_pkroleinfontf.h"
#include "util/XRandom.h"
#include "gamelogic/arenamgr.h"
#include "gamelogic/pkmgr.h"
#include "gamelogic/rolesummarymgr.h"
#include "unit/dummyrole.h"
#include "unit/dummyrolemanager.h"
#include "gamelogic/robotgenerator.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/achievev2Mgr.h"
#include "live/livemanager.h"
#include "attributepk.h"
#include "foreach.h"
#include "scenemanager.h"
#include "pk/rpcg2m_pkrankreq.h"
#include "network/mslink.h"
#include "guild/ptcg2m_notifyscenepkwin.h"
#include "gamelogic/activityrecord.h"
#include "scenestatistics.h"
#include "gamelogic/reportdatamgr.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "gamelogic/LotteryMgr.h"
#include "gamelogic/goalaward.h"

ScenePk::ScenePk(Scene* scene):ScenePkBase(scene)
{
	m_isentertimeout = false;
}

ScenePk::~ScenePk()
{
}
bool ScenePk::InitBaseData()
{
	for(int i = 0; i < m_pkdata.roles_size(); ++i)
	{
		UINT64 roleID = m_pkdata.roles(i).rolebrief().roleid();
		if(Unit::GetUnitType(roleID) == KKSG::Category_DummyRole)
		{
			m_robots.push_back(roleID);
		}
	}

	if(2 != m_pkdata.roles_size())
	{
		return false;
	}

	SPkResult* infoa = AddUnit(m_pkdata.roles(0));
	SPkResult* infob = AddUnit(m_pkdata.roles(1));
	FillOther(infoa, infob);

	return true;
}

void ScenePk::OnEnterBeforeNtf(Role* role)
{
	PtcG2C_PkRoleInfoNtf ptc;

	for(int i = 0; i < m_pkdata.roles_size(); ++i)
	{
		auto& tmpinfo = *ptc.m_Data.add_pkroleinfo();
		tmpinfo = m_pkdata.roles(i);
		std::list<UINT32> records;
		for(int j = tmpinfo.pkrec().records_size() - 1; j >= 0; --j)
		{
			records.push_front(tmpinfo.pkrec().records(j));
			if(records.size() >= 5)
			{
				break;
			}
		}
		tmpinfo.mutable_pkrec()->clear_records();
		for(auto it = records.begin(); it != records.end(); ++it)
		{
			tmpinfo.mutable_pkrec()->add_records(*it);		
		}
	}

	role->Send(ptc);
}

bool ScenePk::Init(const KKSG::CreateBattleParam& roData)
{
	m_pkdata = roData.pkinfo();//first
	if(1 == m_pkdata.roles_size())
	{
		InitRobotInfo();
	}
	if(2 != m_pkdata.roles_size())
	{
		return false;
	}

	std::vector<UINT64> roleIDs;
	bool haverobot = false;
	for(int i = 0; i < m_pkdata.roles_size(); ++i)
	{
		UINT64 roleID = m_pkdata.roles(i).rolebrief().roleid();
		if(Unit::GetUnitType(roleID) != KKSG::Category_Role)
		{
			haverobot = true;
			break;
		}
		roleIDs.push_back(roleID);
	}
	if (!haverobot)
	{
		LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_PVP, GetScene()->GetSceneID(), 0, 0, &roleIDs);
	}

	bool flag = InitBaseData();
	if(flag)
	{
		InitState();
	}

	for (int cnt = 0; cnt < m_pkdata.guildladderid_size(); ++cnt)
	{
		m_guildLadderId.insert(m_pkdata.guildladderid(cnt));
		SSInfo << " guildladderid = " << m_pkdata.guildladderid(cnt) << END;
	}

	return flag;
}

bool ScenePk::OnEvent(const VsEvent& event)
{
	switch(event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			OnRoleEnter(event.roleid);
			break;
		}
	case VS_EVENT_ROLE_QUIT:
		{
			OnRoleLeave(event.roleid);	
			break;
		}
	case VS_EVENT_ROLE_DIE:
		{
			OnRoleDie(event.roleid);
			break;
		}
	default:
		break;
	}

	return true;
}

void ScenePk::Result()
{
	bool invalidflag = false;//无效标志
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		auto& tmp = (i->second);
		if(VS_ROLE_LOADED == tmp.state)
		{
			m_wins.insert(i->first);
		}
		if(Unit::GetUnitType(i->first) == KKSG::Category_Role)
		{
			Role* pRole = RoleManager::Instance()->FindByRoleID(i->first);
			if(NULL == pRole)//如果有玩家找不到 则无法加/减分数 则两个人都不加/减分
			{
				invalidflag = true;
			}
		}
	}

	if (m_wins.size() > 1)
	{
		m_wins.clear();	
	}
	for (auto i = m_units.begin(); i != m_units.end(); ++i)			
	{
		auto& tmp = (i->second);
		if(m_wins.empty() || invalidflag)
		{
			tmp.End(KKSG::PkResult_Draw);
			continue;
		}

		if (IsWin(i->first))
		{
			tmp.End(KKSG::PkResult_Win);
		}
		else
		{
			tmp.End(KKSG::PkResult_Lose);
		}
	}
}

bool ScenePk::OnRoleEnter(UINT64 roleid)
{
	auto i = m_units.find(roleid);
	if (i == m_units.end())
	{
		return false;
	}
	i->second.state = VS_ROLE_LOADED;
	return true;
}

bool ScenePk::OnRoleLeave(UINT64 roleid)
{
	auto i = m_units.find(roleid);
	if (i == m_units.end())
	{
		return false;
	}
	i->second.state = VS_ROLE_QUIT;
	return true;
}

bool ScenePk::OnRoleDie(UINT64 roleid)
{
	auto i = m_units.find(roleid);
	if (i == m_units.end())
	{
		return false;
	}
	i->second.state = VS_ROLE_DIE;
	return true;
}

VsStateType ScenePk::IsAllLoad()
{
	if (m_units.empty())
	{
		return VS_STATE_DUMMY;
	}
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		if (i->second.state == VS_ROLE_WAITLOAD)	
		{
			return VS_STATE_DUMMY;
		}
	}
	return VS_STATE_PREPARE321;
}

bool ScenePk::CheckEnd()
{
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		if (VS_ROLE_LOADED != i->second.state)	
		{
			return true;
		}
	}
	return false;
}

bool ScenePk::IsWin(UINT64 roleid)
{
	auto i = m_wins.find(roleid);
	if (i == m_wins.end())
	{
		return false;
	}
	return true;
}

bool ScenePk::EndRole(Role* role)
{
	auto i = m_units.find(role->GetID());
	if (i == m_units.end())
	{
		return false;
	}
	if (i->second.isdone)	
	{
		return false;
	}	

	CPkRecord* record = role->Get<CPkRecord>();
	record->Update();
	// point & record & honorpoint
	UINT32 honorpoint = 0;
	UINT32 dragoncount = 0;
	if(record->GetRewradLeftCount() > 0)
	{
		record->AddRewardCount();

		honorpoint = GetGlobalConfig().PkDayReward[1];
		record->AddHonorPoint(honorpoint);

		dragoncount = GetGlobalConfig().PkDayReward[2];
		BagGiveItemTransition give(role);
		give.SetReason(ItemFlow_Stage, ItemFlow_Stage_Pk);
		give.GiveItem(DRAGON_COIN, dragoncount);
		give.NotifyClient();
	}
	if (i->second.result == KKSG::PkResult_Win)
	{
		record->AddPoint(role->GetID(), i->second.point);
		record->AddWinNum();
		record->PushRecord(i->first, i->second.other.prefession, i->second.other.name, i->second.point, honorpoint, i->second.result);
		role->OnSceneComplete(GetScene());

		DesignationMgr::Instance()->OnSkyLadderScore(role,record->GetPoint());
		DesignationMgr::Instance()->OnSkyLadderWinTimes(role,record->GetWin());

		AchieveV2Mgr::Instance()->OnPKWinTimes(role,record->GetWin());
		AchieveV2Mgr::Instance()->OnPKScore(role,record->GetPoint());
		// 手Q才有宝箱   微信也有分享红包
		if (role->GetClientConf().logintype() == KKSG::LOGIN_QQ_PF || role->GetClientConf().logintype() == LGOIN_WECHAT_PF)
		{
			LotteryMgr::Instance()->GiveShareRandomGift(role, KKSG::SCENE_PK);   // 内置宝箱
		}
		if (_IsGuildLadderTime())
		{
			PtcG2M_NotifyScenePkWin msg;
			msg.m_Data.set_roleid(role->GetID());
			role->SendToMS(msg);
			//MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), msg);
		}

        {
            UINT32 op_profession = 0;
            for(int i = 0; i< m_pkdata.roles().size(); ++i)
            {
                if(role->GetID() != m_pkdata.roles(i).rolebrief().roleid())
                {
                    op_profession = m_pkdata.roles(i).rolebrief().roleprofession();
                    op_profession %= 10;
                }
            }
            GoalAwardsConfig::Instance()->SetValue(role->GetID(), GOAL_AWARD_ID_200, 1.f, 0);
            switch(op_profession)
            {
            case 1:
                GoalAwardsConfig::Instance()->SetValue(role->GetID(), GOAL_AWARD_ID_201, 1.f, 0);
                break;
            case 2:
                GoalAwardsConfig::Instance()->SetValue(role->GetID(), GOAL_AWARD_ID_202, 1.f, 0);
                break;
            case 3:
                GoalAwardsConfig::Instance()->SetValue(role->GetID(), GOAL_AWARD_ID_203, 1.f, 0);
                break;
            case 4:
                GoalAwardsConfig::Instance()->SetValue(role->GetID(), GOAL_AWARD_ID_204, 1.f, 0);
                break;
            case 5:
                GoalAwardsConfig::Instance()->SetValue(role->GetID(), GOAL_AWARD_ID_205, 1.f, 0);
                break;
            case 6:
                GoalAwardsConfig::Instance()->SetValue(role->GetID(), GOAL_AWARD_ID_206, 1.f, 0);
                break;
            case 7:
                GoalAwardsConfig::Instance()->SetValue(role->GetID(), GOAL_AWARD_ID_207, 1.f, 0);
                break;
            default:
                break;
            }
        }
	}
	else if (i->second.result == KKSG::PkResult_Lose)
	{
		record->SubPoint(role->GetID(), -(i->second.point));
		record->AddLoseNum();
		record->PushRecord(i->first, i->second.other.prefession, i->second.other.name, i->second.point, honorpoint, i->second.result);

		role->OnSceneFailed(GetScene());
	}
	else	// 平局
	{
		record->AddDrawNum();
		record->PushRecord(i->first, i->second.other.prefession, i->second.other.name, i->second.point, honorpoint, i->second.result);

		role->OnSceneFailed(GetScene());
	}
	ReportDataMgr::Instance()->PkChange(role, i->second.result);
	i->second.honorpoint = honorpoint;
	i->second.dragoncount = dragoncount;
	i->second.isdone = true;


	BagGiveItemTransition transition(role);
	transition.SetReason(ItemFlow_Stage, ItemFlow_Stage_Pk);
	if (m_guildLadderId.find(role->GetID()) != m_guildLadderId.end() && role->Get<CActivityRecord>()->GetGuildLadderTime() < 3)
	{
		foreach (iter in  GetGlobalConfig().GuildLadderReward)
		{
			i->second.items[iter->seq[0]] = iter->seq[1];
			transition.GiveItem(iter->seq[0], iter->seq[1]);
		}
		role->Get<CActivityRecord>()->AddGuildLadderTime();
	}
	
	if (0 != honorpoint)
	{
		transition.GiveItem(ARENA_COIN, honorpoint);
	}
	transition.NotifyClient();

	if (_IsGuildLadderTime())
	{
		EventMgr::Instance()->AddEvent(role->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_GuildTianTiCount, 0, 1);
	}
	return true;
}

void ScenePk::EnableRobot(bool istrue)
{
	for (auto i = m_units.begin(); i != m_units.end(); ++i)		
	{
		DummyRole* pdummy = DummyRoleManager::Instance()->Find(i->first);
		if (NULL != pdummy)
		{
			pdummy->EnableAI(istrue);
		}
	}
}

void ScenePk::EndScene()
{
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(i->first);
		if (NULL != role)
		{
			EndRole(role);
		}
	}

	//LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_PVP, GetScene()->GetSceneID(), 0, 0, 0);
}

SPkResult* ScenePk::GetResult(UINT64 roleid)
{
	auto i = m_units.find(roleid);
	if (i == m_units.end())
	{
		return NULL;
	}
	if (!i->second.isdone)	
	{
		return NULL;
	}
	return &i->second;
}

BPkRoleData* ScenePk::GetRoleBaseData(UINT64 roleID)
{
	auto it = m_units.find(roleID);
	if(it != m_units.end())
	{
		return &(it->second);
	}
	return NULL;
}

SPkResult* ScenePk::AddUnit(const KKSG::PkRoleInfo& data)
{
	UINT64 roleID = data.rolebrief().roleid();
	SPkResult& info = m_units[roleID];
	info.basepoint = data.pkrec().point();
	info.other.name = data.rolebrief().rolename();
	info.other.prefession = data.rolebrief().roleprofession();

	if(Unit::GetUnitType(roleID) == KKSG::Category_DummyRole)
	{
		info.state = VS_ROLE_LOADED;
	}
	else
	{
		info.state = VS_ROLE_WAITLOAD;
	}

	return &info;
}

void ScenePk::FillOther(SPkResult* infoa, SPkResult* infob)
{
	int span = infoa->basepoint - infob->basepoint;
	infoa->other.pointspan = -span;
	infob->other.pointspan = span;
	std::swap(infoa->other, infob->other);
}

VsStateType ScenePk::OnLoadTimeOut()
{
	SetEnterTimeout();
	return VS_STATE_END;
}

void ScenePk::OnStateEnd()
{
	GetScene()->SetDontDestory(false);

	EnableRobot(false);
	UpdateBasePoint();
	Result();
	EndScene();

	BeginRankUpdateDelay();
}

void ScenePk::OnEnterFight()
{
	EnableRobot(true);	
}

VsStateType ScenePk::CheckFightEnd()
{
	if(CheckEnd())
	{
		return VS_STATE_END;
	}
	if (GetStateStartTime() +  m_fightUpdateTime <= time(NULL))
	{
		return VS_STATE_END;
	}
	return VS_STATE_DUMMY;
}

VsStateType ScenePk::CheckP321End()
{
	if(CheckEnd())
	{
		return VS_STATE_END;
	}
	return SceneVsBase::CheckP321End();
}

bool ScenePk::_IsGuildLadderTime()
{
	time_t newTime = time(NULL);
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	foreach (i in  GetGlobalConfig().GuildLadderTime)
	{
		if (i->seq[0] == weekNum)  // 星期几
		{
			time_t beginTime = XCommon::MakeTime(newTime, i->seq[1] / 100, i->seq[1] % 100, 0);
			time_t endTime = XCommon::MakeTime(newTime, i->seq[2] / 100, i->seq[2] % 100, 0);

			if (newTime >= beginTime && newTime < endTime)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

UINT32 ScenePk::GetGroup(UINT64 roleid)
{
	for(auto it = m_robots.begin(); it != m_robots.end(); ++it)
	{
		if((*it) == roleid)
		{
			return 0;
		}
	}
	
	if(!m_robots.empty())//有机器人
	{
		return 1;
	}

	UINT32 Index = 1;
	for(auto it = m_units.begin(); it != m_units.end(); ++it)
	{
		if(it->first == roleid)
		{
			return Index;
		}
		++Index;
	}
	
	return 1;
}

void ScenePk::UpdateBasePoint()
{
	//更新最新积分 待优化 *****
	SPkResult* tmp11 = NULL;
	SPkResult* tmp22 = NULL;
	for(auto it = m_units.begin(); it != m_units.end(); ++it)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(it->first);	
		if(pRole)
		{
			pRole->Get<CPkRecord>()->Update();
			(it->second).basepoint = pRole->Get<CPkRecord>()->GetPoint();
		}
		if(NULL == tmp11)
		{
			tmp11 = &(it->second);
		}
		else if(NULL == tmp22)
		{
			tmp22 = &(it->second);
		}
	}
	if(tmp11 && tmp22)
	{
		tmp22->other.pointspan = (tmp22->basepoint - tmp11->basepoint);
		tmp11->other.pointspan = (tmp11->basepoint - tmp22->basepoint);
	}
}

void ScenePk::TestPrint()
{
	for(auto it = m_units.begin(); it != m_units.end(); ++it)
	{
		SSDebug << __FUNCTION__ << " basepoint = " << (it->second).basepoint << " span = " << (it->second).other.pointspan << END;
	}
}

void ScenePk::GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded)
{
	for(auto it = m_units.begin(); it != m_units.end(); ++it)
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

std::string ScenePk::GetRoleName(UINT64 roleID)
{
	for(int i = 0; i < m_pkdata.roles_size(); ++i)
	{
		if(m_pkdata.roles(i).rolebrief().roleid() == roleID)
		{
			return m_pkdata.roles(i).rolebrief().rolename();
		}
	}
	return "";
}

void ScenePk::InitRobotInfo()
{
	if(1 != m_pkdata.roles_size() || 0 == m_pkdata.robotlookupid())
	{
		return ;
	}

	const KKSG::RoleSmallInfo& roleInfo = m_pkdata.roles(0).rolebrief();
	KKSG::RoleSmallInfo robotInfo;

	if(!AddRobot(roleInfo, m_pkdata.robotlookupid(), robotInfo))
	{
		return ;
	}

	KKSG::PkRoleInfo& robot = *m_pkdata.add_roles();
	*robot.mutable_rolebrief() = robotInfo;

	KKSG::PkRoleRec& robotrec = *robot.mutable_pkrec();

	UINT32 point = GetRobotPoint(m_pkdata.roles(0).pkrec().point());
	UINT32 win = XRandom::randInt(5, 100);
	UINT32 lose = XRandom::randInt(5, 100);
	UINT32 draw = XRandom::randInt(5, 100);
	robotrec.set_point(point);
	robotrec.set_win(win);
	robotrec.set_lose(lose);
	robotrec.set_draw(draw);
	for (int i = 0; i < 5; ++i)
	{
		UINT32 result = XRandom::randInt(KKSG::PkResultType_MIN, KKSG::PkResultType_MAX + 1);
		robotrec.add_records(result);
	}
}

void ScenePk::FillResult(KKSG::PkResult* result, Role* role)
{
	if(NULL == result || NULL == role)
	{
		return;
	}

	ScenePkBase::FillResult(result, role);

	SPkResult* tempresult = GetResult(role->GetID());

	if (NULL ==  tempresult)
	{
		;
	}
	else
	{
		foreach (iter in tempresult->items)
		{
			KKSG::ItemBrief* pData = result->add_items();
			pData->set_itemid(iter->first);
			pData->set_itemcount(iter->second);
		}
	}

	for(auto it = m_units.begin(); it != m_units.end(); ++it)
	{
		UINT64 roleID = (it->first);
		auto& tmp = (it->second);
		KKSG::KKVsRoleState ks = KKSG::KK_VS_ROLE_NORMAL;
		if(KKSG::KKVsRoleState_IsValid(tmp.state))
		{
			ks = KKSG::KKVsRoleState(tmp.state);
		}
		if(roleID == role->GetID())
		{
			result->set_mystate(ks);
		}
		else
		{
			result->set_opstate(ks);
		}
	}
}

UINT32 ScenePk::GetRolePoint(UINT64 roleID)
{
	for(int i = 0; i < m_pkdata.roles_size(); ++i)
	{
		const auto& roledata = m_pkdata.roles(i);
		if(roledata.rolebrief().roleid() == roleID)
		{
			return roledata.pkrec().point();
		}
	}
	return 0;
}

void ScenePk::FillRoleBrief(UINT64 roleID, KKSG::RoleBriefInfo& brief)
{
	for(int i = 0; i < m_pkdata.roles_size(); ++i)
	{
		const auto& roledata = m_pkdata.roles(i);
		if(roledata.rolebrief().roleid() == roleID)
		{
			const auto& tmp = roledata.rolebrief();
			brief.set_roleid(tmp.roleid());
			brief.set_type((KKSG::RoleType)tmp.roleprofession());
			brief.set_name(tmp.rolename());
			brief.set_level(tmp.rolelevel());
		}
	}
}

void ScenePk::FillUnitIDs(std::vector<UINT64>& unitIDs)
{
	for(auto it = m_units.begin(); it != m_units.end(); ++it)
	{
		unitIDs.push_back(it->first);
	}
}

