#include "pch.h"
#include "scenehorserace.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "foreach.h"
#include "scene/scenebase.h"
#include "scene/scenevsbase.h"
#include "unit/role.h"
#include "util/gametime.h"
#include <vector>
#include "pb/project.pb.h"
#include "unit/rolemanager.h"
#include "vshandler.h"
#include "gamelogic/horsemgr.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "horserace/ptcg2c_horseawardallntf.h"
#include "gamelogic/MultActivityMgr.h"
#include "horserank/ptcg2c_horserankntf.h"
#include "horse/ptcg2c_horsefinalntf.h"
#include "horse/ptcg2c_horsecountdowntimentf.h"
#include "horserace/ptcg2c_horseanimationntf.h"
#include "unit/pet.h"
#include "table/globalconfig.h"

SceneHorseRace::SceneHorseRace(Scene* pScene):SceneVsBase(pScene),m_datamgr(pScene),m_bFirst(true),m_bReFirst(false),m_nTime(0),m_nRefreshTime(0)
{
	if (pScene->GetSceneTemplateID() == 51)
	{
		m_nEndTime	  =  MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_HORSE);
	}else
	{
		m_nEndTime	 = GameTime::GetTime() + GetGlobalConfig().HorseWholeTime;
	}

	m_nRank       =  0;
	m_nMirSec     =  0;

    Horse::RowData* pRowData = HorseRideMgr::Instance()->GetHorse(pScene->GetSceneTemplateID());
    if (NULL != pRowData)
    {
        m_FinalWallIndex = pRowData->FinalWallIndex;
        m_SprintWallIndex = pRowData->SprintWallIndex;
    }else{
        LogError("SceneHorseRace sceneid = %d get Table failed.\n", pScene->GetSceneTemplateID());
        m_FinalWallIndex = 0;
        m_SprintWallIndex = 0;
    }
}

bool SceneHorseRace::Init(const KKSG::SceneSwitchData& roData)
{
	InitState();
	for (UINT32 i = 0; i < roData.horsedata().roleid_size();i++)
	{
		m_unitids.push_back(roData.horsedata().roleid(i));
	}
	return true;
}

void SceneHorseRace::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);

}

UINT32 SceneHorseRace::GetGroup(UINT64 roleid) 
{
	UINT32 nCamp = 0;
	for (auto iter = m_unitids.begin(); iter != m_unitids.end(); iter++)
	{
		nCamp++;
		if(*iter == roleid)
		{
			return nCamp;
		}
	}
	return 0;
}

UINT32 SceneHorseRace::GetClientShowLaps(int laps)
{
    if(laps < 0)
    {
        return 0;
    }
    Scene *pScene = GetScene();
    if (!pScene)
    {
        return (UINT32)laps;
    }
    Horse::RowData* pRowData = HorseRideMgr::Instance()->GetHorse(pScene->GetSceneTemplateID());
    if (!pRowData)
    {
        return (UINT32)laps;
    }
    return min(pRowData->Laps, (UINT32)laps);
}

void SceneHorseRace::FillAllInfo(Role *pRole,KKSG::HorseReConnectRes &res)
{
	//Ìî³äÅÅÃûÐÅÏ¢
	UINT32 nRank = 1;
	bool bReach =  false;
	res.set_error(KKSG::ERR_SUCCESS);
	for (auto iter=m_vecRoleRaceInfo.begin(); iter!=m_vecRoleRaceInfo.end(); iter++)
	{
		if ((*iter).nRoleID == pRole->GetID())
		{
			if ((*iter).nState == eFinal)
			{
				KKSG::HorseFinal *pFinal =  res.mutable_selfarrive();
				pFinal->set_rank((*iter).nRank);
				pFinal->set_time((UINT32)(*iter).nTime);
				pFinal->set_turns(GetClientShowLaps((*iter).nLaps));
				bReach = true;
			}else
			{
				KKSG::HorseRank *pRank =  res.mutable_rank();
				pRank->set_rank(nRank);
				pRank->set_turns(GetClientShowLaps((*iter).nLaps));
			}
			break;
		}
		nRank++;
	}
	if (m_bReFirst && !bReach)
	{
		KKSG::HorseAnimation *pFinal =  res.mutable_otherreach();
		if (m_nEndTime > m_nTime)
		{
			pFinal->set_time((m_nEndTime-m_nTime)*1000);
		}
	}

}




