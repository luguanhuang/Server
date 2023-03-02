#include "pch.h"
#include "skycitymgr.h"
#include "role/role.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammember.h"
#include "util/XCommon.h"
#include "table/MultActivityMgr.h"
#include "table/globalconfig.h"
#include "scene/scenecreate.h"
#include "scene/sceneswitch.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "team/teamforceop.h"
#include "skycitycfg/skycityconfig.h"
#include "table/SkyArenaReward.h"
#include "mail/mailmgr.h"
#include "loghelper/tlogr.h"
#include "mail/mailconfig.h"
#include "scene/scenemanager.h"
#include "notice/noticemgr.h"
#include "event/eventmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "task/roletaskmgr.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/skycityranklist.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "define/systemiddef.h"
#include "levelseal/levelsealMgr.h"

INSTANCE_SINGLETON(SkyCityMgr);


SkyCityMgr::SkyCityMgr()
{
	m_nStep					 = eSkyCityFlowIdle;
	m_dwMillTime					 = 0;
	m_nSkyCityWaitID		 = 0;
	m_nGames				 = 0;
	m_bOpen					 = false;
	m_nGroupID				 = 0;
	m_nSkyCityWholeTime		 = 0;
	m_nNextTime				 = 0;
	m_nSkyCityWaitingRoomTime = 0;
	m_nSkyCityEnterLv		 = 0;
	m_bAward 	= false;
	m_bOpen1MIN = false;
	m_bOpen2MIN = false;
	m_bOpen3MIN = false;
	m_nIconTime = 0;
}

SkyCityMgr::~SkyCityMgr()
{
}


bool SkyCityMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 50, -1, __FILE__, __LINE__);
	m_nSkyCityWaitingTID	= GetGlobalConfig().GetInt("SkyCityWaitSceneID", SKYCITY_WAIT_SCENE);
	m_nSkyCityFirstHalfTime	= GetGlobalConfig().GetInt("SkyCityFirstHalfTime", 30);
	m_nSkyCityHalfTime		= GetGlobalConfig().GetInt("SkyCityHalfTime", 60);
	m_nSkyCitySpaceTime		= GetGlobalConfig().GetInt("SkyCitySpaceTime", 30);
	m_nSkyCitySecondHalfTime= GetGlobalConfig().GetInt("SkyCitySecondHalfTime", 60);
	m_nSkyCityFloor			= GetGlobalConfig().GetInt("SkyCityFloor", 7);
	m_nSkyCityGames			= GetGlobalConfig().GetInt("SkyCityGames", 5);
	m_nSkyCitySecFloorTeam  = GetGlobalConfig().GetInt("SkyCitySecFloorTeam", 32);
	m_nSkyCityWaitingRoomTime =  (GetGlobalConfig().GetInt("SkyCityWaitingRoomTime", 60))*1000;
	m_nSkyCityWholeTime = (m_nSkyCityFirstHalfTime + m_nSkyCityHalfTime + m_nSkyCitySecondHalfTime+60)*1000;
	m_nSkyCityChangeSceneTime =  GetGlobalConfig().GetInt("SkyCityChangeSceneTime", 20);
	m_nSkyCityEnterLv =  GetGlobalConfig().GetInt("SkyCityEnterLv", 32);
	return true;
}

void SkyCityMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void SkyCityMgr::Match()
{
	SortFightTeamLess();
	ClearGroup();
	FillFloor();
}

bool SkyCityMgr::CheckLevel(CRole *pRole)
{
	CRoleSummary* pSum		= CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	if (!pSum||pSum->GetLevel()<m_nSkyCityEnterLv)
	{
		return false;
	}
	return true;
}

void SkyCityMgr::ClearPPT()
{
	m_mapSingle.clear();
}

void SkyCityMgr::FirstMatch()
{
	SortSinglePPT();
	AddMemberTeam();
	SingleToTeam();
	EnterToHall();
	ClearPPT();
	ClearZombieTeam();
	if (m_mapFightTeam.size()>0)
	{
		BuildTeamData();
		SortFightTeamLess();
		FirstFillFloor();
		SetTeamState();
		BuildGroupTeam();
	}
}

void SkyCityMgr::AddMemberTeam()
{
	TeamForceOp oper;
	for (auto iter = m_mapFightTeam.begin(); iter!=m_mapFightTeam.end();)
	{
		FightTeam *pFightTeam = iter->second;
		if (pFightTeam)
		{
			Team *pTeam = TeamMgr::Instance()->FindTeam(pFightTeam->nTeamID);
			if (pTeam)
			{
				int nSize = SKYCITY_TEAM_MAX_MEMBERS - pTeam->GetMember().size();
				std::vector<CRole *> vectWait;
				while (m_deqRole.size()&&nSize>0)
				{
					vectWait.push_back(m_deqRole.front());
					m_deqRole.pop_front();
					nSize--;
				}
				if (vectWait.size())
				{
					for (auto miter = vectWait.begin(); miter!=vectWait.end(); miter++)
					{
						CRole *pRole = *miter;
						if (pRole)
						{
							LogInfo("AddMemberTeam====TeamID===%d, uuid=%llu name=%s",pFightTeam->nTeamID,pRole->GetID(),pRole->GetName().c_str());
						}
					}
					oper.JoinTeam(pTeam->GetID(), vectWait);
				}
			}else
			{
				delete pFightTeam;
				m_mapFightTeam.erase(iter++);
				continue;
			}
		}
	    iter++;
	}
	
}

void SkyCityMgr::EnterToHall()
{
	while(m_deqRole.size())
	{
		CRole *pRole = m_deqRole.front();
		if (pRole)
		{
			CSceneSwitch::EnterMainHall(pRole);
			LogInfo("EnterToHall==== uuid=%llu name=%s", pRole->GetID(),pRole->GetName().c_str());
			Notice notice(SkyCityOpen_MatchFail);
			notice.SendRole(pRole->GetID());		
		}
		m_deqRole.pop_front();
	}
}

