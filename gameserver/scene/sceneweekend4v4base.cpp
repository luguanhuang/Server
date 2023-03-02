#include "pch.h"
#include "sceneweekend4v4base.h"
#include "weekend4v4/ptcg2c_weekend4v4roledatantf.h"
#include "gamelogic/weekend4v4mgr.h"
#include "gamelogic/weekend4v4record.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "timeutil.h"
#include "scenevsbase.h"
#include "scenebase.h"
#include "scene.h"
#include "util/XRandom.h"
#include "util/gametime.h"
#include "unit/enemy.h"

SceneWeekEnd4v4Base::SceneWeekEnd4v4Base(Scene* scene, WEEKEND4V4_TYPEID wtypeid)
    :SceneVsBase(scene), m_typeid(wtypeid)
{
}

SceneWeekEnd4v4Base::~SceneWeekEnd4v4Base()
{
}

bool SceneWeekEnd4v4Base::Init(const KKSG::CreateBattleParam& roParam)
{
    //if(!WeekEnd4v4Mgr::Instance()->CheckSystemOpen())
    //{
    //    return false;
    //}
    InitState();
    const KKSG::KMatchFightData& roleData = roParam.kmatchfightdata();
    m_roleCount = roleData.roles_size()/2;
    if(0 == m_roleCount || roleData.roles_size() != m_roleCount*2)
    {
        LogError("SceneWeekEnd4v4 rolenum %d is error.\n", roleData.roles_size());
        return false;
    }
    for (int i = 0; i < roParam.kmatchfightdata().roles_size(); i++)
    {
        const KKSG::KMatchRole& role = roleData.roles(i);
        m_roleResult.push_back(SceneWeekEnd4v4Result(role.roleid()));
    }
    m_startTime = GameTime::GetTime();
    m_totalSeconds = 0;
    m_ratio = WeekEnd4v4Mgr::Instance()->GetScoreRatio(m_typeid);

    UINT32 tmp_sceneid = WeekEnd4v4Mgr::Instance()->GetWeekEndSceneID(m_typeid);
    if(0 != tmp_sceneid)
    {
        KillMonsterScoreTable& table = SceneConfig::Instance()->GetKillMonsterScoreTable();
        KillMonsterScoreTable::RowData * pdata = table.GetBysceneID(tmp_sceneid);
        if (NULL != pdata)
        {
            for (auto iter = pdata->monsterScore.begin(); iter != pdata->monsterScore.end(); ++iter)
            {
                m_monsterScore[iter->seq[0]] = iter->seq[1];
            }
        }
    }
    return true;
}

UINT64 SceneWeekEnd4v4Base::GetVoipRoomID(UINT64 nRoleID)
{
	for (size_t i = 0; i < m_roleResult.size(); i++)
	{
		if (nRoleID == m_roleResult[i].m_roleid)
		{
			if (i < m_roleResult.size()/2)
			{
				return m_roleResult[0].m_roleid;
			}else
			{
				return m_roleResult[m_roleResult.size()/2].m_roleid;
			}
		} 
	}
	return -1;
}

void SceneWeekEnd4v4Base::InitState()
{
    m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
    m_statemgr.GetCurrentState()->OnEnter(this);
}

UINT32 SceneWeekEnd4v4Base::GetGroup(UINT64 roleid)
{
    UINT32 index = GetRoleIndex(roleid);
    if((UINT32)-1 == index)
    {
        return 0;
    }
    return (index < m_roleCount)?1:2;
}

UINT32 SceneWeekEnd4v4Base::GetRoleIndex(UINT64 roleid)
{
    for(UINT32 i = 0; i < m_roleResult.size(); ++i)
    {
        if(m_roleResult[i].m_roleid == roleid)
        {
            return i;
        }
    }
    return (UINT32)-1;
}

void SceneWeekEnd4v4Base::SetRoleBaseInfo(Role* pRole)
{
    if(NULL == pRole)
    {
        return;
    }
    UINT64 roleid = pRole->GetRoleID();
    UINT32 index = GetRoleIndex(roleid);
    if((UINT32)-1 == index)
    {
        return;
    }
    m_roleResult[index].m_rolename = pRole->GetName();
    m_roleResult[index].m_level = pRole->GetLevel();
    m_roleResult[index].m_profession = pRole->GetProfession();
}

VsStateType SceneWeekEnd4v4Base::IsAllLoad()
{
    if (m_loadmgr.GetLoadedRoleCount() > 0)
    {
        return VS_STATE_IN_FIGHT;
    }
    return VS_STATE_DUMMY;
}

VsStateType SceneWeekEnd4v4Base::OnLoadTimeOut()
{
    return VS_STATE_IN_FIGHT;
}

