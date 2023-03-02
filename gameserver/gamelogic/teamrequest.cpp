#include "pch.h"
#include "teamrequest.h"
#include "teammgr.h"
#include "team.h"
#include "team/ptcg2c_leaveteam.h"
#include "team/ptcg2c_teamfulldatantf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "table/expeditionconfigmgr.h"
#include "team.h"
#include "teammember.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include <time.h>
#include "globalconfig.h"
#include "stagemgr.h"
#include "teaminvitemgr.h"
#include "foreach.h"
#include "scene/scene.h"
#include "teamhandler.h"
#include "team/ptcg2c_startbattlefailedntf.h"
#include "scene/enterscenecondition.h"
#include "gamelogic/bagtransition.h"
#include "teamrecord.h"
#include "scene/sceneteam.h"
#include "levelsealMgr.h"


TeamRequest::TeamRequest(Role *pRole, TeamCopy* pTeam/* = NULL*/)
{
	m_pRole = pRole;
	if(pTeam)
	{
		m_teaminfo = (*pTeam);
	}
}

TeamRequest::~TeamRequest()
{

}

int TeamRequest::CreateExpenditionTeam(UINT32 expID)
{
	int ret = CheckOpenCondtion(m_pRole, expID);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::JoinTeam()
{
	SSDebug << __FUNCTION__ << " " << m_pRole->GetID() << " " << m_pRole->GetName() << END;
	if(0 == m_teaminfo.GetTeamID())
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	int ret = CheckJoinCondtion(m_pRole);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::StartBattleBeginVote(KKSG::TeamOPRes &roRes)
{
	if(0 == m_teaminfo.GetTeamID())
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	UINT64 proRoleID = 0;
	int ret = CheckStartBattleCondition(proRoleID);
	if(0 != ret)
	{
		if(proRoleID)
		{
			PtcG2C_StartBattleFailedNtf failedNtf;
			failedNtf.m_Data.set_prouserid(proRoleID);
			failedNtf.m_Data.set_reason((KKSG::ErrorCode)ret);
			m_teaminfo.BroadCast(failedNtf, m_pRole->GetID());

			roRes.set_problem_roleid(proRoleID);
		}
		return ret;
	}

	return ret;
}

int TeamRequest::ChangeExpID(int expID, KKSG::TeamOPRes& roRes, KKSG::TeamHelper& nothelpers)
{
	if(0 == m_teaminfo.GetTeamID())
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	UINT64 problemID = 0;
	
	int ret = CheckChangeExpCondition(expID, problemID);

	roRes.set_problem_roleid(problemID);

	if(KKSG::ERR_SUCCESS == ret)
	{
		const auto& members = m_teaminfo.GetMember();
		for(size_t i = 0; i < members.size(); ++i)
		{
			if(members[i].IsHelper())
			{
				UINT64 roleID = members[i].RoleID();
				Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
				if(NULL == pRole)
				{
					nothelpers.add_roles(roleID);
					continue;
				}
				TeamRequest Tq(pRole);
				if(KKSG::ERR_SUCCESS != Tq.SwitchHelper(expID))
				{
					nothelpers.add_roles(roleID);
				}
			}
		}
	}

	return ret;
}

int TeamRequest::Invite(UINT64 roleID)
{
	if(0 == m_teaminfo.GetTeamID())
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	
	if(!m_pRole->IsFamiliar(roleID))
	{
		TeamInviteMgr::Instance()->AddInvToHistory(m_pRole->GetID(), roleID);
	}

	Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
	if(NULL == pRole)//有可能在其它服
	{
		return KKSG::ERR_TEAM_INVITE_ROLE_IS_IN_BATTLE;
	}

	int ret = CheckJoinCondtion(pRole);

	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}


int TeamRequest::StartMatch(int expid)
{
	int ret = CheckEnterCondtion(m_pRole, expid);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}

void TeamRequest::BuyTeamCount(int type, KKSG::BuyTeamSceneCountRet& roRes)
{
	ITeamTypeHandlerBase* handler = ITeamTypeHandlerBase::GetTeamHanderByType(type);
	if(handler)
	{
		handler->BuyCount(m_pRole, roRes);
	}
}

int TeamRequest::CheckJoinCondtion(Role *pRole)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	///> 队伍战力限制
	if (m_teaminfo.IsPPTLimit((UINT32)pRole->GetAttr(TOTAL_POWERPOINT)))
	{
		return KKSG::ERR_TEAM_PPTLIMIT;
	}
	return CheckOpenCondtion(pRole, m_teaminfo.GetExpID());	
}

int TeamRequest::CheckOpenCondtion(Role *pRole, int expid)
{
	if(NULL == pRole || 0 == expid)
	{
		return KKSG::ERR_FAILED;
	}

	ExpeditionTable::RowData* pRow = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expid);
	if(NULL == pRow)
	{
		return KKSG::ERR_TEAM_EXPEDITIONID_NOT_EXIST;
	}

	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pRow->Type);
	if(pHandler)
	{
		return pHandler->CheckOpenCondition(pRole , pRow);
	}
	else
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::CheckEnterCondtion(Role *pRole, int expid, TeamEnterData* pEnterData)
{
	if(NULL == pRole || 0 == expid || NULL == pEnterData)
	{
		return KKSG::ERR_FAILED;
	}

	ExpeditionTable::RowData* pRow = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expid);
	if(NULL == pRow)
	{
		return KKSG::ERR_TEAM_EXPEDITIONID_NOT_EXIST;
	}

	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pRow->Type);
	if(pHandler)
	{
		return pHandler->CheckEnterCondtion(pRole , pRow, pEnterData);
	}
	else
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::CheckStartBattleCondition(UINT64& problemRoleID)
{
	if(0 == m_teaminfo.GetTeamID())
	{
		return KKSG::ERR_FAILED;
	}
	ExpeditionTable::RowData* pConf = m_teaminfo.GetConf();
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pConf->Type);
	if(pHandler)
	{
		return pHandler->CheckBattleStartCondition(&m_teaminfo, problemRoleID);
	}
	else
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::CheckChangeExpCondition(int expID, UINT64& problemRoleID)
{
	if(0 == m_teaminfo.GetTeamID())
	{
		return KKSG::ERR_FAILED;
	}
	ExpeditionTable::RowData* pConf = m_teaminfo.GetConf();
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pConf->Type);
	if(pHandler)
	{
		return pHandler->ChangeExpIDBy(&m_teaminfo, expID, problemRoleID);
	}
	else
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::GetDayCountLeft(Role *pRole, int Type)
{
	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(Type);
	if(pHandler)
	{
		return pHandler->GetDayCountLeft(pRole);
	}
	return 0;
}