void SkyCityMgr::SingleToTeam()
{
	TeamForceOp oper;
	std::vector<CRole *> vecSingle;
	for(auto iter = m_deqRole.begin(); iter != m_deqRole.end(); iter++)
	{
		CRole *pRole = *iter;
		if (pRole->GetTeamID()!=0)
		{
			vecSingle.push_back(*iter);	
		}
	}
	oper.LeaveTeam(vecSingle);
	vecSingle.clear();
	UINT32 nGroupSize = m_deqRole.size()/3;
	for (UINT32 i = 0; i < nGroupSize;i++)
	{
		CRole *pRole = m_deqRole.front();
		m_deqRole.pop_front();
		vecSingle.push_back(pRole);
		pRole = m_deqRole.front();
		m_deqRole.pop_front();
		vecSingle.push_back(pRole);
		pRole = m_deqRole.front();
		m_deqRole.pop_front();
		vecSingle.push_back(pRole);
		
		for (auto iter = vecSingle.begin(); iter != vecSingle.end(); iter++)
		{
			auto role = *iter;
			LogInfo("SingleToTeam==== uuid=%llu name=%s", role->GetID(),role->GetName().c_str());
		}

		Team *pTeam = oper.CreateTeam(vecSingle, SKYCITY_DNE_ID);
		if (pTeam)
		{
			if (!m_mapFightTeam[pTeam->GetID()])
			{
				m_mapFightTeam[pTeam->GetID()] = new FightTeam();
			}
			m_mapFightTeam[pTeam->GetID()]->nTeamID	= pTeam->GetID();
		}else
		{
			LogInfo("SingleToTeam====failed");
		}
		vecSingle.clear();
	}	
}

void SkyCityMgr::TeamEnterHall(Team *pTeam)
{
	if (!pTeam) return;
	for (auto iter = pTeam->GetMember().begin(); iter != pTeam->GetMember().end(); iter++)
	{
		CTeamMember *pMember = *iter;
		if (pMember)
		{
			CRole *pRole = CRoleManager::Instance()->GetByRoleID(pMember->m_qwRoleID);
			if (pRole)
			{
				LogInfo("TeamEnterHall==== uuid=%llu name=%s", pRole->GetID(),pRole->GetName().c_str());
				Notice notice(SkyCityOpen_MatchFail);
				notice.SendRole(pRole->GetID());	
				CSceneSwitch::EnterMainHall(pRole);
			}
		}
	}
}

void SkyCityMgr::ClearZombieTeam()
{
	for (auto iter = m_mapFightTeam.begin(); iter!=m_mapFightTeam.end();)
	{
		FightTeam* pFightTeam = iter->second;
		Team *pTeam = TeamMgr::Instance()->FindTeam(pFightTeam->nTeamID);
		if (pTeam&&pTeam->GetMember().size()<2)
		{
			TeamEnterHall(pTeam);
			delete pFightTeam;
			m_mapFightTeam.erase(iter++);
		}else
		{
			iter++;
		}
	}
}

void SkyCityMgr::BuildTeamData()
{
	for (auto iter = m_mapFightTeam.begin(); iter!=m_mapFightTeam.end();iter++)
	{
		FightTeam* pFightTeam = iter->second;
		if (!pFightTeam)
		{
			continue;
		}
		Team *pTeam = TeamMgr::Instance()->FindTeam(pFightTeam->nTeamID);
		if (pTeam)
		{
			for (auto subiter = pTeam->GetMember().begin(); subiter != pTeam->GetMember().end(); subiter++)
			{
				CTeamMember *pMember = *subiter;
				if(pMember)
				{
					UINT64 nRoleID = pMember->m_qwRoleID;
					CRoleSummary* pSum		= CRoleSummaryMgr::Instance()->GetRoleSummary(nRoleID);
					if(pSum)
					{
						FightRoleEx *pFightRole = new FightRoleEx();
						pFightRole->nRoleID	= nRoleID;
						pFightRole->name		= pSum->GetName();
						pFightRole->nJob		= pSum->GetProfession();
						pFightRole->nPPT		= pSum->GetMaxPPT();
						pFightRole->nLevel	= pSum->GetLevel();
						pFightRole->nFloor    = pFightTeam->nFloor;
						pFightTeam->nPPT += pSum->GetMaxPPT();
						if(!CRoleManager::Instance()->GetByRoleID(nRoleID))
							pFightRole->bLost     = true;
						else
							pFightRole->bLost     = false;
						pFightTeam->mapRole[nRoleID]= pFightRole;

					}

				}
			}
		}
		
	}

}


void SkyCityMgr::SetTeamState()
{
	for (auto iter = m_mapFightTeam.begin(); iter!=m_mapFightTeam.end();iter++)
	{
		FightTeam* pFightTeam = iter->second;
		if (pFightTeam)
		{
			Team *pTeam = TeamMgr::Instance()->FindTeam(pFightTeam->nTeamID);
			if (pTeam)
			{
				pTeam->SetInBattleState();
			}
		}
	}
}

void SkyCityMgr::FirstFillFloor()
{
	GroupTeam *pGroupTeam = NULL;
	UINT32 nFloor = 0;
	for (auto iter = m_mapSortFightTeam.begin(); iter != m_mapSortFightTeam.end(); iter++)
	{
		UINT32 nSize = 0;
		for (auto miter = m_mapSortFightTeam[iter->first].begin(); miter!= m_mapSortFightTeam[iter->first].end(); miter++)
		{
			FightTeam* pFigthTeam = *miter;
			if (pFigthTeam==NULL)
				continue;
			if (nSize < GetGlobalConfig().SkyCity2FloorGroup)
				nFloor = 2;
			else
				nFloor = 1;
			pFigthTeam->nFloor = nFloor;
			SetRoleFloor(pFigthTeam,false,nFloor);
			if (nSize%2==0)
			{
				pGroupTeam			= new GroupTeam();
				pGroupTeam->pSelf	= pFigthTeam;
				pGroupTeam->nFloor	= nFloor;
				m_mapGroup[nFloor].insert(pGroupTeam);
			}else
			{
				pGroupTeam->pTarget = pFigthTeam;
			}
			nSize++;
		}
	}
}