VsStateType SceneWeekEnd4v4Base::CheckFightEnd()
{
    UINT32 redScore = 0;
    UINT32 blueScore = 0;
    GetTeamScore(redScore, blueScore);
    UINT32 goalScore = WeekEnd4v4Mgr::Instance()->GetGoalScore(m_typeid);
    if(goalScore > 0 && (redScore >= goalScore || blueScore >= goalScore ))
    {
        return VS_STATE_END;
    }

    UINT32 maxSeconds = WeekEnd4v4Mgr::Instance()->GetFullTime(m_typeid);
    if(maxSeconds > 0 && GameTime::GetTime() > m_startTime + maxSeconds)
    {
        return VS_STATE_END;
    }
    return VS_STATE_DUMMY;
}

void SceneWeekEnd4v4Base::OnStateEnd()
{
    m_totalSeconds = (UINT32)(GameTime::GetTime() - m_startTime);
    UINT32 maxSeconds = WeekEnd4v4Mgr::Instance()->GetFullTime(m_typeid);
    if(m_totalSeconds > maxSeconds)
    {
        m_totalSeconds = maxSeconds;
    }
    GetScene()->SetDontDestory(false);
    SetEnd();
}

void SceneWeekEnd4v4Base::OnRoleReconnect(Role* pRole)
{
    if (NULL == pRole)
    {
        return;
    }
    LogInfo("role [%llu] reconnected in WeekEnd4v4", pRole->GetID());
    WKSynAllRolesData(pRole, true);
}

void SceneWeekEnd4v4Base::FillBattleRoleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, bool isAllInfo)
{
    if(index >= m_roleResult.size())
    {
        LogError("Vector index out of bounds. %d : %d\n", index, m_roleResult.size());
        return;
    }
    const SceneWeekEnd4v4Result& roledata = m_roleResult[index];
    pProtocData->set_roleid(roledata.m_roleid);
    pProtocData->set_isline(NULL != GetRole(roledata.m_roleid));

    if(isAllInfo)
    {
        pProtocData->set_rolename(roledata.m_rolename);
        pProtocData->set_rolelevel(roledata.m_level);
        pProtocData->set_profession(roledata.m_profession);
        INT64 tmpSeconds = (INT64)(m_startTime + WeekEnd4v4Mgr::Instance()->GetFullTime(m_typeid) - GameTime::GetTime());
        if(tmpSeconds<0)tmpSeconds=0;
        pProtocData->set_timeseconds((UINT32)tmpSeconds);
    }

    pProtocData->set_redblue(GetGroup(roledata.m_roleid));
    FillBattleData(pProtocData, index, roledata);
}

void SceneWeekEnd4v4Base::FillRoleResultData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT64 roleid)
{
    FillBattleRoleData(pProtocData, GetRoleIndex(roleid), true);
}

void SceneWeekEnd4v4Base::FillBattleAllRolesData(KKSG::WeekEnd4v4BattleAllRoleData& pProtocData, bool isAllInfo)
{
    for(UINT32 i = 0; i < m_roleResult.size(); ++i)
    {
        if(0 == m_roleResult[i].m_roleid)
        {
            continue;
        }
        FillBattleRoleData(pProtocData.add_roledata(), i, isAllInfo);
    }
}

void SceneWeekEnd4v4Base::WKBoardcastRoleData(UINT64 roleid, bool isAllInfo)
{
    PtcG2C_WeekEnd4v4RoleDataNtf ptc;
    FillBattleRoleData(ptc.m_Data.add_roledata(), GetRoleIndex(roleid), isAllInfo);
    GetScene()->Broadcast(ptc);
}

void SceneWeekEnd4v4Base::WKSynAllRolesData(Role* pRole, bool isAllInfo)
{
    PtcG2C_WeekEnd4v4RoleDataNtf ptc;
    FillBattleAllRolesData(ptc.m_Data, isAllInfo);
    pRole->Send(ptc);
}

void SceneWeekEnd4v4Base::WKBoardcastAllRolesData(bool isAllInfo)
{
    PtcG2C_WeekEnd4v4RoleDataNtf ptc;
    FillBattleAllRolesData(ptc.m_Data, isAllInfo);
    GetScene()->Broadcast(ptc);
}

UINT32 SceneWeekEnd4v4Base::GetTeamSeconds()
{
    return m_totalSeconds;
}

void SceneWeekEnd4v4Base::AddItemList(KKSG::WeekEnd4v4TeamResult* teamResult, std::vector<Role*>& roRoles, UINT32 redScore, UINT32 blueScore)
{
    //Æ½¾ÖËãË«Êä
    bool redwin = (redScore>blueScore?true:false);
    bool bluewin = (blueScore>redScore?true:false);
    for(auto itor = roRoles.begin(); itor != roRoles.end(); ++itor)
    {
        Role* pRole = *itor;
        if(NULL == pRole)
        {
            continue;
        }
        UINT32 groupid = GetGroup(pRole->GetRoleID());
        bool hasAddItem = false;
        if(1 == groupid)
        {
            hasAddItem = WeekEnd4v4Mgr::Instance()->AddItem(pRole, m_typeid, redwin);
        }else if(2 == groupid){
            hasAddItem = WeekEnd4v4Mgr::Instance()->AddItem(pRole, m_typeid, bluewin);
        }
        if(hasAddItem)
        {
            teamResult->add_hasrewardsid(pRole->GetRoleID());
        }
    }
}