UINT32 SceneHorseRace::Riding(Role *pRole)
{
	PetSys* pPetSys = pRole->Get<PetSys>();
	UINT32 nHorseID = 0;
	UINT64 nFollowID =  pPetSys->GetLastFollowId();
	if (nFollowID!=0)
	{
		pPetSys->OPFollow(nFollowID);
		nHorseID = pPetSys->GetFollowPetId();
	}else
	{
		pPetSys->AddTempPet(GetGlobalConfig().HorseID);
		nHorseID = GetGlobalConfig().HorseID;
	}
	return nHorseID;
}

bool SceneHorseRace::OnEvent(const VsEvent& event)
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

	SSDebug << __FUNCTION__ << " rolename = " << pRole->GetName() << " event = " << event.type << END;
	switch(event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			std::vector<struct RoleRaceInfo>::iterator iter = m_vecRoleRaceInfo.end();
			iter = std::find_if(m_vecRoleRaceInfo.begin(), m_vecRoleRaceInfo.end(), RoleRaceFind(pRole->GetRoleID()));
			if (iter != m_vecRoleRaceInfo.end())
			{
				(*iter).pos = pRole->GetXObject()->GetPosition_p();
			}
			m_loadmgr.AddRole(event.roleid);
			if(IsInWaitLoaded())
			{
				MakePuppet(pRole, true);
			}
            WKRoleEnter(pRole);
		}
		break;
	case VS_EVENT_ROLE_QUIT:
		{
			m_loadmgr.DelRole(event.roleid);
			m_datamgr.OnUnitCombatStop(event.roleid);
			std::vector<struct RoleRaceInfo>::iterator iter = m_vecRoleRaceInfo.end();
			iter = std::find_if(m_vecRoleRaceInfo.begin(), m_vecRoleRaceInfo.end(), RoleRaceFind(pRole->GetRoleID()));
			if (iter != m_vecRoleRaceInfo.end())
			{
				LogInfo("-------SceneHorseRace::OnEvent---VS_EVENT_ROLE_QUIT-----roleid=%llu name=%s state=%d", (*iter).nRoleID,((*iter).szName).c_str(),(*iter).nState);
				(*iter).nState = eLeave;
			}
			PetSys* pPetSys = pRole->Get<PetSys>();
			pPetSys->DelTempPet();
		}
		break;
	default:
		break;
	}
	return true;
}

VsStateType SceneHorseRace::CheckAfterFightEnd()
{	
	if (GetScene()->GetSceneTemplateID() == 51)
	{
		if(!MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_HORSE))
		{
			return VS_STATE_END;
		}
	}else
	{
		if (m_nTime >= m_nEndTime)
		{
			return VS_STATE_END;
		}
	}
	return VS_STATE_DUMMY;
}


void SceneHorseRace::OnStateEnd()
{
	GetScene()->SetDontDestory(false);
}

void SceneHorseRace::Update()
{
	m_nMirSec = TimeUtil::GetMilliSecond();
	m_nTime = GameTime::GetTime();
	m_statemgr.Update();
	Check();
	SyncRaceTime();
	RaceEnd();
}



void SceneHorseRace::SyncRaceTime()
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
		return;
	if (m_nTime  >= m_nRefreshTime + GetGlobalConfig().HorseTimeRefresh/1000)
	{
		PtcG2C_HorseCountDownTimeNtf ntf;
		if (m_nMirSec > m_nStartTime)
		{
			ntf.m_Data.set_time(m_nMirSec-m_nStartTime);
		}else
		{
			ntf.m_Data.set_time(0);
		}
		for (auto iter=m_vecRoleRaceInfo.begin(); iter!=m_vecRoleRaceInfo.end(); iter++)
		{
			Role *pRole = GetRole((*iter).nRoleID);
			if (pRole&&(*iter).nState!=eFinal)
			{
				pRole->Send(ntf);
			}
		}
		m_nRefreshTime = m_nTime + GetGlobalConfig().HorseTimeRefresh/1000;
	}
}


void SceneHorseRace::SetHorseLaps(UINT64 nRole,UINT32 nLaps)
{
	std::vector<struct RoleRaceInfo>::iterator iter = m_vecRoleRaceInfo.end();
	iter = std::find_if(m_vecRoleRaceInfo.begin(), m_vecRoleRaceInfo.end(), RoleRaceFind(nRole));
	if (iter !=  m_vecRoleRaceInfo.end())
	{
		(*iter).nLaps = nLaps;
	}
}


