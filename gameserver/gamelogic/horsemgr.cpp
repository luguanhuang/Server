#include "pch.h"
#include "horsemgr.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "noticemgr.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "define/systemiddef.h"
#include "pvprecord.h"
#include "activity/ptcg2c_halliconsntf.h"
#include "MultActivityMgr.h"
#include "gamelogic/stagemgr.h"
#include "util/gametime.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/sceneswitch.h"
#include "unit/pet.h"
#include "gamelogic/notice.h"
#include "horse/ptcg2c_horsefailtipsntf.h"

INSTANCE_SINGLETON(HorseRideMgr);


HorseRideMgr::HorseRideMgr(){}

HorseRideMgr::~HorseRideMgr(){}

bool HorseRideMgr::Init()
{
	m_nStep = eHorseRaceIdle;
	m_Icon.nState		= eStop;
	m_Icon.nTime		= 0;
	m_Icon.nSystemID	= SYS_HORSE;
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	m_nTime = 0;
	m_bOpen = false;
	m_bEnter = false;
	m_nNoticeTime = 0;
	m_nFailTipsTime = 0;
	m_mapAward.clear();


	m_awardTable.LoadFile("table/HorseRace.txt");
	for (UINT32 i = 0; i < m_awardTable.Table.size(); i++)
	{
		HorseRace::RowData *pRowData = m_awardTable.Table[i];
		m_mapAward[pRowData->sceneid].push_back(pRowData);
	}
	if (!m_horseTable.LoadFile("table/Horse.txt"))
	{
		SSWarn<<"load table/Horse.txt failed"<<END;
		return false;
	}

	return true;
}

void HorseRideMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}


void HorseRideMgr::SendNotice(UINT32 nMin)
{
	auto itend = RoleManager::Instance()->End();
	for(auto iter = RoleManager::Instance()->Begin(); iter != itend; ++iter)
	{
		Role* pRole = *iter;
		if (pRole->GetLevel() < GetGlobalConfig().HorseLevelLimit)
			continue;	
		StageMgr* pStage = pRole->Get<StageMgr>();
		if (pStage->CanPlayCount(GetGlobalConfig().HorseSceneID))
		{
			Notice notice(HorseRaceCountTime);
			notice.Replace("$C",ToString(nMin));
			notice.SendSelf(pRole->GetID());
		}
	}
}


void HorseRideMgr::CheckOpenNotice(UINT32 nSystemID,UINT32 nState)
{
	auto itend = RoleManager::Instance()->End();
	for(auto it = RoleManager::Instance()->Begin(); it != itend; ++it)
	{
		HallIconNoticeCheck(*it, nSystemID,nState);
	}
}

void HorseRideMgr::HallIconNoticeCheck(Role* pRole,UINT32 nSystemID,UINT32 nState)
{
	if(!pRole)
		return ;
	if (pRole->GetLevel() < GetGlobalConfig().HorseLevelLimit)
	{
		return;	
	}
	StageMgr* pStage = pRole->Get<StageMgr>();
	if (nState==KKSG::HICONS_END)
	{
		PtcG2C_HallIconSNtf ptc;
		ptc.m_Data.set_systemid(nSystemID);
		ptc.m_Data.set_state((KKSG::HallIconState)nState);
		pRole->Send(ptc);
		return;
	}else if (nState==KKSG::HICONS_BEGIN)
	{
		if (pStage->CanPlayCount(GetGlobalConfig().HorseSceneID))
		{
			PtcG2C_HallIconSNtf ptc;
			ptc.m_Data.set_systemid(nSystemID);
			ptc.m_Data.set_state((KKSG::HallIconState)nState);
			pRole->Send(ptc);
		}
	}
}

void HorseRideMgr::Open()
{
	m_Icon.nState  = eRuning;
	CheckOpenNotice(m_Icon.nSystemID, KKSG::HICONS_BEGIN);
	m_bOpen = true;
}

bool HorseRideMgr::IsOpen()
{
	return m_bEnter;
}

void HorseRideMgr::SetState(bool bState)
{
	m_bOpen = bState;
}

void HorseRideMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	m_nTime   = GameTime::GetTime();
	switch (m_nStep)
	{
	case eHorseRaceIdle:
		{
			UINT32 nOpenTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_HORSE);
			if (m_bOpen)
			{
				m_bEnter = true;
				if (m_nTime>=nOpenTime+GetGlobalConfig().HorseWaitTime)
				{
					m_Icon.nState = eStop;
					CheckOpenNotice(m_Icon.nSystemID, KKSG::HICONS_END);
					m_nStep = eHorseRaceMatch;
					m_bEnter = false;
				}else
				{
					if (m_nTime >= m_nNoticeTime)
					{
						m_nNoticeTime = m_nTime + 60;
						int nMin = (m_nTime - nOpenTime)/60;
						if (nMin)
						{
							SendNotice(nMin);
						}				
					}

				}

			}
		}
		break;
	case eHorseRaceMatch:
		{
			HorseMatch();
			m_nStep = eHorseRaceEnter;
		}
		break;
	case eHorseRaceEnter:
		{
			LogInfo("---eHorseRaceEnter");
			bool bRet = Enter();
			if (bRet==true)
			{
				m_nFailTipsTime = GameTime::GetTime()+5;
				m_nStep = eHorseRaceFailNotice;
			}
		}
		break;
	case eHorseRaceFailNotice:
		{
			if (m_nTime>=m_nFailTipsTime)
			{
				SendFailTips();
				m_nStep = eHorseRaceEnd;
			}
		}
		break;
	case eHorseRaceEnd:
		{
			LogDebug("---eHorseRaceEnd");
			Destory();
			m_nStep = eHorseRaceIdle;

		}
		break;
	default:
		{}
	}
}