void SkyCityMgr::FillFloor()
{
	std::vector<FightTeam* > vectMatchRobot;
	for (auto iter = m_mapSortFightTeam.begin(); iter != m_mapSortFightTeam.end(); iter++)
	{
		GroupTeam *pGroupTeam = NULL;
		UINT32 nFloor = iter->first;
		UINT32 nSize	= 0;
		for (auto miter = m_mapSortFightTeam[nFloor].begin(); miter!= m_mapSortFightTeam[nFloor].end(); miter++)
		{
			FightTeam* pFightTeam = *miter;
			if (pFightTeam&&pFightTeam->nFail<2)
			{
				if (nSize%2==0)
				{
					pGroupTeam			= new GroupTeam();
					pGroupTeam->pSelf	= pFightTeam;
					pGroupTeam->nFloor	= pFightTeam->nFloor;
					m_mapGroup[nFloor].insert(pGroupTeam);
				}else
				{
					if (pGroupTeam)
					{
						pGroupTeam->pTarget = pFightTeam;
					}else
					{
						LogInfo("--SkyCityMgr::FillFloor()  error");
					}
				}
				nSize++;
			}else
			{
				vectMatchRobot.push_back(pFightTeam);
			}
		}
	}
	for (auto iter = vectMatchRobot.begin(); iter!=vectMatchRobot.end();iter++)
	{
		FightTeam* pTeam = *iter;
		if (pTeam)
		{
			GroupTeam *pGroupTeam = new GroupTeam();
			pGroupTeam->pSelf = pTeam;
			pGroupTeam->nFloor = pTeam->nFloor;
			m_mapGroup[pTeam->nFloor].insert(pGroupTeam);
		}
	}
}

void SkyCityMgr::OnRoleLogout(CRole *pRole)
{
	SetFightRoleState(pRole);
	OnClearFightRole(pRole);
}



void SkyCityMgr::OnLeaveTeam(CRole *pRole,bool bDel)
{
	auto iter = m_mapFightTeam.find(pRole->GetTeamID());
	if (iter!= m_mapFightTeam.end())
	{
		auto pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if (pTeam)
		{
			TeamForceOp oper;
			std::vector<CRole*> vecRole;
			vecRole.push_back(pRole);
			oper.LeaveTeam(vecRole);
			if (bDel)
			{
				FightTeam* pFightTeam = iter->second;
				if (pFightTeam)
				{
					auto siter = pFightTeam->mapRole.find(pRole->GetID());
					if (siter!=pFightTeam->mapRole.end())
					{
						delete siter->second;
						pFightTeam->mapRole.erase(siter);
						LogInfo("OnLeaveTeam---Del TeamID=%d nRoleID=%llu", pTeam->GetID(),pRole->GetID());
					}
				}

			}
		}
	}
}

//设置成员在线状况
void SkyCityMgr::SetFightRoleState(CRole *pRole)
{
	for (auto iter = m_mapGroup.begin(); iter != m_mapGroup.end();iter++)
	{
		UINT32 nFloor = iter->first;
		for (auto miter = m_mapGroup[nFloor].begin(); miter != m_mapGroup[nFloor].end();miter++)
		{
			GroupTeam *pGroupTeam = *miter;
			if (pGroupTeam)
			{
				FightRoleEx *pFight  = pGroupTeam->FindFightRole(pRole->GetID());
				if(pFight)
				{
					pFight->bLost = true;
					break;
				}	
			}
		}
	}
}

//清理单人
void SkyCityMgr::OnClearFightRole(CRole *pRole)
{
	if (!pRole)
	{
		return;
	}
	UINT64 nRole = pRole->GetID();
	UINT32 nTeamID = pRole->GetTeamID();
	auto iter = m_mapSingle.find(pRole->GetID());
	if (iter != m_mapSingle.end())
	{
		m_mapSingle.erase(iter);
	}
}

void SkyCityMgr::OnWaitSceneLogout(CRole *pRole)
{
	OnClearFightRole(pRole);
	auto iter = m_mapFightTeam.find(pRole->GetTeamID());
	if (iter!= m_mapFightTeam.end())
	{
		auto pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if (pTeam)
		{
			TeamForceOp oper;
			std::vector<CRole*> vecRole;
			vecRole.push_back(pRole);
			oper.LeaveTeam(vecRole);
		}
	}
}


void SkyCityMgr::ClearGroup()
{
	for (auto iter = m_mapGroup.begin(); iter != m_mapGroup.end(); iter++)
	{
		for (auto miter = m_mapGroup[iter->first].begin(); miter != m_mapGroup[iter->first].end(); miter++)
		{
			GroupTeam *pGroupTeam = *miter;
			delete pGroupTeam;
		}
	}
	m_mapGroup.clear();
}


void SkyCityMgr::ClearFightTeam(FightTeam *pFightTeam)
{
	for (auto iter = pFightTeam->mapRole.begin(); iter != pFightTeam->mapRole.end(); iter++)
	{
		FightRoleEx *pFightRole = iter->second;
		if (pFightRole)
			delete pFightRole;
	}
	pFightTeam->mapRole.clear();
}

