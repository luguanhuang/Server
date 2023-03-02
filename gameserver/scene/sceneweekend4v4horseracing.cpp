#include "pch.h"
#include "sceneweekend4v4horseracing.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "timeutil.h"
#include "scenevsbase.h"
#include "scenebase.h"
#include "scene.h"
#include "util/XRandom.h"
#include "util/gametime.h"
#include "weekend4v4/ptcg2c_weekend4v4roledatantf.h"
#include "gamelogic/weekend4v4mgr.h"
#include "gamelogic/weekend4v4record.h"
#include "battleresultlistener.h"

SceneWeekEnd4v4HorseRacing::SceneWeekEnd4v4HorseRacing(Scene* scene)
    :SceneHorseRace(scene), m_typeid(WEEKEND4V4_TYPEID_HORSERACING)
{
    m_nEndTime = GameTime::GetTime() + WeekEnd4v4Mgr::Instance()->GetFullTime(m_typeid);
}

SceneWeekEnd4v4HorseRacing::~SceneWeekEnd4v4HorseRacing()
{
}

bool SceneWeekEnd4v4HorseRacing::Init(const KKSG::CreateBattleParam& roParam)
{
    //if(!WeekEnd4v4Mgr::Instance()->CheckSystemOpen())
    //{
    //    return false;
    //}
    InitState();
    const KKSG::KMatchFightData& roleData = roParam.kmatchfightdata();
    m_wkRoleCount = roleData.roles_size()/2;
    if(0 == m_wkRoleCount || roleData.roles_size() != m_wkRoleCount*2)
    {
        LogError("SceneWeekEnd4v4HorseRacing rolenum %d is error.\n", roleData.roles_size());
        return false;
    }
    for (int i = 0; i < roParam.kmatchfightdata().roles_size(); i++)
    {
        const KKSG::KMatchRole& role = roleData.roles(i);
        m_unitids.push_back(role.roleid());
    }
    m_wkStartTime = GameTime::GetTime();
    m_wkUpdateTime = m_wkStartTime;
    return true;
}


UINT64 SceneWeekEnd4v4HorseRacing::GetVoipRoomID(UINT64 nRoleID)
{
	for (size_t i = 0; i < m_unitids.size(); i++)
	{
		if (nRoleID == m_unitids[i])
		{
			if (i < m_unitids.size()/2)
			{
				return m_unitids[0];
			}else
			{
				return m_unitids[m_unitids.size()/2];
			}
		}
	}
	return -1;
}




UINT32 SceneWeekEnd4v4HorseRacing::GetGroup(UINT64 roleid)
{
    UINT32 index = GetRoleIndex(roleid);
    if((UINT32)-1 == index)
    {
        return 0;
    }
    return (index<m_wkRoleCount)?1:2;
}

void SceneWeekEnd4v4HorseRacing::WKRoleEnter(Role* pRole)
{
    if(NULL == pRole)
    {
        return;
    }
    pRole->Get<WeekEnd4v4Record>()->IncCount(m_typeid);
    WKSynAllRolesData(pRole, true);
    WKBoardcastRoleData(pRole->GetRoleID(), true);
}