int TeamRequest::SwitchHelper(int expID)
{
	auto pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pConf->Type);
	if(pHandler)
	{
		return pHandler->CheckHelperCondition(m_pRole, pConf);
	}
	else
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::HandleTeamCost(UINT32 index)
{
	///> 取消赏金
	if (TeamCostInvalidID == index)
	{
		m_pRole->Get<CTeamRecord>()->RecoverTeamCostInfo();
		return KKSG::ERR_SUCCESS;
	}
	///> 设置赏金
	UINT32 expid = m_teaminfo.GetExpID();
	// check invalid
	const Sequence<UINT32, 3>* ret = ExpeditionConfigMgr::Instance()->GetTeamCost(expid, index);
	if (NULL == ret)
	{
		return KKSG::ERR_UNKNOWN;
	}

	// mark
	if(1 != ret->seq[0])//不是钻石团
	{
		BagTakeItemTransition take(m_pRole);
		take.SetReason(ItemFlow_TeamCost, ItemFlow_TeamCostTake);
		if (!take.TakeItem(ret->seq[1], ret->seq[2]))
		{
			take.RollBack();
			return KKSG::ERR_TEAMCOST_DRAGON;
		}
		take.NotifyClient();
	}

	m_pRole->Get<CTeamRecord>()->RecoverTeamCostInfo(TCRR_RESET);
	m_pRole->Get<CTeamRecord>()->MarkTeamCostInfo(expid, index);

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::GetTeamTypeCount(KKSG::TeamOPRes& roRes)
{
    for(int i = 1; i < TEAM_TYPE_MAX; ++i)
    {
        int type = i;
        auto pConf = ExpeditionConfigMgr::Instance()->GetTeamType2ExpConf(type);
        if(NULL == pConf)//找不到很正常
        {
            continue;
        }
        if(TEAM_CCT_NONE == pConf->CostCountType)
        {
            continue;
        }

        CTeamRecord* trinfo = m_pRole->Get<CTeamRecord>();
        int maxCount = trinfo->GetTeamMaxCountToday(type);
        int buyCount = trinfo->GetTeamBuyCountToday(type);
        int leftCount = GetDayCountLeft(m_pRole, type);
		SSDebug << " type = " << type << " max = " << maxCount << " left = " << leftCount << END;

        KKSG::TeamCountClient& data = *roRes.add_teamcount();
		data.set_teamtype(type);
        data.set_maxcount(maxCount);
        data.set_buycount(buyCount);
        data.set_leftcount(leftCount);
    }    
    return KKSG::ERR_SUCCESS;
}

int TeamRequest::BattleContinue(KKSG::TeamOPM2GRes &m2gRes)
{
	if(0 == m_teaminfo.GetTeamID())
	{
		return KKSG::ERR_FAILED;
	}

	if(m_teaminfo.GetLeaderID() != m_pRole->GetID())
	{
		return KKSG::ERR_FAILED;
	}

	Scene* pScene = m_pRole->GetCurrScene();
	if(NULL == pScene)
	{
		return KKSG::ERR_FAILED;
	}

	if(SCENE_WIN != pScene->GetSceneState())
	{
		return KKSG::ERR_FAILED;
	}

	SceneTeam* pSceneTeam = pScene->GetSceneTeam();
	if(NULL == pSceneTeam)
	{
		return KKSG::ERR_FAILED;
	}

	if(!pSceneTeam->IsGoddessOrEndlessA())
	{
		return KKSG::ERR_FAILED;
	}
	
	CtCsInfo info;
	info.leaderID = m_pRole->GetID();
	int ret = pSceneTeam->GotoNextScene(info);		
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	for(auto it = info.beHelper.begin(); it != info.beHelper.end(); ++it)
	{
		m2gRes.mutable_teamhelper()->add_roles(*it);
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::SwitchUseTicket()
{
	if(0 == m_teaminfo.GetTeamID())
	{
		return KKSG::ERR_FAILED;
	}
	ExpeditionTable::RowData* pConf = m_teaminfo.GetConf();
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pConf->Type);
	if(pHandler)
	{
		return pHandler->CheckUseTicketCondition(m_pRole, pConf);
	}
	else
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}