void SkyCityMgr::DesoryGame()
{
	for (auto iter  = m_mapFightTeam.begin(); iter!=m_mapFightTeam.end(); iter++)
	{
		FightTeam* pFightTeam = iter->second;
		if (pFightTeam)
		{
			ClearFightTeam(pFightTeam);
			delete pFightTeam;
		}
	}

	for (auto iter = m_mapGroup.begin(); iter!=m_mapGroup.end();iter++) 
	{
		for (auto miter = iter->second.begin(); miter != iter->second.end(); miter++)
		{
			delete *miter;
		}
	}
	m_mapGroup.clear();
	m_mapFightTeam.clear();
	m_mapSortFightTeam.clear();
	m_deqRole.clear();
	m_deqGroupTeam.clear();
	m_bOpen3MIN = false;
	m_bOpen2MIN = false;
	m_bOpen1MIN = false;
	m_nSkyCityWaitID	= 0;
	m_nGroupID			= 0;
	m_nGames			= 0;
	m_bOpen				= false;
}

void SkyCityMgr::AddGameResult(UINT32 nGroupID,UINT32 nTeamID)
{
	GroupTeam *pGroup =	GetGroupTeam(nGroupID);
	if (!pGroup){return;}
	if (nTeamID == 0)	//平
	{
		if (pGroup->pSelf)
			pGroup->pSelf->nResult = eSkyCityGameFlat;
		if (pGroup->pTarget)
			pGroup->pTarget->nResult = eSkyCityGameFlat;
	}else if (nTeamID == (UINT32)-1)//输
	{
		if (pGroup->pSelf)
			pGroup->pSelf->nResult = eSkyCityGameFail;
		if (pGroup->pTarget)
			pGroup->pTarget->nResult = eSkyCityGameFail;
	}else
	{
		FightTeam *pWinTeam = pGroup->GetFightTeam(nTeamID);
		if (pWinTeam)
		{
			pWinTeam->nResult = eSkyCityGameWin;
			pWinTeam->nFloor++;
			SetRoleFloor(pWinTeam,true,pWinTeam->nFloor);
			LogInfo("AddGameResult== nGroupID=%d  nTeamID=%d nFloor=%d", nGroupID,nTeamID,pWinTeam->nFloor);
			if (pWinTeam->nFail>0)
				pWinTeam->nFail--;
		}
		FightTeam *pFailTeam = pGroup->GetOtherFightTeam(nTeamID);
		if (pFailTeam)
		{
			pFailTeam->nFail++;
			pFailTeam->nResult = eSkyCityGameFail;
		}
	}
}

void SkyCityMgr::SetRoleFloor(FightTeam *pFightTeam,bool bOnline,UINT32 nFloor)
{
	if (!pFightTeam) return;
	for(auto iter = pFightTeam->mapRole.begin(); iter!= pFightTeam->mapRole.end(); iter++)
	{
		FightRoleEx *pFightRole = iter->second;
		if (pFightRole)
		{
			if (bOnline)
			{
				if(!pFightRole->bLost)
				{
					pFightRole->nFloor = nFloor;
				}
			}else
			{
				pFightRole->nFloor = nFloor;
			}
		}
	}

}

GroupTeam * SkyCityMgr::GetGroupTeam(UINT32 nGroupID)
{
	for(auto iter= m_mapGroup.begin(); iter!=m_mapGroup.end();iter++)
	{
		for (auto miter = iter->second.begin(); miter != iter->second.end();miter++)
		{
			GroupTeam *pGroup = *miter;
			if (pGroup)
			{
				if(nGroupID == pGroup->nGroupID)
					return pGroup;
			}
		}
	}
	return NULL;
}

void SkyCityMgr::BuildGroupTeam()
{
	m_deqGroupTeam.clear();
	for(auto iter= m_mapGroup.begin(); iter!=m_mapGroup.end();iter++)
	{
		UINT32 nFloor = iter->first;
		for (auto miter = m_mapGroup[nFloor].begin(); miter != m_mapGroup[nFloor].end();miter++)
		{
			GroupTeam *pGroup = *miter;
			if (pGroup)
			{
				pGroup->nGroupID = ++m_nGroupID;
				m_deqGroupTeam.push_back(pGroup);
			}
		}

	}
}

bool SkyCityMgr::CreateBattle()
{
	UINT32 nSize = 0;
	while (m_deqGroupTeam.size())
	{		
		if (nSize>=10){break;}
		GroupTeam *pGroup = m_deqGroupTeam.front();
		CreateSceneBegin(pGroup);
		m_deqGroupTeam.pop_front();
		nSize++;
	}
	if (m_deqGroupTeam.size()==0)
		return true;
	return false;
}




void SkyCityMgr::HighAwardNotice(FightTeam *pFightTeam)
{
	string szName = "";
	UINT32 nSize = 0;
	UINT32 nFloor = 0;
	std::set<UINT64> guildIDs;
	for (auto iter = pFightTeam->mapRole.begin(); iter != pFightTeam->mapRole.end(); iter++)
	{
		FightRoleEx *pFightRole = iter->second;
		if (pFightRole==NULL)
			continue;
		nFloor = pFightRole->nFloor;
		if (nFloor >= 6)
		{
			UINT64 nRoleID = pFightRole->nRoleID;
			CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pFightRole->nRoleID);
			if (pRoleSum==NULL)
				continue;
			szName += pRoleSum->GetName();
			LogInfo("HighAwardNotice %u Floor == %s", nFloor,szName.c_str());
			szName += ",";
			nSize++;

			UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(nRoleID);
			if (guildID)
			{
				guildIDs.insert(guildID);
			}	
		}
		EventMgr::Instance()->AddEventOnlyOnMS(pFightRole->nRoleID, BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_SKYARENA_FLOOR,nFloor);
		EventMgr::Instance()->AddEventOnlyOnMS(pFightRole->nRoleID, MENTOR_EVENT_OFFSET + KKSG::MentorTask_SkyFloor,0, nFloor);
		EventMgr::Instance()->AddEventOnlyOnMS(pFightRole->nRoleID, MENTOR_EVENT_OFFSET + KKSG::MentorTask_SkyCount, 0, 1);
	}
	if (nSize)
	{
		if (nFloor == 7)
		{
			Notice notice(SkyCityOpen_7Floor);
			notice.Replace("$R",szName);
			notice.Send();
		}
		if (nFloor == 6)
		{
			Notice guildnotice(SkyCityWinGuild);
			guildnotice.Replace("$R",szName);
			for (auto iter = guildIDs.begin(); iter != guildIDs.end(); ++iter)
			{
				guildnotice.SendGuild(*iter);
			}
		}
	}
}