void SceneHorseRace::SetHorseAngle(UINT64 nRole,UINT32 nAngle)
{
	std::vector<struct RoleRaceInfo>::iterator iter = m_vecRoleRaceInfo.end();
	iter = std::find_if(m_vecRoleRaceInfo.begin(), m_vecRoleRaceInfo.end(), RoleRaceFind(nRole));
	if (iter !=  m_vecRoleRaceInfo.end())
	{
		(*iter).fAngle = nAngle;
	}
}



void SceneHorseRace::Check()
{
	Scene *pScene = GetScene();
	if (!pScene)
		return;
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
		return;
	Horse::RowData* pRowData = HorseRideMgr::Instance()->GetHorse(pScene->GetSceneTemplateID());
	if (!pRowData)
	{
		return;
	}
	Vector3 centerPos(pRowData->CenterPos[0], pRowData->CenterPos[1], pRowData->CenterPos[2]);
	Vector3 finalPos(pRowData->FinalPos[0], pRowData->FinalPos[1], pRowData->FinalPos[2]);
	for (auto iter=m_vecRoleRaceInfo.begin(); iter!=m_vecRoleRaceInfo.end(); iter++)
	{
		Role *pRole = GetRole((*iter).nRoleID);
		if (pRole)
		{
			if ((*iter).nState == eRace)
			{
				(*iter).nTime = m_nMirSec-m_nStartTime;
				Vector3 curPos = pRole->GetXObject()->GetPosition_p();
				if (curPos != (*iter).pos)
				{
					int nCrossType = 0;
					int nCurrentIdx = (*iter).nLaps;
					float fDegree = 0.0;
					bool bRet =  pScene->CheckSequenceBlock((*iter).pos,curPos,centerPos,nCurrentIdx,nCrossType,fDegree);
					if (bRet)
					{
						LogDebug("-----C--------nRole=%llu- name=%s-nLap=%d--nCrossType=%d --------",pRole->GetID(),pRole->GetName().c_str(),nCurrentIdx,nCrossType);
						if (nCrossType)
						{
                            if(nCurrentIdx == (*iter).nLaps+1 || (nCurrentIdx == 1 && (*iter).nLaps<1))
                            {
                                (*iter).nLaps = nCurrentIdx;
                            }
                        }
						else
						{
                            if (nCurrentIdx>=0 && nCurrentIdx == (*iter).nLaps)
                            {
                                (*iter).nLaps = nCurrentIdx -1;
                            }
						}
					}
					(*iter).pos = curPos;
					if ((*iter).nLaps == -1)
					{
						continue;
					}
					(*iter).fAngle = fDegree;
                    LogDebug("SceneHorseRace degree %f\n", fDegree);
					if ((*iter).nLaps == pRowData->SprintWallIndex)
					{
						(*iter).fDistance =  Vector3::Distance((*iter).pos,finalPos);
					}
					if((*iter).nLaps == pRowData->FinalWallIndex)
					{
						if (m_bFirst)
						{
							PtcG2C_HorseAnimationNtf ntf;
							ntf.m_Data.set_time(pRowData->CollisionWallTime*1000);
							GetScene()->Broadcast(ntf);
							m_bFirst = false;
							m_bReFirst = true;
							m_nEndTime = m_nTime + pRowData->CollisionWallTime;
						}
						(*iter).nState  = eFinal;
						MakePuppet(pRole, true);
					}
				}
			}

		}else
		{
			(*iter).nState = eLeave;
		}
	}
	SendRank();

}


void SceneHorseRace::GetRoleRank(Role *pRole,UINT32 &nTime, UINT32 &nRank,UINT32 &nHorse)
{
	if (!pRole)
	{
		return;
	}
	UINT32 i = 0;
	for (auto iter=m_vecRoleRaceInfo.begin(); iter!=m_vecRoleRaceInfo.end(); iter++)
	{
		i++;
		if ((*iter).nRoleID == pRole->GetID())
		{	
			if((*iter).nState== eFinal)
			{
				nTime = (UINT32)(*iter).nTime;
				nRank = (*iter).nRank;
			}else
			{
				nTime = 0;
				nRank = i;
			}
			nHorse= (*iter).nHorse;
			break;
		}
	}
}