void SceneWeekEnd4v4HorseRacing::AddItemList(KKSG::WeekEnd4v4TeamResult* teamResult, std::vector<Role*>& roRoles, UINT32 redScore, UINT32 blueScore)
{
    //平局算双输
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

bool SceneWeekEnd4v4HorseRacing::IsWin(Role* pRole, UINT32 redScore, UINT32 blueScore)
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

UINT32 SceneWeekEnd4v4HorseRacing::GetRoleIndex(UINT64 roleid)
{
    for(UINT32 i = 0; i < m_unitids.size(); ++i)
    {
        if(m_unitids[i] == roleid)
        {
            return i;
        }
    }
    return (UINT32)-1;
}

void SceneWeekEnd4v4HorseRacing::GetTeamScore(UINT32& redTotalScore, UINT32& blueTotalScore)
{
    redTotalScore = blueTotalScore = 0;
    std::vector<UINT64> roleidSorted;
    GetRank(roleidSorted);
    for(UINT32 i = 0; i<roleidSorted.size(); ++i)
    {
        UINT32 tmpScore = WeekEnd4v4Mgr::Instance()->GetHorseRacingRankPoint(i);
        UINT32 groupid = GetGroup(roleidSorted[i]);
        if(1 == groupid)
        {
            redTotalScore += tmpScore;
        }else if(2 == groupid){
            blueTotalScore += tmpScore;
        }
    }
}

UINT32 SceneWeekEnd4v4HorseRacing::GetTeamSeconds()
{
    return (UINT32)(GameTime::GetTime() - m_wkStartTime);
}

void SceneWeekEnd4v4HorseRacing::FillResult(NormalBattleResultListener& listener)
{
    KKSG::PvpRoleInfo info;
    for(auto itor = m_vecRoleRaceInfo.begin();itor != m_vecRoleRaceInfo.end(); ++itor)
    {
        info.set_roleid(itor->nRoleID);
        listener.SetInitStatus(info);
        FillBattleRoleData(listener.GetWeekEnd4v4RoleResult(), itor->nRoleID, true);
    }
}

void SceneWeekEnd4v4HorseRacing::WKRankChangeOrFinal(UINT32 rankChangeOrFinal)
{
    if(0 == rankChangeOrFinal)
    {
        return;
    }
    WKBoardcastAllRolesData(false);
}

void SceneWeekEnd4v4HorseRacing::OnRoleReconnect(Role* pRole)
{
    if (NULL == pRole)
    {
        return;
    }
    LogInfo("role [%llu] reconnected in WeekEnd4v4", pRole->GetID());
    WKSynAllRolesData(pRole, true);
}

void SceneWeekEnd4v4HorseRacing::FillBattleRoleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT64 roleid, bool isAllInfo)
{
    UINT32 index = (UINT32)-1;
    for(UINT32 i = 0; i<m_vecRoleRaceInfo.size(); i++)
    {
        if(m_vecRoleRaceInfo[i].nRoleID == roleid)
        {
            index = i;
            break;
        }
    }
    if(index >= m_vecRoleRaceInfo.size())
    {
        LogError("Vector index out of bounds. %d : %d\n", index, m_vecRoleRaceInfo.size());
        return;
    }
    const RoleRaceInfo& roledata = m_vecRoleRaceInfo[index];
    pProtocData->set_roleid(roledata.nRoleID);
    pProtocData->set_isline(NULL != GetRole(roledata.nRoleID));

    if(isAllInfo)
    {
        pProtocData->set_rolename(roledata.szName);
        pProtocData->set_rolelevel(roledata.nLv);
        pProtocData->set_profession(roledata.nJob);
        pProtocData->set_timeseconds(0);        //赛马无倒计时
    }

    pProtocData->set_redblue(GetGroup(roledata.nRoleID));
    UINT32 tmpRank = index+1;
    pProtocData->set_rank(tmpRank);
    pProtocData->set_score(WeekEnd4v4Mgr::Instance()->GetHorseRacingRankPoint(tmpRank-1));
}

void SceneWeekEnd4v4HorseRacing::FillBattleAllRolesData(KKSG::WeekEnd4v4BattleAllRoleData& pProtocData, bool isAllInfo)
{
    for(UINT32 i = 0; i < m_vecRoleRaceInfo.size(); ++i)
    {
        if(0 == m_vecRoleRaceInfo[i].nRoleID)
        {
            continue;
        }
        FillBattleRoleData(pProtocData.add_roledata(), m_vecRoleRaceInfo[i].nRoleID, isAllInfo);
    }
}

void SceneWeekEnd4v4HorseRacing::WKBoardcastRoleData(UINT64 roleid, bool isAllInfo)
{
    PtcG2C_WeekEnd4v4RoleDataNtf ptc;
    FillBattleRoleData(ptc.m_Data.add_roledata(), roleid, isAllInfo);
    GetScene()->Broadcast(ptc);
}

void SceneWeekEnd4v4HorseRacing::WKSynAllRolesData(Role* pRole, bool isAllInfo)
{
    PtcG2C_WeekEnd4v4RoleDataNtf ptc;
    FillBattleAllRolesData(ptc.m_Data, isAllInfo);
    pRole->Send(ptc);
}

void SceneWeekEnd4v4HorseRacing::WKBoardcastAllRolesData(bool isAllInfo)
{
    PtcG2C_WeekEnd4v4RoleDataNtf ptc;
    FillBattleAllRolesData(ptc.m_Data, isAllInfo);
    GetScene()->Broadcast(ptc);
}