void SkyCityMgr::GiveFightTeam(FightTeam *pFightTeam)
{
	UINT32 levelseal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	levelseal = SkyCityConfig::Instance()->GetLevelSeal(levelseal);

	for (auto iter = pFightTeam->mapRole.begin(); iter != pFightTeam->mapRole.end(); iter++)
	{
		FightRoleEx *pFightRole = iter->second;
		if (pFightRole==NULL)
			continue;
		UINT32 nFloor = pFightRole->nFloor;
		UINT64 nRoleID = pFightRole->nRoleID;

		CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pFightRole->nRoleID);
		if (pRoleSum==NULL)
			continue;
		
		KKSG::SkyCityRankData data;
		data.set_roleid(pRoleSum->GetID());
		data.set_job(pRoleSum->GetProfession());
		data.set_floor(nFloor);
		data.set_rolename(pRoleSum->GetName());
		data.set_fight(pRoleSum->GetMaxPPT());
		data.set_killer(pFightRole->nKiller);
		CRankListMgr::Instance()->GetSkyCityRankList()->OnChange(data);
		NotifyEndIcon(nRoleID,KKSG::HICONS_BEGIN);
		
		RoleTaskMgr::Instance()->UpdateTask(nRoleID,KKSG::TaskOper_Add,KKSG::TaskConn_Activity,KKSG::TaskActType_SkyCityRound,nFloor); 

		const SkyArenaReward::RowData* pRow = SkyCityConfig::Instance()->GetSkyArenaReward(levelseal,nFloor);
		if (pRow)
		{
			std::vector<ItemDesc> rewards;
			for (auto iter = pRow->Reward.begin(); iter != pRow->Reward.end(); iter++)
			{
				Sequence<uint, 2> seq = *iter;
				ItemDesc item;
				item.itemID = seq[0];
				item.itemCount = seq[1];
				rewards.push_back(item);
			}
			const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_SkyCity);
			char szBuf[64] = {0};
			sprintf(szBuf, "%d", nFloor);
			string szContent = pConf->m_content;
			string szTitle   = pConf->m_title;
			string szMailContent = XCommon::ReplaceStr(szContent, "x", szBuf);
			MailData oData(szTitle, szMailContent,rewards ,MAIL_EXPIRE_TIME);
			oData.SetReason(ItemFlow_SkyCityAward);
			CMailMgr::Instance()->SendMail(nRoleID, oData);
			EventMgr::Instance()->AddEvent(nRoleID,DESIGNATION_COM_TYPE_SKYCITY,nFloor);
			SSInfo << "Send Mail To Client, MailType = SkyCityMgr::GiveFightTeam, " << "RoleID=" << nRoleID<<" nFloor="<<nFloor<< END;
		}
	}
}



void SkyCityMgr::GiveAward()
{
	// 处理排行榜
	SkyCityRankList* pCurRankList = (SkyCityRankList*)CRankListMgr::Instance()->GetRankList(KKSG::SkyCityRank);
	if (pCurRankList)
	{
		pCurRankList->Clear();
	}
	m_bAward = true;
	for (auto iter = m_mapGroup.begin(); iter != m_mapGroup.end(); iter++) 
	{
		std::set<GroupTeam * > setGroup = iter->second;
		for (auto miter = setGroup.begin(); miter != setGroup.end(); miter++)
		{
			GroupTeam * pGroupTeam = *miter;
			if (pGroupTeam)
			{
				if (pGroupTeam->pSelf)
				{
					GiveFightTeam(pGroupTeam->pSelf);
					HighAwardNotice(pGroupTeam->pSelf);
				}
				if (pGroupTeam->pTarget)
				{
					GiveFightTeam(pGroupTeam->pTarget);
					HighAwardNotice(pGroupTeam->pTarget);
				}
			}
		}
	}

}


bool SkyCityMgr::AddTeam(Team *pTeam)
{
	bool bState	= MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_SKYCITY);
	if (bState)
	{
		auto iter = m_mapFightTeam.find(pTeam->GetID());
		FightTeam* pFightTeam = NULL;
		if (iter != m_mapFightTeam.end()&&iter->second)
		{
			pFightTeam = iter->second;
		}else
		{
			pFightTeam = new FightTeam();
			m_mapFightTeam[pTeam->GetID()] = pFightTeam;
		}
		pFightTeam->nTeamID	= pTeam->GetID();
		std::list<CTeamMember *> memList		= pTeam->GetMember();
		for (auto iter= memList.begin();iter!=memList.end();iter++)
		{
			CTeamMember *pMember= *iter;
			if (pMember)
			{
				CRole *pRole = CRoleManager::Instance()->GetByRoleID(pMember->m_qwRoleID);
				if (pRole)
					EnterWaitRoom(pRole,false);
			}
		}
		return true;
	}
	return false;
}

void SkyCityMgr::GetNoticeRole(std::vector<UINT64> &vecRole)
{
	for(auto iter = CRoleManager::Instance()->Begin(); iter!=CRoleManager::Instance()->End();iter++)
	{
		if(CheckLevel(*iter))
		{
			vecRole.push_back((*iter)->GetID());
		}
	}
}