void SceneHorseRace::RaceEnd()
{
	if (GameTime::GetTime() >= m_nEndTime)
	{
		UINT32 nRank = 1;
		for (auto iter=m_vecRoleRaceInfo.begin(); iter!=m_vecRoleRaceInfo.end(); iter++)
		{
			(*iter).nRank = nRank++;
			Role* pRole = GetRole((*iter).nRoleID);
			if (pRole)
			{
				PetSys* pPetSys = pRole->Get<PetSys>();
				pPetSys->DelTempPet();
			}
		}
		SetEnd();
	}
}


void SceneHorseRace::GetRank(std::vector<UINT64> &roles)
{
	SortRoleRace();
	for (auto iter=m_vecRoleRaceInfo.begin(); iter!=m_vecRoleRaceInfo.end(); iter++)
		roles.push_back((*iter).nRoleID);
}

void SceneHorseRace::SendRank()
{
	SortRoleRace();
	UINT32 nRank = 1;
    UINT32 nRankChange = 0;
	for (auto iter=m_vecRoleRaceInfo.begin(); iter!=m_vecRoleRaceInfo.end(); iter++)
	{
		Role *pRole = GetRole((*iter).nRoleID);
		if (pRole)
		{
			if((*iter).nLastRank != nRank || (*iter).nLastLaps != (*iter).nLaps)
			{
				if ((*iter).nState== eFinal)
				{
					if ((*iter).bSend)
					{
						if ((*iter).nRank==0)
							(*iter).nRank = ++m_nRank;
						(*iter).bSend = false;
						PtcG2C_HorseFinalNtf finalntf;
						finalntf.m_Data.set_rank((*iter).nRank);
						finalntf.m_Data.set_turns(GetClientShowLaps((*iter).nLaps));
						pRole->Send(finalntf);
                        nRankChange = 1;
					}
				}else
				{
					PtcG2C_HorseRankNtf ntf;
					ntf.m_Data.set_rank(nRank);
					ntf.m_Data.set_turns(GetClientShowLaps((*iter).nLaps));
					pRole->Send(ntf);
                    nRankChange = 2;
				}
				(*iter).nLastRank = nRank;
				(*iter).nLastLaps = (*iter).nLaps;
			}
		}
		nRank++;
	}
    WKRankChangeOrFinal(nRankChange);
}



VsStateType SceneHorseRace::IsAllLoad()
{
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		if(!m_loadmgr.HaveRole(m_unitids[i]))
		{
			return VS_STATE_DUMMY;
		}
		else if(m_loadmgr.LoadedTime(m_unitids[i]) < GetGlobalConfig().HorseStartAnimTime)
		{
			return VS_STATE_DUMMY;
		}
	}
	return VS_STATE_PREPARE321;
}


void SceneHorseRace::OnEnterP321(){}

void SceneHorseRace::OnEnterFight()
{
	for (auto iter=m_vecRoleRaceInfo.begin(); iter!=m_vecRoleRaceInfo.end(); iter++)
	{
		Role *pRole = GetRole((*iter).nRoleID);
		if (pRole)
		{
			(*iter).nState = eRace;
		}else
		{
			(*iter).nState = eLeave;
		}
	}
	m_nStartTime  = TimeUtil::GetMilliSecond();
}



VsStateType SceneHorseRace::CheckFightEnd()
{
	return VS_STATE_DUMMY;
}

void SceneHorseRace::OnEnterAfterFight()
{
}

void SceneHorseRace::PreDoEnterScene(Role* pRole)
{
	if(pRole->IsWatcher())
	{
		return;
	}

	RoleRaceInfo info(m_FinalWallIndex, m_SprintWallIndex);
	std::vector<struct RoleRaceInfo>::iterator iter = m_vecRoleRaceInfo.end();
	iter = std::find_if(m_vecRoleRaceInfo.begin(), m_vecRoleRaceInfo.end(), RoleRaceFind(pRole->GetRoleID()));
	UINT32 nHorse = Riding(pRole);
	if (iter == m_vecRoleRaceInfo.end())
	{
		info.nRoleID = pRole->GetID();
		info.nJob = pRole->GetProfession();
		info.szName = pRole->GetName();
		info.nState = eReady;
		info.nHorse = nHorse;
		info.nLv	= pRole->GetLevel();
		LogInfo("-------SceneHorseRace::OnEvent-- VS_EVENT_ROLE_LOADED------roleid=%llu name=%s state=%d", info.nRoleID,info.szName.c_str(),info.nState);
		m_vecRoleRaceInfo.push_back(info);
	}
}