void HorseRideMgr::SendFailTips()
{
	Scene *pScene = SceneManager::Instance()->FindStaticScene(GetGlobalConfig().HorseWaitSceneID);
	if (!pScene)
		return;
	const std::list<Role *>& vecRole = pScene->GetAllRoles();
	UINT32 nGroup = 0;
	for (auto iter = vecRole.begin(); iter!=vecRole.end(); iter++)
	{
		Role *pRole = *iter;
		if (pRole)
		{
			LogInfo("HorseFailTips uid=%llu", pRole->GetID());
			PtcG2C_HorseFailTipsNtf ntf;
			pRole->Send(ntf);
		}
	}
}


void HorseRideMgr::HorseMatch()
{
	Scene *pScene = SceneManager::Instance()->FindStaticScene(GetGlobalConfig().HorseWaitSceneID);
	if (!pScene)
	{
		return;
	}
	const std::list<Role *>& vecRole = pScene->GetAllRoles();
	UINT32 nGroup = 0;
	for (auto iter = vecRole.begin(); iter!=vecRole.end(); iter++)
	{
		Role *pRole = *iter;
		if (pRole)
		{
			LogInfo("HorseMatch uid=%llu", pRole->GetID());
			m_RoleIDs.push_back(pRole->GetID());
		}
	}
}

void HorseRideMgr::ChangeScene(CSceneSwitch::RoleList &rolelist)
{
	KKSG::SceneSwitchData oData;
	KKSG::HorseSwitchData *pData = oData.mutable_horsedata();
	for (auto iter = rolelist.begin(); iter!=rolelist.end();iter++)
	{
		LogInfo("insert roleid=%llu", (*iter)->GetID());
		pData->add_roleid((*iter)->GetID());
	}
	bool bRet = CSceneSwitch::ChangeScene(rolelist,GetGlobalConfig().HorseSceneID, oData);
	if (bRet)
	{
		LogInfo("enter horse scene sucess playermax=%d", rolelist.size());
	}else
	{
		LogInfo("enter horse scene failed playermax=%d", rolelist.size());
	}
}

bool HorseRideMgr::Enter()
{
	UINT32 nGroup = 0;
	UINT32 nMaxGroup = GetGlobalConfig().HorseGroupCount>12?12:GetGlobalConfig().HorseGroupCount;
	CSceneSwitch::RoleList oRoleList;
	for (auto iter = m_RoleIDs.begin(); iter!=m_RoleIDs.end(); )
	{
		Role *pRole = RoleManager::Instance()->FindByRoleID(*iter);
		if (pRole)
		{
			if(pRole->GetFlag(RSF_ISSceneSwitch_Verifying)==false)
			{
				oRoleList.push_back(pRole);
			}else
			{
				LogError("roleid=%llu scene state=RSF_ISSceneSwitch_Verifying", pRole->GetID());
			}
		}
		m_RoleIDs.erase(iter++);
		if (oRoleList.size()==GetGlobalConfig().HorsePlayerNum)
		{
			ChangeScene(oRoleList);
			oRoleList.clear();
			nGroup++;
			if (nGroup>=nMaxGroup)
				return false;
		}
	}
	if (oRoleList.empty()==false)
	{
		ChangeScene(oRoleList);
		oRoleList.clear();
	}
	if (m_RoleIDs.empty())
	{
		return true;
		LogInfo("---------SetState---------------------%d",m_nStep);
	}
	return false;
}

HorseRace::RowData* HorseRideMgr::GetRank(UINT32 nRank,UINT32 nSceneID)
{
	auto iter = m_mapAward.find(nSceneID);
	if (iter != m_mapAward.end())
	{
		std::vector<HorseRace::RowData*> &vecAward = iter->second;
		for (auto siter = vecAward.begin(); siter!=vecAward.end(); siter++)
		{
			HorseRace::RowData* pRowData = *siter;
			if (pRowData->Rank == nRank)
			{
				return pRowData;
			}
		}
	}
	return NULL;
}

Horse::RowData* HorseRideMgr::GetHorse(UINT32 nSceneID)
{
	return m_horseTable.GetBysceneid(nSceneID);
}

void HorseRideMgr::Destory()
{
	m_RoleIDs.clear();
	m_bOpen = false;
	m_bEnter = false;
}

void HorseRideMgr::OnRoleLogin(Role* pRole)
{
	UINT32 nOpenTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_HORSE);
	UINT32 nEndTime  = nOpenTime + GetGlobalConfig().HorseWaitTime;
	if (m_nTime>=nOpenTime && m_nTime < nEndTime)
	{
		HallIconNoticeCheck(pRole,m_Icon.nSystemID,KKSG::HICONS_BEGIN);
	}else
	{
		HallIconNoticeCheck(pRole,m_Icon.nSystemID,KKSG::HICONS_END);
	}
}