void SkyCityMgr::ActivityNotice()
{
	UINT32 nOpenTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_SKYCITY);
	UINT32 nTime     = TimeUtil::GetTime();

	if (nOpenTime>nTime && nOpenTime-nTime<=180&&!m_bOpen3MIN)
	{
		std::vector<UINT64> vecRole;
		GetNoticeRole(vecRole);
		Notice notice(SkyCityOpen_3MIN);
		notice.SendPlays(vecRole);
		m_bOpen3MIN = true;
	}else if (nOpenTime>nTime && nOpenTime-nTime<=120&&!m_bOpen2MIN)
	{
		std::vector<UINT64> vecRole;
		GetNoticeRole(vecRole);
		Notice notice(SkyCityOpen_2MIN);
		notice.SendPlays(vecRole);
		m_bOpen2MIN = true;
	}else if (nOpenTime>nTime && nOpenTime-nTime<=60&&!m_bOpen1MIN)
	{
		std::vector<UINT64> vecRole;
		GetNoticeRole(vecRole);
		Notice notice(SkyCityOpen_1MIN);
		notice.SendPlays(vecRole);
		m_bOpen1MIN = true;
	}
}
	

void SkyCityMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	m_dwMillTime   = TimeUtil::GetMilliSecond();

	ActivityNotice();
	if (m_dwMillTime >= m_nNextTime)
	{
		switch (m_nStep)
		{
		case eSkyCityFlowIdle:
			{
				if (m_bOpen)
				{
					m_nStep = eSkyCityFlowCreateWaitRoom;
					SSInfo << "Sky City eSkyCityFlowCreateWaitRoom" << END;
				}
				m_nNextTime = m_dwMillTime+SKYCITY_TIME_SPAN;
			}
			break;
		case eSkyCityFlowCreateWaitRoom:
			{
				CreateSceneWaitBegin();
				m_nStep = eSkyCityFlowSigUp;
				m_nNextTime = m_dwMillTime+SKYCITY_TIME_SPAN;
				LogInfo("eSkyCityFlowCreateWaitRoom == %d", m_nGames);
			}
			break;
		case eSkyCityFlowSigUp:
			{
				m_nStep = eSkyCityFlowFirstMatch;
				m_nNextTime = m_dwMillTime+m_nSkyCityWaitingRoomTime;
				LogInfo("eSkyCityFlowSigUp == %d", m_nGames);
			}
			break;
		case eSkyCityFlowFirstMatch:
			{
				m_nGames++;
				m_nNextTime = m_dwMillTime+SKYCITY_TIME_SPAN;
				FirstMatch();
				LogInfo("deSkyCityFlowFirstMatch == %d", m_nGames);
				m_nStep = eSkyCityFlowChangeScene;
			}
			break;
		case eSkyCityFlowMatch:
			{
				m_nGames++;
				Match();
				if (m_nGames>m_nSkyCityGames)
				{
					m_nStep     = eSkyCityFlowAward;
					m_nNextTime = m_dwMillTime+SKYCITY_TIME_SPAN;
				}else
				{
					m_nStep     = eSkyCityFlowChangeScene;
					m_nNextTime = m_dwMillTime+SKYCITY_TIME_SPAN;
					BuildGroupTeam();
				}
				LogInfo("eSkyCityFlowMatch == %d", m_nGames);
			}
			break;
		case eSkyCityFlowChangeScene:
			{
				LogInfo("eSkyCityFlowChangeScene == %d", m_nGames);
				bool bRet = CreateBattle();
				if (bRet)
				{
					m_nStep		= eSkyCityFlowEndFight;
					m_nNextTime = m_dwMillTime+m_nSkyCityWholeTime;
				}else
				{
					m_nStep		= eSkyCityFlowChangeScene;
					m_nNextTime = m_dwMillTime+SKYCITY_TIME_SPAN*m_nSkyCityChangeSceneTime;
				}
			}
			break;
		case eSkyCityFlowEndFight:
			{
				LogInfo("eSkyCityFlowEndFight == %d", m_nGames);
				m_nStep		= eSkyCityFlowMatch;
				m_nNextTime = m_dwMillTime+SKYCITY_TIME_SPAN;
			}
			break;
		case eSkyCityFlowAward:
			{
				LogInfo("eSkyCityFlowAward == %d", m_nGames);
				GiveAward();
				m_nNextTime = m_dwMillTime+SKYCITY_TIME_SPAN;
				m_nStep = eSkyCityFlowEnd;
			}
			break;
		case eSkyCityFlowEnd:
			{
				LogInfo("eSkyCityFlowEnd == %d", m_nGames);
				DesoryGame();
				m_nStep				= eSkyCityIconDisapper;
				m_nIconTime         = GameTime::GetTodayBeginTime() + 24*3600;
				break;
			}
		case eSkyCityIconDisapper:
			{
				UINT32 nTime = GameTime::GetTime();
				if (nTime>=m_nIconTime)
				{
					m_nStep				= eSkyCityFlowIdle;
					ClearIcon();
				}
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void SkyCityMgr::ClearIcon()
{
	for (auto iter = m_nRoleID.begin(); iter!=m_nRoleID.end(); iter++)
	{
		NotifyEndIcon(*iter, HICONS_END);

	}
	m_nRoleID.clear();
}

void SkyCityMgr::ClearRoleIcon(CRole *pRole)
{
	std::set<UINT64>::iterator iter =  m_nRoleID.find(pRole->GetID());
	if (iter!=m_nRoleID.end())
	{
		m_nRoleID.erase(iter);
	}
}


void SkyCityMgr::NotifyEndIcon(UINT64 nRoleID,KKSG::HallIconState eState)
{
	if(eState == HICONS_BEGIN)
	{
		m_nRoleID.insert(nRoleID);
	}
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(nRoleID);
	if (pRole)
	{
		PtcM2C_HallIconMNtf ntf;
		ntf.m_Data.set_state(eState);
		ntf.m_Data.set_systemid(SYS_SKYARENAEND);
		pRole->Send(ntf);
	}

}


void SkyCityMgr::FillRoleSCRoleStatistics(UINT32 nGroupID,UINT64 nRole,UINT32 nKiller, UINT32 nDeadCount, double nDamage)
{
	GroupTeam *pGroup = SkyCityMgr::GetGroupTeam(nGroupID);
	LogInfo("FillRoleSCRoleStatistics 1=nGroupID=%d UUID=%llu KILLER=%d DAMAGE=%f nDeadCount=%d", 
		nGroupID,nRole, nKiller,nDamage, nDeadCount);
	if (!pGroup) return;
	FightTeam *pFightTeam = pGroup->pSelf;
	if (pFightTeam)
	{
		FightRoleEx *pFightRole = pFightTeam->mapRole[nRole];
		if(pFightRole)
		{
			pFightRole->nKiller		+= nKiller;
			pFightRole->nDeadCount	+= nDeadCount;
			pFightRole->nDamage		+= nDamage;
			LogInfo("FillRoleSCRoleStatistics 2=nGroupID=%d UUID=%llu KILLER=%d DAMAGE=%f nDeadCount=%d",
				nGroupID, nRole, pFightRole->nKiller, pFightRole->nDamage,pFightRole->nDeadCount);
			return;
		}
	}
	pFightTeam = pGroup->pTarget;
	if (pFightTeam)
	{
		FightRoleEx *pFightRole = pFightTeam->mapRole[nRole];
		if(pFightRole)
		{
			pFightRole->nKiller		+= nKiller;
			pFightRole->nDeadCount	+= nDeadCount;
			pFightRole->nDamage		+= nDamage;
			LogInfo("FillRoleSCRoleStatistics 2=nGroupID=%d UUID=%llu KILLER=%d DAMAGE=%f nDeadCount=%d",
				nGroupID, nRole, pFightRole->nKiller, pFightRole->nDamage,pFightRole->nDeadCount);
		}
	}
}

void SkyCityMgr::EnterGroupFightScene(FightTeam *pFightTeam,UINT32 nSceneID)
{
	for(auto iter =pFightTeam->mapRole.begin(); iter!=pFightTeam->mapRole.end();iter++)
	{
		FightRoleEx *pFightRole = iter->second;
		if (pFightRole&&!pFightRole->bLost)
		{
			KKSG::SceneSwitchData oData;
			CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRoleID);
			if (pRole)
			{
				UINT32 nTeamID = pRole->GetTeamID();
				if (nTeamID!=0&&nTeamID==pFightTeam->nTeamID)
				{
					CSceneSwitch::EnterScene(pRole, nSceneID, oData);
				}
			}
		}

	}
}


void SkyCityMgr::CreateFightSceneEnd(UINT32 nGroupID, UINT32 nFloor, UINT32 nSceneID)
{
	GroupTeam *pGroup = SkyCityMgr::GetGroupTeam(nGroupID);
	if (!pGroup) return;
	if (pGroup->pSelf)
	{
		EnterGroupFightScene(pGroup->pSelf,nSceneID);
	}
	if (pGroup->pTarget)
	{
		EnterGroupFightScene(pGroup->pTarget,nSceneID);
	}
}

void SkyCityMgr::OnLogin(CRole *pRole)
{
	auto iter = m_nRoleID.find(pRole->GetID());
	if (iter!=m_nRoleID.end())
	{
		NotifyEndIcon(pRole->GetID(),KKSG::HICONS_BEGIN);
	}
}



void SkyCityMgr::OnLeaveFightScene(CRole *pRole,UINT32 nFromSceneID,UINT32 nToSceneID)
{
	if ((nFromSceneID>=31&&nFromSceneID<=37)&&(nToSceneID<31||nToSceneID>37))//战斗场景往其他场景跑
	{
		OnClearFightRole(pRole);					
		OnLeaveTeam(pRole);
		SetFightRoleState(pRole);
	}else if (nFromSceneID==30&&(nToSceneID<30||nToSceneID>37)) //等待场景往其他场景跑
	{
		OnClearFightRole(pRole);								//清理单人
		OnLeaveTeam(pRole,true);								//清理队伍
	}
}


bool SkyCityMgr::EnterWaitRoom(CRole *pRole,bool bSingle)
{
	CSceneSwitch::RoleList roRoleList;
	roRoleList.push_back(pRole);
	KKSG::SceneSwitchData roData;
	bool bRet = CSceneSwitch::EnterScene(pRole, m_nSkyCityWaitID, roData);
	if (bRet&&bSingle)
	{
		UINT64 nRoleID = pRole->GetID();
		CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(nRoleID);
		if (pRoleSum)
		{
			m_mapSingle[nRoleID]=  pRoleSum->GetMaxPPT();
		}
	}
	return bRet;
}


void SkyCityMgr::CreateSceneWaitEnd(UINT32 warType, UINT32 battleId, UINT32 sceneid)
{
	m_nSkyCityWaitID	= sceneid;
}


bool SkyCityMgr::CreateSceneWaitBegin()
{
	KKSG::CreateBattleParam param;
	KKSG::SkyCitySceneInfo *pInfo =  param.mutable_skycity();
	pInfo->set_bwaitroom(true);
	bool flag = CSceneCreator::Instance()->CreateBattleScene(m_nSkyCityWaitingTID, &ICreateSkyCityWaitListener::GlobalCreateSkyCityWaitListener, param);	
	if(false == flag) return false;
	return true;
}


void SkyCityMgr::AddSkyCityStatistics(FightTeam *pFightTeam, KKSG::SkyCitySceneInfo *pInfo)
{
	for (auto iter = pFightTeam->mapRole.begin(); iter!= pFightTeam->mapRole.end(); iter++)
	{
		FightRoleEx* pFightRole = iter->second;
		if (pFightRole)
		{
			KKSG::SCRoleStatistics *pStatistics = pInfo->add_rolsstatistics();
			LogInfo("AddSkyCityStatistics=UUID=%llu KILLER=%d DAMAGE=%f nDeadCount=%d" ,pFightRole->nRoleID, pFightRole->nKiller,pFightRole->nDamage, pFightRole->nDeadCount);
			pStatistics->set_roleid(pFightRole->nRoleID);
			pStatistics->set_killcount(pFightRole->nKiller);
			pStatistics->set_deadcount(pFightRole->nDeadCount);
			pStatistics->set_totaldamage(pFightRole->nDamage);
		}
	}
}

void SkyCityMgr::AddSkyCityTeamBaseInfo(FightTeam *pFightTeam, KKSG::SkyCitySceneInfo *pInfo,bool bSelf)
{
	for (auto iter = pFightTeam->mapRole.begin(); iter!= pFightTeam->mapRole.end(); iter++)
	{
		FightRoleEx *pFightRole = iter->second;
		if (!pFightRole||pFightRole->bLost)
			continue;
		CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRoleID);
		if (pRole)
		{
			UINT32 nTeamID = pRole->GetTeamID();
			if (nTeamID!=0&&nTeamID==pFightTeam->nTeamID)
			{
				KKSG::SkyCityTeamBaseInfo *pTeamInfo = NULL;
				if (bSelf)
					pTeamInfo = pInfo->add_self();
				else
					pTeamInfo = pInfo->add_target();
				LogInfo("AddSkyCityTeamBaseInfo====TeamID===%d, name=%s groupid=%d uuid=%llu job=%d games=%d floor=%d",
					pFightTeam->nTeamID, iter->second->name.c_str(),pInfo->ngroupid(),pFightRole->nRoleID, pFightRole->nJob,pInfo->ngames(),pInfo->nfloor());
				pTeamInfo->set_uid(pFightRole->nRoleID);
				pTeamInfo->set_job(pFightRole->nJob);
				pTeamInfo->set_lv(pFightRole->nLevel);
				pTeamInfo->set_ppt(pFightRole->nPPT);
				pTeamInfo->set_name(pFightRole->name);
				pTeamInfo->set_teamid(pFightTeam->nTeamID);
				pTeamInfo->set_online(pFightRole->bLost);
			}
		}
	}
}

bool SkyCityMgr::CreateSceneBegin(GroupTeam *pGroup)
{
	UINT32 nTime = TimeUtil::GetTime() + m_nSkyCityWholeTime/1000;
	if (!pGroup) return false;
	KKSG::CreateBattleParam param;
	KKSG::SkyCitySceneInfo *pInfo =  param.mutable_skycity();
	pInfo->set_ngroupid(pGroup->nGroupID);
	pInfo->set_ngames(m_nGames);
	pInfo->set_endtime(nTime);
	bool bOpen = false;
	UINT32 nFloor = 0;
	FightTeam *pSelfTeam = pGroup->pSelf;
	if (pSelfTeam)
	{
		bOpen = true;
		pInfo->set_nfloor(pSelfTeam->nFloor);
		AddSkyCityStatistics(pSelfTeam, pInfo);
		AddSkyCityTeamBaseInfo(pSelfTeam, pInfo, true);
		nFloor = pSelfTeam->nFloor;
	}
	FightTeam *pTargetTeam = pGroup->pTarget;
	if (pTargetTeam)
	{
		bOpen = true;
		pInfo->set_nfloor(pTargetTeam->nFloor);
		AddSkyCityStatistics(pTargetTeam, pInfo);
		if (pSelfTeam&&pInfo->self_size())
		{
			AddSkyCityTeamBaseInfo(pTargetTeam, pInfo, false);
		}else
		{
			AddSkyCityTeamBaseInfo(pTargetTeam, pInfo, true);
		}
		nFloor = pTargetTeam->nFloor;
	}

	if (bOpen&&nFloor>0&&nFloor<=7)
	{
		bool flag = CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().SkyCityFightSceneID[nFloor-1], &ICreateSkyCityListener::GlobalCreateSkyCityListener, param);	
		if(false == flag){return false;}
	}

	return true;
}