bool SceneWeekEnd4v4Base::IsWin(Role* pRole, UINT32 redScore, UINT32 blueScore)
{
    if(NULL == pRole)
    {
        return false;
    }
    UINT32 groupid = GetGroup(pRole->GetRoleID());
    if(1 == groupid)
    {
        return redScore>blueScore?true:false;
    }else if(2 == groupid){
        return blueScore>redScore?true:false;
    }
    return false;
}

void SceneWeekEnd4v4Base::PushReviveList(UINT64 roleid)
{
    UINT32 relifetime = WeekEnd4v4Mgr::Instance()->GetReviveSeconds(m_typeid);
    if(0 == relifetime)
    {
        return;
    }
    m_deathList.push_back(SceneWeekEnd4v4ReviveInfo(roleid, GameTime::GetTime()+relifetime));
}

void SceneWeekEnd4v4Base::AddBuffUnbeatable(Role* pRole)
{
    if(NULL == pRole)
    {
        return;
    }
    XAddBuffActionArgs args;
    args.vecBuffs.push_back(BuffDesc(4005, 1));
    args.Caster = pRole->GetRoleID();
    args.EffectImm = true;
    pRole->GetXObject()->DispatchAction(&args);
}

void SceneWeekEnd4v4Base::AddBuffChangeUnbeatable(Role* pRole, UINT32 buffID)
{
    if(NULL == pRole)
    {
        return;
    }
    XAddBuffActionArgs args;
    args.vecBuffs.push_back(BuffDesc(buffID, 1));
    args.vecBuffs.push_back(BuffDesc(4005, 1));
    args.Caster = pRole->GetRoleID();
    args.EffectImm = true;
    pRole->GetXObject()->DispatchAction(&args);
}

bool SceneWeekEnd4v4Base::ResultCountOrSeconds()
{
    return true;
}

void SceneWeekEnd4v4Base::ReviviRoleList()
{
    time_t nowtime = GameTime::GetTime();
    for(auto itor = m_deathList.begin(); itor != m_deathList.end();)
    {
        if(nowtime >= itor->m_reviveTime)
        {
            ReviveRole(itor->m_roleid);
            m_deathList.erase(itor++);
        }
        break;
    }
}

bool SceneWeekEnd4v4Base::OnEvent(const VsEvent& event)
{
    UINT32 roleindex = GetRoleIndex(event.roleid);
    if (-1 == roleindex)
    {
        LogError("Can't find role [%llu] in WeekEnd4v4.", event.roleid);
        return false;
    }
    Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
    if (NULL == pRole)
    {
        return false;
    }

    switch (event.type)
    {
    case VS_EVENT_ROLE_LOADED:
        {
            SetRoleBaseInfo(pRole);
            m_loadmgr.AddRole(event.roleid);
            InitRole(pRole);
            pRole->Get<WeekEnd4v4Record>()->IncCount(m_typeid);
            WKSynAllRolesData(pRole, true);
            WKBoardcastRoleData(event.roleid, true);
            LogDebug("role [%llu] is loaded in WeekEnd4v4.", event.roleid);
            break;
        }
    case VS_EVENT_ROLE_QUIT:
        {
            OnEventRoleQuit(pRole);
            m_loadmgr.DelRole(event.roleid);
            if (m_loadmgr.GetLoadedRoleCount() == 0)
            {
                SetState(VS_STATE_END);
            }
            break;
        }
    case VS_EVENT_ROLE_DIE:
        {
            OnEventRoleDie(pRole);
            break;
        }
    default:
        break;
    }

    return true;
}

void SceneWeekEnd4v4Base::Update()
{
    m_statemgr.Update();
    ReviviRoleList();
}

void SceneWeekEnd4v4Base::AddScore(UINT64 roleid, UINT32 addscore)
{
    UINT32 index = GetRoleIndex(roleid);
    if((UINT32)-1 == index)
    {
        return;
    }
    m_roleResult[index].IncCount(addscore);
}

UINT32 SceneWeekEnd4v4Base::GetScore(UINT32 count)
{
    return m_ratio * count;
}

UINT32 SceneWeekEnd4v4Base::GetMonsterScore(UINT32 monsterid)
{
    auto itor = m_monsterScore.find(monsterid);
    if(m_monsterScore.end() == itor)
    {
        return 0;
    }
    return itor->second;
}

void SceneWeekEnd4v4Base::GetTeamScore(UINT32& redScore, UINT32& blueScore)
{
    for(UINT32 i = 0; i< m_roleCount; i++)
    {
        redScore += GetScore(m_roleResult[i].GetCount());
        blueScore += GetScore(m_roleResult[i+m_roleCount].GetCount());
    }
}