void SkyCityMgr::SortFightTeamLess()
{
	m_mapSortFightTeam.clear();
	std::vector<UINT32> vectFloor;
	for (auto iter = m_mapFightTeam.begin(); iter!=m_mapFightTeam.end();iter++)
	{
		FightTeam*pFigthTeam = iter->second;
		if (pFigthTeam)
		{
			UINT32 nFloor = pFigthTeam->nFloor;
			m_mapSortFightTeam[nFloor].push_back(iter->second);
			vectFloor.push_back(nFloor);
		}
	}
	for (auto iter = vectFloor.begin(); iter!=vectFloor.end();iter++)
	{
		UINT32 nFloor = *iter;
		std::sort(m_mapSortFightTeam[nFloor].begin(),m_mapSortFightTeam[nFloor].end(),TeamInfoCmp());
	}
}

void SkyCityMgr::SortSinglePPT()
{
	m_deqRole.clear();
	std::vector<SinglePPT> vectSinglePPT;
	for (auto iter = m_mapSingle.begin(); iter!=m_mapSingle.end();iter++)
	{
		SinglePPT ppt;
		ppt.nPPT = iter->second;
		ppt.nRoleID = iter->first;
		vectSinglePPT.push_back(ppt);
	}
	std::sort(vectSinglePPT.begin(),vectSinglePPT.end(),SinlePPTCmp());
	for (auto iter = vectSinglePPT.begin(); iter!= vectSinglePPT.end(); iter++)
	{
		CRole *pRole = CRoleManager::Instance()->GetByRoleID((*iter).nRoleID);
		if (pRole)
		{
			m_deqRole.push_back(pRole);
		}
	}
}

