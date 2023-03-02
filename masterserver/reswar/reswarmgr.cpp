#include "pch.h"
#include "reswarmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammember.h"
#include "util/XCommon.h"
#include "table/MultActivityMgr.h"
#include "table/globalconfig.h"
#include "guildbuffmgr.h"
#include "scene/scenecreate.h"
#include "scene/sceneswitch.h"
#include "reswar/guildbuffmgr.h"
#include "levelseal/levelsealMgr.h"
#include "reswar/ptcm2c_reswarguildbriefntf.h"
#include "reswar/ptcm2c_guildbuffsimpleinfontf.h"
#include "reswar/ptcm2c_reswarranksimpleinfontf.h"
#include "reswar/ptcm2c_guildbuffsimpleitemntf.h"
#include "reswar/ptcm2c_reswarstatentf.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "reswar/ptcm2c_reswartimentf.h"
#include "reswar/ptcm2c_reswarminedatantf.h"
#include "reswar/ptcm2c_guildbuffcdparamntf.h"
#include "reswar/ptcm2c_reswarenemytimentf.h"
#include "notice/noticemgr.h"
#include "guild/guilddef.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "define/systemiddef.h"
#include "guildauct/guildauctmgr.h"
#include "util/gametime.h"
#include "team/teamforceop.h"
#include "reswar/reswarpvemgr.h"
#include "levelseal/levelsealMgr.h"

INSTANCE_SINGLETON(ResWarMgr);

ResWarMgr::ResWarMgr(){}

ResWarMgr::~ResWarMgr(){}

bool ResWarMgr::Init()
{
	m_nStep					= 0;
	m_nTime					= 0;
	m_nUniID				= 0;
	m_nNextTime				= 0;
	m_nDurationTime			= 0;
	m_nCountDownType		= 0;
	m_nCountDownTime		= 0;
	m_bNotice           = true;
	m_nEndTime				= 0;
	m_bAward                = true;
	m_bOpen					=  false;
	m_bFinal5Min			= true;
	m_nSyReadyTime          = 0;
	m_handler			    = CTimerMgr::Instance()->SetTimer(this, 0, 100, -1, __FILE__, __LINE__);
	return true;
}

void ResWarMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void ResWarMgr::SetState(bool bState)
{
	if(GetGlobalConfig().ResWarSwitch)
		m_bOpen = bState;
}

bool ResWarMgr::GetState()
{
	return m_bOpen;
}

UINT32 ResWarMgr::GetStep()
{
	return m_nStep;
}

UINT32 ResWarMgr::GetResWarPVDNEID()
{
	return GetGlobalConfig().ResWarDNEID;
}

UINT64 ResWarMgr::GetTeamFight(UINT32 nTeamID)
{
	UINT64 nRet = 0;
	Team *pTeam = TeamMgr::Instance()->FindTeam(nTeamID);
	if (pTeam)
	{
		for(auto iter =pTeam->GetMember().begin(); iter!=pTeam->GetMember().end(); iter++)
		{
			CTeamMember *pMember = *iter;
			CRoleSummary* pSum		= CRoleSummaryMgr::Instance()->GetRoleSummary(pMember->m_qwRoleID);
			if (pSum)
				nRet	+= pSum->GetMaxPPT();
		}
	}
	return nRet;
}



UINT64 ResWarMgr::GetGuildFight(UINT64 nGuildID)
{
	UINT64 nRet = 0;
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild) return 0;

	std::map<UINT64, GuildMember> vecGuildMember = pGuild->GetGuildMember();
	for (auto iter = vecGuildMember.begin(); iter!=vecGuildMember.end(); iter++)
	{
		CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(iter->first);
		if (pSum)
			nRet += pSum->GetMaxPPT();
	}
	return nRet;
}

void ResWarMgr::GetGuildGroup(UINT64 nGuildID,std::set<GuildFightTeam *> &setGuildTeam)
{
	UINT32 nGroupID = GetGuildGroupID(nGuildID);
	for (auto iter = m_mapGroupGuildInfo[nGroupID]->mapGroupGuild.begin();  iter!=m_mapGroupGuildInfo[nGroupID]->mapGroupGuild.end();iter++)
	{
		GuildTeam *pGroup = iter->second;
		for (auto miter = pGroup->mapTeam.begin(); miter!=pGroup->mapTeam.end(); miter++)
		{
			GuildFightTeam* pTeam = miter->second;
			setGuildTeam.insert(pTeam);
		}
	}
}



GuildTeam* ResWarMgr::GetGuildTeam(UINT64 nGuildID)
{
	GuildTeam *pGuildTeam = NULL;
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild) 
		return NULL;
	for (auto iter=m_mapGroupGuildInfo.begin();iter!=m_mapGroupGuildInfo.end();iter++)
	{
		GroupGuildInfo *pGroup = iter->second;
		if (pGroup)
		{
			for (auto miter = pGroup->mapGroupGuild.begin(); miter!=pGroup->mapGroupGuild.end(); miter++)
			{
				GuildTeam* pGuildTeam = miter->second;
				if (pGuildTeam&&pGuildTeam->nGuildID == nGuildID)
					return pGuildTeam;
			}
		}
	}
	return pGuildTeam;
}

void ResWarMgr::AddRoleNotice(GuildTeam *pGuildTeam, UINT64 nRoleID, UINT32 nRes)
{
	if (!pGuildTeam)
		return;
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(nRoleID);
	if (!pRole)
		return;
	Notice *pNotice = new Notice(GuildResWarAddResValue);
	pNotice->Replace("$R", pRole->GetName());
	pNotice->AddGuild(pGuildTeam->nGuildID);
	std::string strRes = ToString(nRes);
	pNotice->Replace("{0}", strRes);
	pGuildTeam->vecNotice.push_back(pNotice);
}

void ResWarMgr::AddNotice(GuildTeam *pGuildTeam,UINT64 nTargetID,UINT32 nBuffID,UINT32 nRes)
{
	if(pGuildTeam==NULL)
		return;
	if (nBuffID == eStealGuildRes) //1偷取目标公会资源总量5%~10%
	{
		Notice *pNotice =  new Notice(GuildResWarStealGuildRes);
		pNotice->AddGuild(pGuildTeam->nGuildID);
		pNotice->AddGuild(nTargetID);
		std::string strRes = ToString(nRes);
		pNotice->Replace("{0}", strRes);
		pGuildTeam->vecNotice.push_back(pNotice);
		Notice notice(GuildResWarStealGuildResChanel);
		notice.AddGuild(pGuildTeam->nGuildID);
		notice.AddGuild(nTargetID);
		notice.Replace("{0}", strRes);
		notice.SendGuild(pGuildTeam->nGuildID);

	}else if(nBuffID == eAddEnemyTime)//2目标公会探索时间延长X秒时间
	{
		Notice  *pNotice = new  Notice(GuildResWarAddEnemyTime);
		pNotice->AddGuild(pGuildTeam->nGuildID);
		pNotice->AddGuild(nTargetID);
		pNotice->AddGuild(nTargetID);
		pGuildTeam->vecNotice.push_back(pNotice);
		Notice notice(GuildResWarAddEnemyTimeChanel);
		notice.AddGuild(pGuildTeam->nGuildID);
		notice.AddGuild(nTargetID);
		notice.AddGuild(nTargetID);
		notice.SendGuild(pGuildTeam->nGuildID);

	}else if (nBuffID == eSubSelfTime)//3己方公会探索时间缩短X秒时间
	{
		Notice *pNotice = new Notice(GuildResWarSubSelfTime);
		pNotice->AddGuild(pGuildTeam->nGuildID);
		pGuildTeam->vecNotice.push_back(pNotice);
		Notice notice(GuildResWarSubSelfTimeChanel);
		notice.AddGuild(pGuildTeam->nGuildID);
		notice.SendGuild(pGuildTeam->nGuildID);
	}else if(nBuffID == eAddSelfRes) //4己方公会成员探索获得的资源量增加X倍时间
	{
		Notice *pNotice = new Notice(GuildResWarAddSelfRes);
		pNotice->AddGuild(pGuildTeam->nGuildID);
		pGuildTeam->vecNotice.push_back(pNotice);
		Notice notice(GuildResWarAddSelfResChanel);
		notice.AddGuild(pGuildTeam->nGuildID);
		notice.SendGuild(pGuildTeam->nGuildID);

	}else if (nBuffID == eEnemyUnKnown)//5敌方公会所有成员探索结果变为“？”时间
	{
		Notice *pNotice = new Notice(GuildResWarEnemyUnKnown);
		pNotice->AddGuild(pGuildTeam->nGuildID);
		pNotice->AddGuild(nTargetID);
		pNotice->AddGuild(nTargetID);
		pGuildTeam->vecNotice.push_back(pNotice);
		Notice notice(GuildResWarEnemyUnKnownChanel);
		notice.AddGuild(pGuildTeam->nGuildID);
		notice.AddGuild(nTargetID);
		notice.AddGuild(nTargetID);
		notice.SendGuild(pGuildTeam->nGuildID);
	}else if (nBuffID == eClearDeBuff)
	{
		Notice *pNotice=new Notice(GuildResWarClearDeBuff);
		pNotice->AddGuild(pGuildTeam->nGuildID);
		pGuildTeam->vecNotice.push_back(pNotice);
		Notice notice(GuildResWarClearDeBuffChanel);
		notice.AddGuild(pGuildTeam->nGuildID);
		notice.SendGuild(pGuildTeam->nGuildID);
	}
}


bool ResWarMgr::UseBuffItem(UINT64 nGuildID,UINT32 nItem)
{
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild)
		return false;
	GuildTeam*  pGuildTeam = GetGuildTeam(nGuildID);
	if (!pGuildTeam)
		return false;
	UINT32 nSize = pGuildTeam->mapBuff[nItem];
	if (nSize<=0) 
		return false;
	pGuildTeam->mapBuff[nItem] = --nSize;
	PtcM2C_GuildBuffSimpleItemNtf ntf;
	ntf.m_Data.set_guildid(nGuildID);
	LogInfo("UseBuffItem----nGuildId=%llu nItemID=%d", nGuildID, nItem);
	GetNoticeInfo(pGuildTeam,ntf.m_Data);
	for (auto iter = pGuildTeam->mapBuff.begin(); iter!=pGuildTeam->mapBuff.end(); iter++)
	{
		UINT32 nItemID = iter->first;
		UINT32 nItemCount = iter->second;
		if (nItemCount>0)
		{
			GuildBuffItem *pBuffItem = ntf.m_Data.add_item();
			pBuffItem->set_itemid(nItemID);
			pBuffItem->set_count(nItemCount);
			LogInfo("UseBuffItem----nItemID=%d nItemCount=%d", nItemID, nItemCount);
		}
	}
	const std::map<UINT64, GuildMember>& allMember = pGuild->GetGuildMember();
	for (auto iter = allMember.begin(); iter != allMember.end(); iter++)
	{
		LogInfo("-------------roleid=%llu- -------------", iter->first);
		bool bJoin = GetRoleJoin(iter->first);
		if (bJoin)
		{
			CRole *pRole = CRoleManager::Instance()->GetByRoleID(iter->first);
			if (pRole)
			{
				LogInfo("UseBuffItem----RoleID==%llu", pRole->GetID());
				pRole->Send(ntf);
			}
		}
	}
	return true;
}


void ResWarMgr::SyncGuildRank(GuildTeam *pFightTeam)
{
	if (pFightTeam==NULL)
		return;
	UINT64 nGuildID			   = pFightTeam->nGuildID;
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild) 
		return;
	GuildTeam* pGuildTeam	   = GetGuildTeam(nGuildID);
	UINT32 nGroupID			   = GetGuildGroupID(nGuildID);
	GroupGuildInfo*pGroup      = GetGuildGroup(nGroupID);
	if (!pGroup) 
		return;	
	PtcM2C_ResWarRankSimpleInfoNtf ntf;
	ResWarRankSimpleInfo info = ntf.m_Data;
	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end(); iter++)
	{
		GuildTeam* pGuildTeam	= iter->second;
		if (pGuildTeam)
		{
			ResWarRank  *pRank	= info.add_rank();
			pRank->set_id(pGuildTeam->nGuildID);
			pRank->set_name(pGuildTeam->szName);
			pRank->set_value(pGuildTeam->nResValue);
			pRank->set_icon(pGuildTeam->nIcon);
		}
	}
	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end(); iter++)
	{
		GuildTeam* pGuildTeam	= iter->second;
		if (pGuildTeam)
		{
			Guild *pOneGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
			if (pOneGuild)
			{
				for (auto siter = pOneGuild->GetGuildMember().begin(); siter != pOneGuild->GetGuildMember().end(); siter++)
				{
					CRole *pRole = CRoleManager::Instance()->GetByRoleID(siter->first);
					if (pRole)
					{
						pRole->Send(ntf);
					}
				}
			}
		}
	}
}

void ResWarMgr::GetResWarGuildRank(GuildTeam *pGuildTeam,KKSG::ResWarGuildBrief *pBrief)
{
	if (pGuildTeam==NULL)
		return;
	UINT64 nGuildID			   = pGuildTeam->nGuildID;
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild) 
		return;
	UINT32 nGroupID			   = GetGuildGroupID(nGuildID);
	GroupGuildInfo*pGroup      = GetGuildGroup(nGroupID);
	if (!pGroup) 
		return;	
	ResWarRankSimpleInfo *pInfo = pBrief->mutable_rankinfo();
	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end(); iter++)
	{
		GuildTeam* pGuildTeam	= iter->second;
		if (pGuildTeam )
		{
			ResWarRank  *pRank	= pInfo->add_rank();
			pRank->set_id(pGuildTeam->nGuildID);
			pRank->set_name(pGuildTeam->szName);
			pRank->set_value(pGuildTeam->nResValue);
			pRank->set_icon(pGuildTeam->nIcon);
		}
	}
}


void ResWarMgr::GetResWarGuildBuff(GuildTeam *pGuildTeam,KKSG::ResWarGuildBrief *pBrief)
{
	if (pGuildTeam==NULL) 
		return;
	UINT64 nGuildID			   = pGuildTeam->nGuildID;
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild) 
		return;
	UINT32 nGroupID			   = GetGuildGroupID(nGuildID);
	GroupGuildInfo*pGroup      = GetGuildGroup(nGroupID);
	if (!pGroup) 
		return;	
	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end(); iter++)
	{
		GuildTeam* pGuildTeam = iter->second;
		if (!pGuildTeam) 
			continue;
		GuildBuffSimpleInfo  *pInfo	= pBrief->add_buffinfo();
		pInfo->set_guildid(pGuildTeam->nGuildID);
		std::set<GuildBuffInfo *> setGuildBuffInfo;
		GuildBuffMgr::Instance()->GetBuff(pGuildTeam->nGuildID, setGuildBuffInfo);
		for (auto siter = setGuildBuffInfo.begin(); siter!=setGuildBuffInfo.end(); siter++)
		{
			GuildBuffInfo *pGuildBuffInfo = *siter;
			if (!pGuildBuffInfo) 
				continue;
			GuildBuff *pGuildBuff = pInfo->add_buff();
			pGuildBuff->set_id(pGuildBuffInfo->nID);
			if (pGuildBuffInfo->nTime > m_nTime)
			{
				pGuildBuff->set_time(pGuildBuffInfo->nTime - m_nTime);
			}else
			{
				pGuildBuff->set_time(0);
			}
		}
	}
}

void ResWarMgr::OnLeaveGuild(UINT64 nGuildID, UINT64 nRoleID)
{
	if (!m_bOpen)
	{
		return;
	}
	GuildTeam* pGuildTeam	   = GetGuildTeam(nGuildID);
	if (!pGuildTeam)
	{
		return;
	}
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(nRoleID);
	if(pRole)
	{
		TeamForceOp oper;
		std::vector<CRole*>  roles;
		roles.push_back(pRole);
		oper.LeaveTeam(roles);
		SendRoleIcon(pRole,SYS_RESWAR,HICONS_END);
	}
	for (auto iter = pGuildTeam->mapTeam.begin(); iter!=pGuildTeam->mapTeam.end(); iter++)
	{
		GuildFightTeam *pFightTeam = iter->second;
		if (pFightTeam)
		{
			auto miter = pFightTeam->mapFightRole.find(nRoleID);
			if (miter!=pFightTeam->mapFightRole.end())
			{
				CRoleSummary* pSum	 = CRoleSummaryMgr::Instance()->GetRoleSummary(nRoleID);
				if (pSum)
				{
					if (pFightTeam->nFight>pSum->GetMaxPPT())
					{
						pFightTeam->nFight -= pSum->GetMaxPPT();
					}
				}
				GuildFightRole *pFightRole = miter->second;
				LogInfo("LeaveGuild roleid=%llu name=%s",pFightRole->nRole,pFightRole->szName.c_str());
				delete pFightRole;
				pFightTeam->mapFightRole.erase(miter);
				break;
			}
		}
	}

}


void ResWarMgr::OnJoinGuild(UINT64 nGuildID, CRole *pRole)
{
	if (NULL==pRole||!m_bOpen)
		return;
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (NULL==pGuild)
		return;
	GuildTeam* pGuildTeam	   = GetGuildTeam(pGuild->GetID());
	if (NULL==pGuildTeam)
		return;
	SendRoleIcon(pRole,SYS_RESWAR,HICONS_BEGIN);
}


void ResWarMgr::GetNoticeInfo(GuildTeam *pGuildTeam,KKSG::GuildBuffSimpleItem &item)
{
	if (pGuildTeam==NULL) 
		return;
	UINT32 nStart = 0;
	if (pGuildTeam->vecNotice.size()>20)
		nStart = pGuildTeam->vecNotice.size()- 20;
	for (UINT32 i = nStart; i < pGuildTeam->vecNotice.size(); i++)
	{
		Notice *pInfo = pGuildTeam->vecNotice[i];
		KKSG::ChatInfo* pChatInfo = item.add_chatinfo();
		pChatInfo->CopyFrom(*pInfo->chatinfo);
	}
}


void ResWarMgr::GetNoticeInfo(GuildTeam *pGuildTeam,KKSG::ResWarGuildBrief *pBrief)
{
	if (pGuildTeam==NULL) 
		return;
	UINT32 nStart = 0;
	if (pGuildTeam->vecNotice.size()>20)
		nStart = pGuildTeam->vecNotice.size()- 20;
	for (UINT32 i = nStart; i < pGuildTeam->vecNotice.size(); i++)
	{
		Notice *pInfo = pGuildTeam->vecNotice[i];
		KKSG::ChatInfo* pChatInfo = pBrief->add_chatinfo();
		pChatInfo->CopyFrom(*pInfo->chatinfo);
	}
}


void ResWarMgr::SyncGuildBuff(GuildTeam *pFightTeam)
{
	if (pFightTeam==NULL) 
		return;
	UINT64 nGuildID			   = pFightTeam->nGuildID;
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild)
		return;
	GuildTeam* pGuildTeam	   = GetGuildTeam(nGuildID);
	UINT32 nGroupID			   = GetGuildGroupID(nGuildID);
	GroupGuildInfo*pGroup      = GetGuildGroup(nGroupID);
	if (!pGroup) 
		return;	
	PtcM2C_GuildBuffSimpleInfoNtf ntf;
	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end(); iter++)
	{
		GuildTeam* pGuildTeam = iter->second;
		if (!pGuildTeam) 
			continue;
		GuildBuffSimpleInfo  *pInfo	= ntf.m_Data.add_buff();
		pInfo->set_guildid(pGuildTeam->nGuildID);
		std::set<GuildBuffInfo *> setGuildBuffInfo;
		GuildBuffMgr::Instance()->GetBuff(pGuildTeam->nGuildID, setGuildBuffInfo);
		for (auto miter = setGuildBuffInfo.begin(); miter!=setGuildBuffInfo.end(); miter++)
		{
			GuildBuffInfo *pGuildBuffInfo = *miter;
			if (!pGuildBuffInfo) 
				continue;
			GuildBuff *pGuildBuff = pInfo->add_buff();
			pGuildBuff->set_id(pGuildBuffInfo->nID);
			if (pGuildBuffInfo->nTime > m_nTime)
				pGuildBuff->set_time(pGuildBuffInfo->nTime - m_nTime);
			else
				pGuildBuff->set_time(0);
		}	
	}

	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end(); iter++)
	{
		GuildTeam* pGuildTeam	= iter->second;
		if (pGuildTeam)
		{
			Guild *pOneGuild = CGuildMgr::Instance()->GetGuild(pGuildTeam->nGuildID);
			if (pOneGuild)
			{
				for (auto miter = pOneGuild->GetGuildMember().begin(); miter != pOneGuild->GetGuildMember().end(); miter++)
				{
					bool bJoin = GetRoleJoin(miter->first);
					if (bJoin)
					{
						CRole *pRole = CRoleManager::Instance()->GetByRoleID(miter->first);
						if (pRole)
							pRole->Send(ntf);
					}
				}
			}
		}
	}
}


void ResWarMgr::SetGuildResData(KKSG::ResWarBase &data)
{
	if (m_nStep!=eGuildResFlowExplore)
		return;
	UINT64 nGuildID = data.guildid();
	UINT32 nTeamID  = data.teamid();
	UINT32 nRes     = data.res();
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(nGuildID);
	if (!pGuild)
		return;
	GuildTeam* pGuildTeam =   GetGuildTeam(nGuildID);
	if (!pGuildTeam||!pGuildTeam->pGroupGuildInfo) 
		return;
	GuildFightTeam* pFightTeam = GetGuildFightTeam(nGuildID, nTeamID);
	if (!pFightTeam)
	{
		return;
	}

	pFightTeam->nState = eGuildResStateResult;
	for (int i = 0; i<data.itemlist().size(); i++)
	{
		UINT64 nRoleID = data.itemlist(i).roleid();
		UINT32 nItemID = data.itemlist(i).itemid();
		UINT32 nItemCount = data.itemlist(i).count();
		LogInfo("------SetGuildResData--GuildName=%s-----itemid=%d itemcont=%d--",pGuild->GetName().c_str(),  nItemID, nItemCount);
		CRoleSummary*pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(nRoleID);
		if (pSum)
		{
			GuildBuffTable::RowData *pRowData = ResWarAwardMgr::Instance()->GetGuildBuffItemID(nItemID);
			if (pRowData)
			{
				Notice *pNotice = new Notice(GuildResWarCard);
				pNotice->Replace("$R", pSum->GetName());
				pNotice->AddGuild(pGuildTeam->nGuildID);
				pNotice->Replace("{0}", pRowData->name);
				pGuildTeam->vecNotice.push_back(pNotice);

				Notice notice(GuildResWarCard);
				notice.Replace("$R", pSum->GetName());
				notice.AddGuild(pGuildTeam->nGuildID);
				notice.Replace("{0}", pRowData->name);
				notice.SendGuild(pGuildTeam->nGuildID);
			}
		}
		pGuildTeam->mapBuff[nItemID] += nItemCount;
	}
	for (int i = 0; i < data.rolelist_size(); i++)
	{
		UINT64 nRoleID = data.rolelist(i).role();
		UINT32 nRes    = data.rolelist(i).res();
		pGuildTeam->nResValue += nRes;
		
		LogInfo("------RoleResWar-------roleid=%llu nRes=%d--",nRoleID,  nRes);
		auto iter = pFightTeam->mapFightRole.find(nRoleID);
		if (iter!=pFightTeam->mapFightRole.end())
		{
			GuildFightRole *pFightRole = iter->second;
			if(pFightRole!=NULL)
			{
				pFightRole->nRes += nRes;
				AddRoleNotice(pGuildTeam, nRoleID, nRes);
			}else
			{
				LogWarn("Invaild FightRole ---roleid =%llu res=%u", nRoleID, nRes);
				pFightTeam->mapFightRole.erase(iter);
			}
		}

	}
	pFightTeam->vecBuff.clear();
	pFightTeam->vecMineral.clear();
	PtcM2C_ResWarGuildBriefNtf ntf;
	KKSG::ResWarGuildBrief *pBrief = &ntf.m_Data;
	GetResWarGuildBrief(pGuildTeam, pFightTeam, pBrief);
	GetNoticeInfo(pGuildTeam,pBrief);
	for (auto iter = pFightTeam->mapFightRole.begin(); iter != pFightTeam->mapFightRole.end(); iter++)
	{
		GuildFightRole* pFightRole = iter->second;
		if (pFightRole)
		{
			CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRole);
			if (pRole)
				pRole->Send(ntf);
		}
	}
	SyncGuildMineValue(pGuildTeam->pGroupGuildInfo, pGuildTeam->nGuildID, pGuildTeam->nResValue);
	
}


void ResWarMgr::SyncGuildMineValue(GroupGuildInfo *pGroup, UINT64 nGuildID, UINT32 nRes)
{
	if (!pGroup)
	{
		return;
	}
	PtcM2C_ResWarMineDataNtf reswarntf;
	reswarntf.m_Data.set_guildid(nGuildID);
	reswarntf.m_Data.set_mine(nRes);

	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end();iter++)
	{
		GuildTeam *pTargetTeam = iter->second;
		if (pTargetTeam)
		{
			Guild *pTargetGuild = CGuildMgr::Instance()->GetGuild(pTargetTeam->nGuildID);
			if (pTargetGuild)
			{
				for (auto miter = pTargetGuild->GetGuildMember().begin(); miter != pTargetGuild->GetGuildMember().end(); miter++)
				{
					bool bJoin = GetRoleJoin(miter->first);
					if (bJoin)
					{
						CRole *pRole = CRoleManager::Instance()->GetByRoleID(miter->first);
						if (pRole)
						{
							LogInfo("--------SetGuildResData---guild=%s----name=%s-------",pTargetTeam->szName.c_str(),pRole->GetName().c_str());
							pRole->Send(reswarntf);
						}
					}
				}
			}
		}
	}
}


void ResWarMgr::SyncGuildMine(GuildTeam *pGuildTeam)
{
	if (!pGuildTeam||!pGuildTeam->pGroupGuildInfo)
		return;
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(pGuildTeam->nGuildID);
	if(!pGuild)
		return;
	PtcM2C_ResWarGuildBriefNtf ntf;
	KKSG::ResWarGuildBrief *pBrief = &ntf.m_Data;
	pBrief->set_guildid(pGuildTeam->nGuildID);
	for (auto iter = pGuildTeam->mapTeam.begin(); iter!= pGuildTeam->mapTeam.end(); iter++)
	{
		GuildFightTeam *pFightTeam = iter->second;
		if (pFightTeam)
		{
			for (auto siter = pFightTeam->vecMineral.begin(); siter != pFightTeam->vecMineral.end(); siter++)
			{
				UINT32 nMineID = *siter;
				pBrief->add_mineid(nMineID);
			}
			for (auto miter = pFightTeam->mapFightRole.begin(); miter!=pFightTeam->mapFightRole.end(); miter++)
			{
				GuildFightRole *pFightRole = miter->second;
				if(pFightRole)
				{
					CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRole);
					if (pRole)
						pRole->Send(ntf);
				}
			}
		}
	}
}


void ResWarMgr::GetResWarGuildBrief(GuildTeam *pGuildTeam,GuildFightTeam *pFightTeam,KKSG::ResWarGuildBrief *pBrief)
{
	if (!pGuildTeam) return;
	UINT64 nGuildID		   = pGuildTeam->nGuildID;
	UINT32 nGroupID		   = GetGuildGroupID(nGuildID);
	GroupGuildInfo* pGroup = GetGuildGroup(nGroupID);
	if (!pGroup)
	{
		pBrief->set_error(ERR_RESWAR_GROUP);
		return;
	}
	pBrief->set_guildid(nGuildID);
	if (pFightTeam&&pFightTeam->nTime > m_nTime)
	{
		UINT32 nLeftTime = pFightTeam->nTime - m_nTime;
		pBrief->set_cd(nLeftTime);
	}else
	{
		pBrief->set_cd(0);
	}
	pBrief->set_timetype(m_nCountDownType);
	if (m_nCountDownTime > m_nTime)
	{
		pBrief->set_timecoutdown(m_nCountDownTime- m_nTime);

	}else
	{
		pBrief->set_timecoutdown(0);
	}

	if (pGuildTeam->nNextUseBuffTime>m_nTime)
	{
		pBrief->set_cardcd(pGuildTeam->nNextUseBuffTime-m_nTime);
	}else
	{
		pBrief->set_cardcd(0);
	}
	pBrief->set_totalcd(pGuildTeam->nCD);
	//buff背包
	for (auto iter = pGuildTeam->mapBuff.begin(); iter!=pGuildTeam->mapBuff.end(); iter++)
	{
		UINT32 nItemID = iter->first;
		UINT32 nItemCount = iter->second;
		if (nItemCount>0)
		{
			GuildBuffItem *pBuffItem = pBrief->add_item();
			pBuffItem->set_itemid(nItemID);
			pBuffItem->set_count(nItemCount);
		}
	}
	//矿点
	if (pFightTeam)
	{
		bool bHas = GuildBuffMgr::Instance()->IsUnknown(pGuildTeam->nGuildID);
		for (auto iter = pFightTeam->vecMineral.begin(); iter != pFightTeam->vecMineral.end(); iter++)
		{
			UINT32 nMineID = *iter;
			if (bHas)
			{
				pBrief->add_mineid(0);
			}else
			{
				pBrief->add_mineid(nMineID);
			}
		}
		for (auto iter = pFightTeam->vecBuff.begin(); iter!=pFightTeam->vecBuff.end(); iter++)
		{
			pBrief->add_buffid(*iter);
		}
	}
	pBrief->set_error(ERR_SUCCESS);
}

GroupGuildInfo* ResWarMgr::GetGuildGroup(UINT32 nGroupID)
{
	return  m_mapGroupGuildInfo[nGroupID];
}

UINT64 ResWarMgr::GetGuildID(Team *pTeam)
{
	if (!pTeam) 
		return 0;
	UINT64 nLeaderID = pTeam->GetLeader();
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(nLeaderID);
	if (pGuild)
		return pGuild->GetID();
	return 0;
}

UINT32 ResWarMgr::GetGuildLv(Team *pTeam)
{
	if (!pTeam)
		return 0;
	UINT64 nLeaderID = pTeam->GetLeader();
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(nLeaderID);
	if (pGuild)
		return pGuild->GetLevel();
	return 0;
}

KKSG::ErrorCode ResWarMgr::Check(Team *pTeam)
{
	if (!m_bOpen)
		return KKSG::ERR_RESWAR_ACTIVITY;
	UINT64 nGuildID = GetGuildID(pTeam);
	UINT32 nGroupID = GetGuildGroupID(nGuildID);
	if (nGroupID == 0) 
		return KKSG::ERR_RESWAR_GROUP;
	GroupGuildInfo *pGroup = m_mapGroupGuildInfo[nGroupID];
	if (!pGroup)
		return KKSG::ERR_RESWAR_GROUP;
	return KKSG::ERR_SUCCESS;
}


KKSG::ErrorCode ResWarMgr::Explore(Team *pTeam)
{
	if (!pTeam)
		return KKSG::ERR_RESWAR_TEAM;
	KKSG::ErrorCode nRet = Check(pTeam);
	if (nRet != KKSG::ERR_SUCCESS)
		return nRet;
	UINT32 nTeamID  = pTeam->GetID();
	UINT64 nGuildID = GetGuildID(pTeam);
	UINT32  nLv     = GetGuildLv(pTeam);
	if (nLv<2)
		return ERR_RESWAR_GROUP;
	GuildTeam      *pGuildTeam		= GetGuildTeam(nGuildID);
	if (!pGuildTeam) 
		KKSG::ERR_RESWAR_GROUP;
	GuildFightTeam *pGuildFightTeam = GetGuildFightTeam(nGuildID,nTeamID);
	if (!pGuildFightTeam)
	{
		pGuildFightTeam = AddTeam(pTeam);
		if (!pGuildFightTeam)
			return KKSG::ERR_RESWAR_GROUP;
	}
	pGuildTeam->mapTeam[nTeamID] = pGuildFightTeam;
	pGuildFightTeam->pGuildTeam   = pGuildTeam;
	m_mapExploreTeam[nTeamID]	=	pGuildFightTeam;
	pGuildFightTeam->nTime		= m_nTime + pGuildTeam->nCD;
	pGuildFightTeam->vecMineral.clear();
	pGuildFightTeam->vecBuff.clear();
	pGuildFightTeam->nState		= eGuildResStateExploreIng;
	PtcM2C_ResWarStateNtf statentf;
	statentf.m_Data.set_state(KKSG::ResWarExploreState);
	PtcM2C_ResWarGuildBriefNtf ntf;
	GetResWarGuildBrief(pGuildTeam, pGuildFightTeam, &ntf.m_Data);
	for (auto miter = pGuildFightTeam->mapFightRole.begin(); miter!=pGuildFightTeam->mapFightRole.end();miter++)
	{
		GuildFightRole* pFightRole = miter->second;
		if (pFightRole)
		{
			CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRole);
			if (pRole)
			{
				pRole->Send(ntf);
				pRole->Send(statentf);
			}
		}
	}
	return KKSG::ERR_SUCCESS;
}


void ResWarMgr::BuildMineralBattle(GuildFightTeam *pFightTeam)
{
	if (!pFightTeam) 
		return;
	UINT32 nLevelSeal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	std::map<UINT32,UINT32> mapMineral;
	pFightTeam->vecMineral.clear();
	pFightTeam->vecBuff.clear();
	UINT32 nTotal =  ResWarAwardMgr::Instance()->GetMineralLevelSeal(nLevelSeal, mapMineral);
	for (UINT32 i = 0; i < GetGlobalConfig().ResWarPVEMineSize; i++)
	{
		UINT32 nRand  = XRandom::randInt(1, nTotal);
		for (auto iter = mapMineral.begin(); iter!=mapMineral.end(); iter++)
		{
			UINT32 nID		= iter->first;
			UINT32 nWeight	= iter->second;
			if (nRand <= nWeight)
			{
				pFightTeam->vecMineral.push_back(nID);
				GuildMineralBufflist::RowData* pRowData = ResWarAwardMgr::Instance()->GetRandBuffList();
				if (pRowData)
				{
					pFightTeam->vecBuff.push_back(pRowData->BuffID);
				}
				LogInfo("BuildMineralBattle mineid=%d", nID);
				break;
			}
		}
	}

	LogInfo("BuildMineralBattle--teamid=%d guildid=%llu", pFightTeam->nTeamID,pFightTeam->nGuildID);
}

bool ResWarMgr::CheckGroup(GuildFightTeam *pSelfTeam,GuildFightTeam *pTargetTeam)
{
	if (pSelfTeam==pTargetTeam)
	{
		return false;
	}
	if (pSelfTeam&&pTargetTeam)
	{
		if (pSelfTeam->pGuildTeam&&pTargetTeam->pGuildTeam)
		{
			if (pSelfTeam->pGuildTeam->pGroupGuildInfo&&pSelfTeam->pGuildTeam->pGroupGuildInfo==pTargetTeam->pGuildTeam->pGroupGuildInfo)
			{
				if (pSelfTeam->pGuildTeam->nGuildID != pTargetTeam->pGuildTeam->nGuildID)
					return true;
				return false;
			}
		}
	}
	return false;
}

KKSG::ErrorCode ResWarMgr::CancelExplore(Team *pTeam)
{
	if (!pTeam) 
		return KKSG::ERR_RESWAR_TEAM;
	if (!m_bOpen)
		return KKSG::ERR_RESWAR_ACTIVITY;
	UINT64 nGuildID = GetGuildID(pTeam);
	int nGroupID	= GetGuildGroupID(nGuildID);
	if (nGroupID == -1) 
		return KKSG::ERR_RESWAR_GROUP;
	GroupGuildInfo *pGroup = m_mapGroupGuildInfo[nGroupID];
	if (!pGroup) 
		return KKSG::ERR_RESWAR_GROUP;
	UINT32 nTeamID  = pTeam->GetID();
	GuildFightTeam *pSelf = GetGuildFightTeam(nGuildID,nTeamID);
	if (!pSelf)
		return KKSG::ERR_RESWAR_GROUP;
	if (pSelf->nState!=eGuildResStateExploreIng)
		return KKSG::ERR_RESWAR_STATE;
	pSelf->nState = eGuildResStateIdle;
	pSelf->nTime  = 0;
	for (auto iter = m_mapExploreTeam.begin(); iter != m_mapExploreTeam.end(); )
	{
		GuildFightTeam *pFightTeam = iter->second;
		if (pFightTeam->nTeamID == nTeamID)
		{
			m_mapExploreTeam.erase(iter++);
		}else
		{
			iter++;
		}
	}
	PtcM2C_ResWarGuildBriefNtf ntf;
	PtcM2C_ResWarStateNtf statentf;
	statentf.m_Data.set_state(KKSG::ResWarCancelState);
	GetResWarGuildBrief(pSelf->pGuildTeam, pSelf, &ntf.m_Data);
	for (auto miter = pSelf->mapFightRole.begin(); miter!=pSelf->mapFightRole.end();miter++)
	{
		GuildFightRole* pFightRole = miter->second;
		if (pFightRole)
		{
			CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRole);
			if (pRole)
			{
				pRole->Send(ntf);
				pRole->Send(statentf);
			}
		}
	}

	return KKSG::ERR_SUCCESS;
}

void ResWarMgr::QueryFightRank(UINT32 nID, KKSG::ResWarRoleRankRes &res)
{
	for (auto iter = m_mapGuildWarRoleRank[nID].begin();iter != m_mapGuildWarRoleRank[nID].end();iter++)
	{
		KKSG::ResWarRoleRank  *pRank = res.add_data();
		GuildWarRoleRank rank = *iter;
		pRank->set_rolename(rank.szName);
		pRank->set_roleid(rank.nRoleID);
		pRank->set_res(rank.nResValue);
		pRank->set_guildname(rank.szGuildName);
		pRank->set_guild(rank.nGuildID);
	}
}


void ResWarMgr::SortFightRoleLess(UINT32 nID)
{
	GroupGuildInfo *pGroup = m_mapGroupGuildInfo[nID];
	if(!pGroup) 
		return;
	m_mapGuildWarRoleRank[nID].clear();
	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end();iter++)
	{
		GuildTeam *pGuildTeam = iter->second;
		if (pGuildTeam)
		{
			for (auto miter = pGuildTeam->mapTeam.begin(); miter!=pGuildTeam->mapTeam.end();miter++)
			{
				GuildFightTeam *pGuildFightTeam =  miter->second;
				if (pGuildFightTeam)
				{
					for (auto riter = pGuildFightTeam->mapFightRole.begin(); riter != pGuildFightTeam->mapFightRole.end(); riter++)
					{
						GuildFightRole *pGuildFightRole = riter->second;
						if (pGuildFightRole!=NULL)
						{
							GuildWarRoleRank rank;
							rank.nRoleID   = pGuildFightRole->nRole;
							rank.nResValue = pGuildFightRole->nRes;
							rank.szName    = pGuildFightRole->szName;
							rank.szGuildName = pGuildTeam->szName;
							rank.nGuildID   = pGuildTeam->nGuildID;
							m_mapGuildWarRoleRank[nID].push_back(rank);
						}
					}
				}

			}
		}
	}
	std::sort(m_mapGuildWarRoleRank[nID].begin(),m_mapGuildWarRoleRank[nID].end(),GuildWarRoleRankCmp());
}


void ResWarMgr::SortGuildWarRankLess(UINT32 nID)
{
	auto siter = m_mapGroupGuildInfo.find(nID);
	if (siter==m_mapGroupGuildInfo.end()||siter->second==NULL)
		return;
	GroupGuildInfo *pGroup = siter->second;
	m_mapGuildWarRank[nID].clear();
	for (auto iter = pGroup->mapGroupGuild.begin(); iter!=pGroup->mapGroupGuild.end();iter++)
	{
		GuildTeam *pGuildTeam = iter->second;
		if (pGuildTeam)
		{
			LogInfo("--SortGuildWarRankLess--- nGuildID=%llu nPrestige=%d nResValue=%d szName=%s",
				pGuildTeam->nGuildID,pGuildTeam->nPrestige, pGuildTeam->nResValue,pGuildTeam->szName.c_str());
			GuildWarRank rank;
			rank.nGuildID = pGuildTeam->nGuildID;
			rank.nPrestige = pGuildTeam->nPrestige;
			rank.nResValue = pGuildTeam->nResValue;
			rank.szName    = pGuildTeam->szName;
			m_mapGuildWarRank[nID].push_back(rank);
		}
	}
	std::sort(m_mapGuildWarRank[nID].begin(),m_mapGuildWarRank[nID].end(),GuildWarRankCmp());
}


void ResWarMgr::SetFightTime()
{
	for (auto iter = m_mapExploreTeam.begin(); iter!=m_mapExploreTeam.end(); iter++)
	{
		GuildFightTeam *pFightTeam = iter->second;
		pFightTeam->nTime = m_nTime + 20; 
	}
}



void ResWarMgr::SendEnemyAttack(GroupGuildFightTeam *pGroupGuildFightTeam)
{
	if (!pGroupGuildFightTeam)
		return;
	pGroupGuildFightTeam->nFightTime = m_nTime + GetGlobalConfig().ResWarEnemyAttack;
	PtcM2C_ResWarEnemyTimeNtf ntf;
	std::vector<GuildFightTeam *> vecGuilFightTeam;
	vecGuilFightTeam.push_back(pGroupGuildFightTeam->pSelf);
	vecGuilFightTeam.push_back(pGroupGuildFightTeam->pTarget);
	for(auto siter = vecGuilFightTeam.begin(); siter!=vecGuilFightTeam.end();siter++)
	{
		GuildFightTeam *pFightTeam = *siter;
		if (pFightTeam)
		{
			for(auto iter = pFightTeam->mapFightRole.begin(); iter!=pFightTeam->mapFightRole.end();iter++)
			{
				GuildFightRole* pFightRole = iter->second;
				if(pFightRole)
				{
					CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRole);
					if (pRole)
					{
						pRole->Send(ntf);
					}
				}
			}
		}
	}
}

void ResWarMgr::ExploreRes()
{
	std::vector<GuildFightTeam*> vecFightTeam;
	for (auto iter = m_mapExploreTeam.begin(); iter!=m_mapExploreTeam.end();)
	{
		GuildFightTeam *pFightTeam = iter->second;
		if (m_nTime >= pFightTeam->nTime  && pFightTeam->nState == eGuildResStateExploreIng)
		{
			pFightTeam->nState = eGuildResStateExploreEnd;
			vecFightTeam.push_back(pFightTeam);
			m_mapExploreTeam.erase(iter++);
		}else
		{
			iter++;
		}
	}
	for (UINT32 i = 0; i<vecFightTeam.size();i++)
	{
		GuildFightTeam *pFightTeam = vecFightTeam[i];
		if (!pFightTeam)
			continue;
		if(pFightTeam->nState == eGuildResStateMatch)
			continue;
		for (UINT32 j = vecFightTeam.size()-1; j>i; j--)
		{
			GuildFightTeam *pTargetTeam = vecFightTeam[j];
			if (!pTargetTeam)
				continue;
			if(pTargetTeam->nState == eGuildResStateMatch) 
				continue;
			bool bOk = CheckGroup(pFightTeam,pTargetTeam);
			if(!bOk)
				continue;
			float fRate = 0.0;
			if (pFightTeam->nFight > pTargetTeam->nFight)
				fRate = (float)pFightTeam->nFight/(float)pTargetTeam->nFight;
			else
				fRate = (float)pTargetTeam->nFight/(float)pFightTeam->nFight;
			if (fRate<GetGlobalConfig().ResWarFightRate&&GetGlobalConfig().ResWarPullGroup>0)
			{
				GroupGuildFightTeam *pGroupGuildFightTeam = new GroupGuildFightTeam();
				pGroupGuildFightTeam->pSelf   = pFightTeam;
				pGroupGuildFightTeam->pTarget = pTargetTeam;
				pGroupGuildFightTeam->nUniID = ++m_nUniID;
				pFightTeam->vecMineral.clear();
				pFightTeam->nState            = eGuildResStateMatch;
				pTargetTeam->nState           = eGuildResStateMatch;
				SendEnemyAttack(pGroupGuildFightTeam);
				pTargetTeam->vecMineral.clear();
				m_vecMatchPVPGroup.push_back(pGroupGuildFightTeam); 
				LogInfo("----------ExploreRes---------------nSelfGuildID=%llu nSelfTeamID=%d nTargetGuildID=%llu nTargetTeamID=%d nGroupID=%d",
					pFightTeam->nGuildID,pFightTeam->nTeamID,pTargetTeam->nGuildID,pTargetTeam->nTeamID,pGroupGuildFightTeam->nUniID);
				break;	
			}

		}		
	}
	for (auto iter = vecFightTeam.begin(); iter != vecFightTeam.end();iter++)
	{
		GuildFightTeam *pFightTeam = *iter;
		if (!pFightTeam ||!pFightTeam->pGuildTeam)
			continue;
		if (pFightTeam->nState == eGuildResStateMatch)
			continue;
		LogInfo("-----ExploreRes----------guildid=%llu teamid=%d guildname=%s",pFightTeam->nGuildID, pFightTeam->nTeamID, pFightTeam->pGuildTeam->szName.c_str());
		pFightTeam->nUniID = ++m_nUniID;	
		pFightTeam->nState = eGuildResStateMatch;
		m_vecMatchPVE.push_back(pFightTeam); 
		BuildMineralBattle(pFightTeam);
		PtcM2C_ResWarGuildBriefNtf ntf;
		GetResWarGuildBrief(pFightTeam->pGuildTeam, pFightTeam, &ntf.m_Data);
		for (auto miter = pFightTeam->mapFightRole.begin(); miter!=pFightTeam->mapFightRole.end();miter++)
		{
			GuildFightRole* pFightRole = miter->second;
			if (pFightRole&&pFightRole->bLost==false)
			{
				CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRole);
				if (pRole)
				{
					pRole->Send(ntf);
				}
			}
		}
	}
}


void ResWarMgr::Match()
{
	GuildSorter &sorter = CGuildMgr::Instance()->GetSorter();
	std::vector<Guild *> &vecGuild = sorter.GetSortResult(KKSG::GuildSortByPrestige);
	UINT32 nTotal = 0;
	int nIndex = 0;

	std::map<UINT64, SimpleGuildInfo> mapGuildIDName;
	std::vector<UINT64> vectMatchGuildID;
	for (UINT32 j = 0;j<vecGuild.size();j++)
	{
		Guild *pGuild	= vecGuild[j];
		if (pGuild&&pGuild->GetLevel()>=GetGlobalConfig().ResWarGuildLv)
		{
			UINT64 nGuildID = pGuild->GetID();
			vectMatchGuildID.push_back(nGuildID);
			mapGuildIDName[nGuildID].szName = pGuild->GetName();
			mapGuildIDName[nGuildID].nIcon  = pGuild->GetIcon();
			mapGuildIDName[nGuildID].nPrestige = pGuild->GetPrestige();
			LogInfo("----Match--guildid=%llu-guildname=%s-level=%d-prestige=%d",nGuildID ,mapGuildIDName[nGuildID].szName.c_str(),pGuild->GetLevel(),mapGuildIDName[nGuildID].nPrestige);
		}
	}

	std::vector<UINT64> vecGuildID;
	for (UINT32 i = 0; i<vectMatchGuildID.size()/MAX_GUILD_WAR_GROUP; i++)
	{
		vecGuildID.clear();
		for (UINT32 k = 0; k < MAX_GUILD_WAR_GROUP; k++)
		{
			vecGuildID.push_back(vectMatchGuildID[i*MAX_GUILD_WAR_GROUP+k]);
		}
		for (UINT32 j = 0; j<GUILD_MAX_TEAM;j++)
		{
			nIndex++;
			m_mapGroupGuildInfo[nIndex]						= new  GroupGuildInfo();
			m_mapGroupGuildInfo[nIndex]->nGroupID           = nIndex;
		}
		std::random_shuffle(vecGuildID.begin(), vecGuildID.end());
		for (UINT32 j = 0; j<vecGuildID.size(); j++)
		{
			UINT64 nGuildID =  vecGuildID[j];
			UINT32 nPos   = nTotal/GUILD_MAX_TEAM+1;
			if (m_mapGroupGuildInfo[nPos])
			{
				m_mapGroupGuildInfo[nPos]->mapGroupGuild[nGuildID]			  = new GuildTeam();
				m_mapGroupGuildInfo[nPos]->mapGroupGuild[nGuildID]->nGuildID  = nGuildID;
				m_mapGroupGuildInfo[nPos]->mapGroupGuild[nGuildID]->szName    = mapGuildIDName[nGuildID].szName;
				m_mapGroupGuildInfo[nPos]->mapGroupGuild[nGuildID]->nPrestige = mapGuildIDName[nGuildID].nPrestige;
				m_mapGroupGuildInfo[nPos]->mapGroupGuild[nGuildID]->nIcon     = mapGuildIDName[nGuildID].nIcon;
				m_mapGroupGuildInfo[nPos]->mapGroupGuild[nGuildID]->nCD       = GetGlobalConfig().ResWarExploreTime;
				m_mapGroupGuildInfo[nPos]->mapGroupGuild[nGuildID]->pGroupGuildInfo = m_mapGroupGuildInfo[nPos];
				LogInfo("MatchFull ------groupid=%d guilid=%llu guildname=%s",nPos,nGuildID,mapGuildIDName[nGuildID].szName.c_str());
				nTotal++;
			}
		}
	}
	int leftCount = ((vectMatchGuildID.size() - nTotal)/GUILD_MAX_TEAM)*GUILD_MAX_TEAM;
	if (leftCount)
	{
		vecGuildID.clear();
		for (UINT32 j = nTotal;j<vectMatchGuildID.size();j++)
		{
			vecGuildID.push_back(vectMatchGuildID[j]);
			leftCount--;
			if (leftCount <= 0)
			{
				break;
			}
		}
		GroupGuildInfo *pGroupGuild = NULL;
		for (auto iter=vecGuildID.begin();iter!=vecGuildID.end();iter++)
		{
			UINT64 nGuildID = *iter;
			if(nTotal%GUILD_MAX_TEAM==0)
			{
				nIndex++;
				pGroupGuild = new  GroupGuildInfo();
				m_mapGroupGuildInfo[nIndex]	= pGroupGuild;
			}
			if(!pGroupGuild)
				continue;
			pGroupGuild->nGroupID                           = nIndex;
			GuildTeam *pGuildTeam =  new GuildTeam();
			pGroupGuild->mapGroupGuild[nGuildID]			= pGuildTeam;
			pGuildTeam->pGroupGuildInfo = pGroupGuild;
			pGuildTeam->nGuildID  = nGuildID;
			pGuildTeam->szName    = mapGuildIDName[nGuildID].szName;
			pGuildTeam->nCD       = GetGlobalConfig().ResWarExploreTime;
			pGuildTeam->nPrestige = mapGuildIDName[nGuildID].nPrestige;
			pGuildTeam->nIcon     = mapGuildIDName[nGuildID].nIcon;
			LogInfo("MatchNoFull ------groupid=%d guilid=%llu guildname=%s",nIndex,pGuildTeam->nGuildID,pGuildTeam->szName.c_str());
			nTotal++;
		}
	}
}

void ResWarMgr::SetIconState(CRole *pRole)
{
	if (pRole==NULL)
		return;
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild)
	{
		GuildTeam* pGuildTeam	   = GetGuildTeam(pGuild->GetID());
		if (pGuildTeam)
		{
			 GuildFightRole *pFRole = GetFightRole(pGuildTeam,pRole->GetID());
			 if (pFRole)
				 pFRole->bIcon = false;
		}
	}
}

GuildFightTeam* ResWarMgr::AddTeam(Team *pTeam)
{
	UINT64 nGuildID					= GetGuildID(pTeam);
	UINT32 nTeamID					= pTeam->GetID();
	GuildFightTeam *pGuildFightTeam	= GetGuildFightTeam(nGuildID,nTeamID);
	if (pGuildFightTeam)
		return pGuildFightTeam;
	GuildTeam* pGuildTeam		= GetGuildTeam(nGuildID);
	GroupGuildInfo *pGroup		= pGuildTeam->pGroupGuildInfo;
	if (pGroup&&pGuildTeam)
	{
		pGuildFightTeam					= new GuildFightTeam();
		pGuildFightTeam->pGuildTeam		= pGuildTeam; 
		pGuildTeam->mapTeam[nTeamID]	= pGuildFightTeam;
		pGuildFightTeam->nGuildID       = nGuildID;
		pGuildFightTeam->nTeamID        = nTeamID;
		for (auto smiter = pTeam->GetMember().begin(); smiter!=pTeam->GetMember().end();smiter++)
		{
			CTeamMember *pMember = *smiter;
			UINT64 nRoleID		 = pMember->m_qwRoleID;
			CRoleSummary* pSum	 = CRoleSummaryMgr::Instance()->GetRoleSummary(pMember->m_qwRoleID);
			if (pSum)
			{
				GuildFightRole *pFightRole=GetFightRole(pGuildTeam, nRoleID);
				if(pFightRole)
				{
					GuildFightTeam *pOldTeam = pFightRole->pGuildFightTeam;
					if (pOldTeam)
					{
						auto iter = pOldTeam->mapFightRole.find(pFightRole->nRole);
						if (iter != pOldTeam->mapFightRole.end())
							pOldTeam->mapFightRole.erase(iter);
						if (pOldTeam->nFight>pSum->GetMaxPPT())
							pOldTeam->nFight -= pSum->GetMaxPPT();
					}
				}else
				{
					pFightRole		   =  new GuildFightRole();
					pFightRole->nFight = pSum->GetMaxPPT();
					pFightRole->nJob   = pSum->GetProfession();
					pFightRole->nLevel = pSum->GetLevel();
					pFightRole->nRole  = pSum->GetID();
					pFightRole->szName = pSum->GetName();
				}
				pGuildFightTeam->nFight += pSum->GetMaxPPT();
				pGuildFightTeam->mapFightRole[nRoleID] =  pFightRole;
				pFightRole->pGuildFightTeam = pGuildFightTeam;
				pFightRole->bLost = false;
				LogInfo("AddTeam=====Roleid=%lld name=%s level=%d ppt=%d job=%d",nRoleID, pFightRole->szName.c_str(), pFightRole->nLevel,pFightRole->nFight,pFightRole->nJob);
			}
		}
		return pGuildFightTeam;
	}
	return NULL;
}

UINT32 ResWarMgr::IsInResWar(UINT64 nGuildID)
{
	if (m_bOpen)
	{
		bool bRet = GetGuildGroupID(nGuildID);
		return bRet;
	}
	return 0;
}

UINT32 ResWarMgr::GetGuildGroupID(UINT64 nGuildID)
{
	if(nGuildID==0) 
		return 0;
	for (auto iter = m_mapGroupGuildInfo.begin();  iter!=m_mapGroupGuildInfo.end();iter++)
	{
		GroupGuildInfo *pGuildInfo = iter->second;
		if (pGuildInfo)
		{
			for (auto miter = pGuildInfo->mapGroupGuild.begin(); miter!=pGuildInfo->mapGroupGuild.end();miter++)
			{
				GuildTeam  *pGuildTeam = miter->second;
				if (pGuildTeam)
				{
					if (pGuildTeam->nGuildID == nGuildID)
						return pGuildInfo->nGroupID;
				}
			}
		}
	}
	return 0;
}

GroupGuildFightTeam* ResWarMgr::GetPVPGroupFightTeam(UINT32 nUniID)
{
	GroupGuildFightTeam *pGroup  = NULL;
	for (auto iter = m_vecReportGroup.begin();  iter!=m_vecReportGroup.end();iter++)
	{
		pGroup = *iter;
		if (pGroup->nUniID == nUniID)
			return pGroup;
	}
	return NULL;
}

GuildFightTeam* ResWarMgr::GetGuildFightTeam(UINT64 nGuildID,UINT32 nTeamID)
{
	for (auto iter = m_mapGroupGuildInfo.begin();  iter!=m_mapGroupGuildInfo.end();iter++)
	{
		GroupGuildInfo *pGroupInfo = iter->second;
		if(pGroupInfo)
		{
			if (pGroupInfo->mapGroupGuild[nGuildID])
			{
				GuildTeam* pGuildTeam = pGroupInfo->mapGroupGuild[nGuildID];
				if(pGuildTeam->mapTeam[nTeamID])
					return pGuildTeam->mapTeam[nTeamID];
			}
		}
	}
	return NULL;
}

void ResWarMgr::LeaveTeam(Team *pTeam,UINT64 nRole)
{
	if (!pTeam||!m_bOpen)
	{
		return;
	}
	UINT32 nDNEID = ResWarMgr::Instance()->GetResWarPVDNEID();
	if (nDNEID == pTeam->GetConf()->DNExpeditionID)
	{
		for (auto iter = m_mapGroupGuildInfo.begin(); iter!= m_mapGroupGuildInfo.end(); iter++)
		{
			GroupGuildInfo *pGroup = iter->second;
			if (!pGroup) 
				continue;
			for (auto miter = pGroup->mapGroupGuild.begin(); miter !=pGroup->mapGroupGuild.end();miter++)
			{
				GuildTeam* pGuildTeam = miter->second;
				if (!pGuildTeam) 
					continue;
				auto titer = pGuildTeam->mapTeam.find(pTeam->GetID());
				if (titer!=pGuildTeam->mapTeam.end())
				{
					GuildFightTeam *pFightTeam = titer->second;
					if (pFightTeam)
					{
						auto siter = pFightTeam->mapFightRole.find(nRole);
						if (siter!=pFightTeam->mapFightRole.end())
						{
							GuildFightRole *pFightRole = siter->second;
							if(pFightRole)
							{
								pFightRole->bLost = true;
							}						
						}
					}
				}
			}
		}
	}
}


void ResWarMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	m_nTime   = GameTime::GetTime();
	switch (m_nStep)
	{
	case eGuildResFlowIdle:
		{
			UINT32 nOpenTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILDRESWAR);
			UINT32 nEndTime =  MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILDRESWAR);
			time_t nDiff  =  (GameTime::GetWeekBeginTime(m_nTime) -  GameTime::GetWeekBeginTime(MSConfig::Instance()->GetGameServerOpenTimeStamp()))/(7*24*3600) + 1;
			MultActivityConf* pCfg = MultActivityMgr::Instance()->GetActivityConfig(MULT_ACTIVITY_GUILDRESWAR);
			if (pCfg)
			{
				if (nDiff >= pCfg->openServerWeek)
				{
					UINT32 nNoticeTime = nOpenTime - GetGlobalConfig().ResWarNoticeTime;
					if (m_nTime>=nNoticeTime&&m_nTime<nEndTime&&m_bNotice)
					{
						LogInfo("Notice --- ResWarMgr::OnTimer");
						m_bNotice= false;
						Notice notice(GuildResWarSignNotice);
						notice.Send();
					}
				}
			}
			if (m_bOpen)
			{
				LogInfo("ResWar Start");
				m_bAward = true;
				m_nStep = eGuildResFlowGroup;
				m_nNextTime = m_nTime + GetGlobalConfig().ResWarWaitingTime;
				m_nDurationTime = m_nTime;
				m_nCountDownType = eResWarReady;
				m_nCountDownTime = m_nTime + GetGlobalConfig().ResWarWaitingTime;
				Match();
				SendGuildRoleIcon(eResWarMsgTypeNotice);
				SendGuildRoleIcon(eResWarMsgTypeIcon);
			}
		}
		break;
	case eGuildResFlowGroup:
		{
			m_nEndTime = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILDRESWAR);
			if (m_nTime >= m_nCountDownTime)
			{
				m_nCountDownType = eResWarEnd;
				m_nCountDownTime = m_nEndTime;
				SendGuildRoleIcon(eResWarMsgTypeTime);
				SendGuildRoleIcon(eResWarOpen5Min);
				m_nStep = eGuildResFlowExplore;
				LogInfo("ResWar Explore");
			}
		}
		break;
	case eGuildResFlowExplore:
		{
			if(m_nTime >= m_nSyReadyTime)
			{
				m_nSyReadyTime = m_nTime +  3;
				SendGuildRoleIcon(eResWarMsgTypeTime);
			}

			if(m_nTime >= m_nEndTime-5*60 && m_bFinal5Min)
			{
				m_bFinal5Min = false;
				SendGuildRoleIcon(eResWarFinal5Min);
			}
			if (m_nTime<=m_nEndTime)
			{
				ExploreRes();
				StartBattlePVP();
			}else
			{
				m_nStep		= eGuildResFlowAward;
			}
		}
		break;
	case eGuildResFlowAward: 
		{
			if (m_bAward)
			{
				SendAllIcon(SYS_RESWAR,KKSG::HICONS_END);
				SendGuildRoleIcon(eResWarAwardTypeIcon);
				GiveFinalAward();
				m_bAward =false;
				m_bOpen			= false;
				LogInfo("ResWar End");
			}
			if (m_nTime > m_nEndTime + GetGlobalConfig().ResWarDestoryTime)
			{
				m_nStep = eGuildResFlowEnd;
			}
		}
		break;
	case eGuildResFlowEnd:
		{
			SendAllIcon(SYS_RESWAR_AWARD,KKSG::HICONS_END);
			Destory();
			m_nStep = eGuildResFlowIdle;
		}
		break;
	default:
		{}
	}
	
}


void ResWarMgr::Destory()
{
	m_nDurationTime = 0;
	m_nUniID		= 0;
	m_bAward = false;
	m_bNotice  = true;
	m_bFinal5Min = true;
	m_nSyReadyTime = 0;
	m_vecReportGroup.clear();
	GuildBuffMgr::Instance()->Clear();
	for (auto iter=m_vecMatchPVPGroup.begin(); iter!=m_vecMatchPVPGroup.end(); iter++)
	{
		GroupGuildFightTeam* pGroup =*iter;
		if (pGroup)
			delete pGroup;
	}
	m_vecMatchPVPGroup.clear();
	m_vecMatchPVE.clear();
	m_mapExploreTeam.clear();
	m_mapGuildWarRoleRank.clear();
	m_mapGuildWarRank.clear();
	for (auto iter = m_mapGroupGuildInfo.begin(); iter!= m_mapGroupGuildInfo.end(); iter++)
	{
		GroupGuildInfo *pGroup = iter->second;
		if (!pGroup) 
			continue;
		for (auto miter = pGroup->mapGroupGuild.begin(); miter !=pGroup->mapGroupGuild.end();miter++)
		{
			 GuildTeam* pGuildTeam = miter->second;
			 if (!pGuildTeam) 
				 continue;
			 for (auto niter = pGuildTeam->vecNotice.begin(); niter != pGuildTeam->vecNotice.end(); niter++)
			 {
				 Notice *pNotice = *niter;
				 delete pNotice;
			 }
			 if(pGuildTeam->vecNotice.size())
			 {
				 pGuildTeam->vecNotice.clear();
			 }
			 for (auto siter = pGuildTeam->mapTeam.begin(); siter!= pGuildTeam->mapTeam.end(); siter++)
			 {
				 GuildFightTeam *pFightTeam =siter->second;
				 if (!pFightTeam) 
					 continue;
				 for (auto titer = pFightTeam->mapFightRole.begin(); titer!=pFightTeam->mapFightRole.end();titer++)
				 {
					 GuildFightRole* pFightRole = titer->second;
					 if (pFightRole)
					 {
						delete pFightRole;
					 }
				 }
				 delete pFightTeam;
			 }
			 delete pGuildTeam;
		}
		delete pGroup;
	}
	m_mapGroupGuildInfo.clear();
	m_setRoleID.clear();
}


void ResWarMgr::AddResWarTeamBaseInfo(GuildFightTeam *pFightTeam, KKSG::ResWarSceneInfo *pSceneInfo,bool bSelf)
{
	if (!pFightTeam||!pFightTeam->pGuildTeam) 
		return;
	for (auto iter = pFightTeam->mapFightRole.begin(); iter!= pFightTeam->mapFightRole.end(); iter++)
	{
		GuildFightRole *pFightRole = iter->second;
		if (!pFightRole||pFightRole->bLost) 
			continue;
		CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRole);
		if (pRole)
		{
			UINT32 nTeamID = pRole->GetTeamID();
			if (nTeamID!=0&&nTeamID==pFightTeam->nTeamID)
			{
				ResWarTeamBaseInfo *pTeamBase = NULL;
				if (bSelf)
					pTeamBase = pSceneInfo->add_self();
				else
					pTeamBase = pSceneInfo->add_target();
				pTeamBase->set_uid(pFightRole->nRole);
				pTeamBase->set_job(pFightRole->nJob);
				pTeamBase->set_lv(pFightRole->nLevel);
				pTeamBase->set_ppt(pFightRole->nFight);
				pTeamBase->set_name(pFightRole->szName);
				pTeamBase->set_teamid(pFightTeam->nTeamID);
				pTeamBase->set_online(pFightRole->bLost);
				pTeamBase->set_guildid(pFightTeam->nGuildID);
				pTeamBase->set_guildname(pFightTeam->pGuildTeam->szName);
			}
		}
	}
}

bool ResWarMgr::CheckFightState(Team *pTeam, UINT64 nRole)
{
	if (!pTeam||!m_bOpen)
	{
		return true;
	}
	UINT32 nDNEID = ResWarMgr::Instance()->GetResWarPVDNEID();
	if (nDNEID == pTeam->GetConf()->DNExpeditionID)
	{
		UINT64 nGuildID = GetGuildID(pTeam);
		UINT32 nTeamID  = pTeam->GetID();
		GuildFightTeam *pFightTeam = GetGuildFightTeam(nGuildID,nTeamID);
		if (pFightTeam&&pFightTeam->nState==eGuildResStateFight)
		{
			return false;
		}
	}
	return true;
}


UINT32 ResWarMgr::GetTeamOnline(GuildFightTeam *pFightTeam)
{
	UINT32 nRet = 0;
	if (pFightTeam == NULL)
	{
		return nRet;
	}
	for(auto iter = pFightTeam->mapFightRole.begin(); iter!=pFightTeam->mapFightRole.end(); iter++)
	{
		GuildFightRole* pFightRole = iter->second;
		if (!pFightRole->bLost)
		{
			nRet++;
		}
	}
	return nRet;
}

bool ResWarMgr::CheckTeam(Team *pTeam)
{
	if (pTeam)
	{
		if (pTeam->GetMember().empty()==false)
		{
			Guild *pTeamGuild = CGuildMgr::Instance()->GetGuild(pTeam->GetGuildID());
			for (auto iter = pTeam->GetMember().begin();iter!=pTeam->GetMember().end(); iter++ )
			{
				Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId((*iter)->RoleID());
				if (pGuild==NULL||pTeamGuild!=pGuild)
				{
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

void ResWarMgr::JoinTeam(Team *pTeam,UINT64 nRole)
{
	if (!pTeam||!m_bOpen)
	{
		return;
	}
	UINT32 nDNEID = ResWarMgr::Instance()->GetResWarPVDNEID();
	if (nDNEID == pTeam->GetConf()->DNExpeditionID)
	{
		UINT64 nGuildID = GetGuildID(pTeam);
		UINT32 nTeamID = pTeam->GetID();
		GuildFightTeam *pFightTeam = GetGuildFightTeam(nGuildID,nTeamID);
		if (pFightTeam)
		{
			CRoleSummary* pSum	 = CRoleSummaryMgr::Instance()->GetRoleSummary(nRole);
			if (pSum)
			{
				GuildFightRole *pFightRole=GetFightRole(pFightTeam->pGuildTeam, pSum->GetID());
				if(pFightRole)
				{
					GuildFightTeam *pOldTeam = pFightRole->pGuildFightTeam;
					if (pOldTeam)
					{
						auto iter = pOldTeam->mapFightRole.find(pFightRole->nRole);
						if (iter != pOldTeam->mapFightRole.end())
						{
							pOldTeam->mapFightRole.erase(iter);
						}
						if (pOldTeam->nFight>pSum->GetMaxPPT())
						{
							pOldTeam->nFight -= pSum->GetMaxPPT();
						}
					}
				}else
				{
					pFightRole	    =  new GuildFightRole();
					pFightRole->nFight = pSum->GetMaxPPT();
					pFightRole->nJob   = pSum->GetProfession();
					pFightRole->nLevel = pSum->GetLevel();
					pFightRole->nRole  = pSum->GetID();
					pFightRole->szName = pSum->GetName();
					pFightTeam->nFight += pSum->GetMaxPPT();
				}
				pFightTeam->mapFightRole.insert(std::pair<UINT64, GuildFightRole*>(pFightRole->nRole, pFightRole));
				pFightRole->pGuildFightTeam = pFightTeam;
				pFightRole->bLost = false;
			}
		}
	}
}

GuildFightRole *ResWarMgr::GetFightRole(GuildTeam *pGuildTeam, UINT64 nRoleID)
{
	if(NULL==pGuildTeam) 
		return NULL;
	for(auto iter = pGuildTeam->mapTeam.begin(); iter != pGuildTeam->mapTeam.end(); iter++)
	{
		GuildFightTeam *pFightTeam = iter->second;
		if (pFightTeam)
		{
			auto siter = pFightTeam->mapFightRole.find(nRoleID);
			if (siter != pFightTeam->mapFightRole.end())
			{
				return siter->second;
			}
		}
	}
	return NULL;
}




bool ResWarMgr::SetPVPData(GuildFightTeam *pSelf, GuildFightTeam *pTarget,UINT32 nUnnID)
{
	if (!pSelf||!pTarget) 
		return false;
	pSelf->nState = eGuildResStateFight;
	pTarget->nState = eGuildResStateFight;
	time_t nTime = GameTime::GetTime() + GetGlobalConfig().ResWarPVPTime;
	KKSG::CreateBattleParam param;
	KKSG::ResWarSceneInfo *pInfo =  param.mutable_reswar();
	pInfo->set_groupid(nUnnID);
	pInfo->set_endtime(nTime);
	pInfo->set_selfguildid(pSelf->nGuildID);
	UINT32 nSelfTime = GuildBuffMgr::Instance()->GetAddBuffTime(pSelf->nGuildID);
	pInfo->set_selfaddtime(nSelfTime);
	UINT32 nTargetTime = GuildBuffMgr::Instance()->GetAddBuffTime(pSelf->nGuildID);
	pInfo->set_targetaddtime(nTargetTime);
	pInfo->set_targetguildid(pTarget->nGuildID);
	AddResWarTeamBaseInfo(pSelf, pInfo,true);
	AddResWarTeamBaseInfo(pTarget, pInfo,false);
	Team *pSelfTeam = TeamMgr::Instance()->FindTeam(pSelf->nTeamID);
	if (pSelfTeam)
	{
		pSelfTeam->SetInBattleState();
	}
	Team *pTargetTeam = TeamMgr::Instance()->FindTeam(pTarget->nTeamID);
	if (pTargetTeam)
	{
		pTargetTeam->SetInBattleState();
	}
	bool flag = CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().ResWarPVPSceneID, &ICreateResWarListener::GlobalCreateResWarListener, param);	
	if(false == flag)
	{
		return false;
	}
	return true;
}

void ResWarMgr::EnterGroupFightScene(GuildFightTeam *pFightTeam,UINT32 nSceneID)
{
	if (!pFightTeam) 
		return;
	for(auto iter =pFightTeam->mapFightRole.begin(); iter!=pFightTeam->mapFightRole.end();iter++)
	{
		GuildFightRole *pFightRole = iter->second;
		if (pFightRole&&!pFightRole->bLost)
		{
			KKSG::SceneSwitchData oData;
			CRole *pRole = CRoleManager::Instance()->GetByRoleID(pFightRole->nRole);
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

void ResWarMgr::StartBattlePVP()
{
	UINT32 nTotal = 0;
	for (auto iter = m_vecMatchPVPGroup.begin();  iter!=m_vecMatchPVPGroup.end();)
	{
		if (nTotal >= GetGlobalConfig().ResWarPullGroup)
		{
			break;
		}
		GroupGuildFightTeam *pGroup = *iter;
		if (pGroup && pGroup->nFightTime&&m_nTime >= pGroup->nFightTime)
		{
			SetPVPData(pGroup->pSelf,pGroup->pTarget,pGroup->nUniID);
			m_vecReportGroup.push_back(pGroup);
			iter = m_vecMatchPVPGroup.erase(iter);
			nTotal++;
		}else
		{
			iter++;	
		}
	}
}

void ResWarMgr::SetFightTeamState(UINT32 nGroup)
{
	for (auto iter = m_vecReportGroup.begin(); iter!=m_vecReportGroup.end(); iter++)
	{
		GroupGuildFightTeam *pGroup  = *iter;
		if (pGroup&&pGroup->nUniID==nGroup)
		{
			if (pGroup->pSelf)
			{
				pGroup->pSelf->nState = eGuildResStateResult;
			}
			if(pGroup->pTarget)
			{
				pGroup->pTarget->nState = eGuildResStateResult;
			}
			LogInfo("pvp结束清理分组信息....%d",pGroup->nUniID);
			delete pGroup;
			m_vecReportGroup.erase(iter);
			break;
		}
	}
}

bool ResWarMgr::StartBattlePVE(GuildTeam *pGuildTeam,GuildFightTeam *pFightTeam,UINT32 nResID,UINT32 nBuffID)
{
	if (!pGuildTeam||!pFightTeam)
		return false;
	pFightTeam->nState = eGuildResStateFight;
	KKSG::CreateBattleParam param;
	KKSG::ResWarSceneInfo *pInfo =  param.mutable_reswar();
	pInfo->set_groupid(pFightTeam->nUniID);
	UINT32 nSize = pFightTeam->vecMineral.size();
	pInfo->set_mineid(nResID);
	pInfo->set_teamid(pFightTeam->nTeamID);
	pInfo->set_selfguildid(pGuildTeam->nGuildID);
	pInfo->set_buffid(nBuffID);

	Team *pTeam = TeamMgr::Instance()->FindTeam(pFightTeam->nTeamID);
	if (pTeam)
	{
		pTeam->SetInBattleState();
	}
	UINT32 nSelfTime = GuildBuffMgr::Instance()->GetAddBuffTime(pGuildTeam->nGuildID);
	pInfo->set_selfaddtime(nSelfTime);
	AddResWarTeamBaseInfo(pFightTeam, pInfo,true);
	bool flag = CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().ResWarPVESceneID, &ICreateResWarListener::GlobalCreateResWarListener, param);	
	return flag;
}





void ResWarMgr::GetGuildWarRank(UINT32 nGroupID,KKSG::ResWarFinalAll *pData)
{
	for (UINT32 i = 0; i < m_mapGuildWarRank[nGroupID].size();i++)
	{
		GuildWarRank *pRank = &m_mapGuildWarRank[nGroupID][i];
		KKSG::ResWarFinal*pFinal = pData->add_data();
		pFinal->set_guildid(pRank->nGuildID);
		pFinal->set_guildicon(pRank->nIcon);
		pFinal->set_guildname(pRank->szName);
		pFinal->set_res(pRank->nResValue);
	}
}


bool ResWarMgr::GiveAward(UINT64 nRoleID,Guild *pGuild,GuildMineralBattleReward::RowData *pRowData)
{
	Guild *pGuildRank = CGuildMgr::Instance()->GetGuildByRoleId(nRoleID);

	if (pGuildRank!=pGuild)
	{
		return false;
	}
	GuildMember *pMember = pGuildRank->GetMember(nRoleID);
	if (!pMember)
	{
		return false;
	}
	std::vector<ItemDesc> rewards;

	if (pMember->position == GPOS_LEADER)
	{
		for (auto niter = pRowData->AdministratorReward1.begin();niter!=pRowData->AdministratorReward1.end(); niter++)
		{
			ItemDesc item(niter->seq[0],niter->seq[1]);
			rewards.push_back(item);
			LogInfo("---------GiveFinalAward---------roleid=%llu itemid=%d-itemcount=%d-----------",nRoleID,item.itemID,item.itemCount);
		}
	}else if (pMember->position == GPOS_VICELEADER)
	{
		for(auto niter = pRowData->AdministratorReward2.begin();niter!=pRowData->AdministratorReward2.end(); niter++)
		{
			ItemDesc item(niter->seq[0],niter->seq[1]);
			LogInfo("---------GiveFinalAward---------roleid=%llu itemid=%d-itemcount=%d-----------",nRoleID,item.itemID,item.itemCount);
			rewards.push_back(item);
		}
	}else
	{
		for(auto niter = pRowData->MembersReward.begin();niter!=pRowData->MembersReward.end(); niter++)
		{
			ItemDesc item(niter->seq[0],niter->seq[1]);
			LogInfo("---------GiveFinalAward---------roleid=%llu itemid=%d-itemcount=%d-----------",nRoleID,item.itemID,item.itemCount);
			rewards.push_back(item);
		}
	}
	Notice notice(GuildResWarSignAward);
	notice.Replace("$R",ToString(pRowData->Rank));
	notice.SendRole(nRoleID);
	const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_ResWar);
	string szContent = pConf->m_content;
	string szTitle   = pConf->m_title;
	MailData oData(szTitle, szContent,rewards ,MAIL_EXPIRE_TIME);
	oData.SetReason(ItemFlow_GuildResWarAward);
	CMailMgr::Instance()->SendMail(nRoleID, oData);
	return true;
}


void ResWarMgr::GiveFinalAward()
{
	UINT32 curSealType = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	curSealType        = ResWarAwardMgr::Instance()->GetLevelSeal(curSealType);
	std::set<UINT64> setRole;
	GuildBuffMgr::Instance()->GetRoleID(setRole);

	std::map<UINT64, stAuctInfo> mapAuct;
	for (auto iter = m_mapGroupGuildInfo.begin(); iter!=m_mapGroupGuildInfo.end();iter++)
	{
		GroupGuildInfo *pInfo = iter->second;
		if (!pInfo)
			continue;
		SortGuildWarRankLess(pInfo->nGroupID);
		SortFightRoleLess(pInfo->nGroupID);
		for (size_t i = 0; i < m_mapGuildWarRank[pInfo->nGroupID].size();i++)
		{
			GuildWarRank *pRank = &m_mapGuildWarRank[pInfo->nGroupID][i];
			GuildMineralBattleReward::RowData *pRowData = 	ResWarAwardMgr::Instance()->GetGuildMineralBattleReward(i+1, curSealType);
			if (!pRowData)
				return;
			Guild *pGuild = CGuildMgr::Instance()->GetGuild(pRank->nGuildID);
			if (!pGuild) 
				continue;
			pGuild->AddExp(pRowData->GuildEXP);
			pGuild->AddPrestige(pRowData->GguildPrestige);		
			mapAuct[pGuild->GetID()].nRank = i + 1;
			for (auto miter = m_mapGuildWarRoleRank[pInfo->nGroupID].begin(); miter !=m_mapGuildWarRoleRank[pInfo->nGroupID].end(); miter++)
			{
				GuildWarRoleRank  &rolerank = *miter;
				GuildBuffMgr::Instance()->DelRoleID(rolerank.nRoleID);
				bool bRet = GiveAward(rolerank.nRoleID, pGuild,pRowData);
				if (bRet)
				{
					mapAuct[pGuild->GetID()].vecRoleID.push_back(rolerank.nRoleID);
				}
			}
		}
	}

	for (auto iter = setRole.begin(); iter!=setRole.end(); iter++)
	{
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(*iter);
		if (pGuild==NULL)
			continue;
		GuildMineralBattleReward::RowData *pRowData = 	ResWarAwardMgr::Instance()->GetGuildMineralBattleReward(mapAuct[pGuild->GetID()].nRank, curSealType);
		if (!pRowData)
			continue;
		GiveAward(*iter,pGuild,pRowData);
		mapAuct[pGuild->GetID()].vecRoleID.push_back(*iter);
	}

	for (auto iter = mapAuct.begin(); iter!=mapAuct.end(); iter++)
	{
		stAuctInfo &refInfo = iter->second;
		GuildAuctMgr::Instance()->AddGuildAuctData(iter->first, GA_ACT_GUILD_RESOURCE, refInfo.nRank, refInfo.vecRoleID);
	}

}

bool ResWarMgr::CreatePVPSceneBegin(GroupGuildFightTeam *pGroup)
{
	SetPVPData(pGroup->pSelf,pGroup->pTarget,pGroup->nUniID);
	return true;
}

void ResWarMgr::CreatePVPSceneEnd(UINT32 nGroupID,  UINT32 nSceneID)
{
	GroupGuildFightTeam *pGroup = GetPVPGroupFightTeam(nGroupID);
	if (!pGroup) 
		return;
	EnterGroupFightScene(pGroup->pSelf,nSceneID);
	EnterGroupFightScene(pGroup->pTarget,nSceneID);
	ClearPVPGroup(nGroupID);
}

void ResWarMgr::ClearPVPGroup(UINT32 nGroupID)
{
	for (auto iter = m_vecReportGroup.begin();  iter!=m_vecReportGroup.end();)
	{
		GroupGuildFightTeam *pGroupTeam = *iter;
		if (pGroupTeam->nUniID == nGroupID)
		{
			delete pGroupTeam;
			iter = m_vecReportGroup.erase(iter);
			break;
		}else
		{
			iter++;
		}
	}
}


bool ResWarMgr::GetRoleJoin(UINT64 nRole)
{
	for (auto iter = m_setRoleID.begin(); iter != m_setRoleID.end(); iter++)
	{
		if (nRole == *iter)
			return true;
	}
	return false;
}

void ResWarMgr::AddRoleJoin(UINT64 nRole)
{
	m_setRoleID.insert(nRole);
}

GuildFightTeam* ResWarMgr::GetPVEGroupFightTeam(UINT32 nUniID)
{
	GuildFightTeam *pGroup  = NULL;
	for (auto iter = m_vecMatchPVE.begin();  iter!=m_vecMatchPVE.end();iter++)
	{
		pGroup = *iter;
		if (pGroup->nUniID == nUniID)
			return pGroup;
	}
	return NULL;
}

void ResWarMgr::CreatePVESceneEnd(UINT32 nGroupID,  UINT32 nSceneID)
{
	GuildFightTeam* pFightTeam =  GetPVEGroupFightTeam(nGroupID);
	if (!pFightTeam)
		return;
	EnterGroupFightScene(pFightTeam,nSceneID);
}

void ResWarMgr::SendAllIcon(UINT32 nIcon,UINT32 nState)
{
	for(auto iter = CRoleManager::Instance()->Begin(); iter!=CRoleManager::Instance()->End();iter++)
	{
		CRole* pRole = *iter;
		CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
		if (pRoleSum)
		{
			if(pRoleSum->GetLevel() < GetGlobalConfig().ResWarLevelLimit)
				continue;
		}
		PtcM2C_HallIconMNtf ptc;
		ptc.m_Data.set_systemid(nIcon);
		ptc.m_Data.set_state((KKSG::HallIconState)nState);
		pRole->Send(ptc);
	}
}


UINT32 ResWarMgr::GetRiteTime()
{
	return ResWarAwardMgr::Instance()->GetRiteTime(m_nDurationTime);
}

void ResWarMgr::SendGuildRoleIcon(UINT32 nType)
{
	for (auto iter = m_mapGroupGuildInfo.begin(); iter!=m_mapGroupGuildInfo.end(); iter++)
	{
		 GroupGuildInfo *pGroup = iter->second;
		 if (pGroup)
		 {
			 for (auto miter = pGroup->mapGroupGuild.begin(); miter!=pGroup->mapGroupGuild.end();miter++)
			 {
				 GuildTeam* pGuildTeam = miter->second;
				 if (pGuildTeam)
				 {
					 Guild *pOneGuild = CGuildMgr::Instance()->GetGuild(pGuildTeam->nGuildID);
					 if (pOneGuild)
					 {
						 for (auto siter = pOneGuild->GetGuildMember().begin(); siter != pOneGuild->GetGuildMember().end(); siter++)
						 {
							CRole *pRole = CRoleManager::Instance()->GetByRoleID(siter->first);
							if (pRole)
							{
								switch(nType)
								{
								case eResWarMsgTypeIcon:
									{
										LogInfo("-----SendGuildRoleIcon---  ntype=%d name=%s",HICONS_BEGIN,pRole->GetName().c_str());
										SendRoleIcon(pRole,SYS_RESWAR,HICONS_BEGIN);
									}
									break;
								case eResWarAwardTypeIcon:
									{
										LogInfo("-----SendGuildAwardIcon---  ntype=%d name=%s",HICONS_BEGIN,pRole->GetName().c_str());
										SendRoleIcon(pRole,SYS_RESWAR_AWARD,HICONS_BEGIN);
									}
									break;
								case eResWarMsgTypeNotice:
									{
										SendRoleNotice(pRole,GuildResWarStart);
									}
									break;
								case eResWarOpen5Min:
									{
										SendRoleNotice(pRole,ResWarOpen5Min);
									}
									break;
								case eResWarFinal5Min:
									{
										SendRoleNotice(pRole,ResWarFinal5Min);
									}
									break;
								case eResWarMsgTypeTime:
									{
										SendTime(pRole);
									}
									break;
								}

							}
						 }

					 }
				 }
			 }
		 }
	}
}

void ResWarMgr::SendRoleIcon(CRole *pRole,UINT32 nSysID,UINT32 nState)
{
	if(!pRole||!m_bOpen)
		return ;
	CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	if (pRoleSum)
	{
		if(pRoleSum->GetLevel() < GetGlobalConfig().ResWarLevelLimit)
			return;
		LogInfo("---SendRoleIcon--,Role=%p m_bOpen=%d nState=%d",pRole,m_bOpen,nState);
		PtcM2C_HallIconMNtf ptc;
		ptc.m_Data.set_systemid(nSysID);
		ptc.m_Data.set_state((KKSG::HallIconState)nState);
		pRole->Send(ptc);
	}
}

void ResWarMgr::OnLogin(CRole *pRole)
{
	if (!pRole)
		return;

	if (m_bOpen)
	{
		if (m_bAward==true)
		{
			bool bRet = CheckInGroup(pRole,false);
			if (bRet)
				SendRoleIcon(pRole,SYS_RESWAR,HICONS_BEGIN);
		}
	}
	if (m_bAward==false)
	{
		bool bRet = CheckInGroup(pRole,true);
		if(bRet)
			SendRoleIcon(pRole,SYS_RESWAR_AWARD,HICONS_BEGIN);	
	}
}

void ResWarMgr::SendTime(CRole *pRole)
{
	if (!pRole)
		return;
	PtcM2C_ResWarTimeNtf ntf;
	ntf.m_Data.set_ntime(m_nCountDownTime-m_nTime);
	pRole->Send(ntf);
}


bool ResWarMgr::CheckInGroup(CRole *pRole,bool bCheck)
{
	if (!pRole)
		return false;
	for (auto iter = m_mapGroupGuildInfo.begin(); iter!=m_mapGroupGuildInfo.end(); iter++)
	{
		GroupGuildInfo *pGroup = iter->second;
		if (pGroup)
		{
			for (auto miter = pGroup->mapGroupGuild.begin(); miter!=pGroup->mapGroupGuild.end();miter++)
			{
				GuildTeam* pGuildTeam = miter->second;
				if (pGuildTeam)
				{
					Guild *pOneGuild = CGuildMgr::Instance()->GetGuild(pGuildTeam->nGuildID);
					if (pOneGuild)
					{
						GuildMember* pMember = pOneGuild->GetMember(pRole->GetID());
						if (pMember)
						{
							if (bCheck)
							{
								GuildFightRole *pFRole = GetFightRole(pGuildTeam, pRole->GetID());
								if (pFRole&&pFRole->bIcon)
									return true;
								else
									return false;
							}else
							{
								return true;
							}

						}
					}
				}
			}
		}
	}
	return false;
}

void ResWarMgr::SendRoleNotice(CRole *pRole, UINT32 nNoticeType)
{
	if(!pRole)
		return ;
	Notice notice(nNoticeType);
	notice.SendRole(pRole->GetID());
}


void ResWarMgr::SendGuildBuffCD(Guild *pGuild, CRole *pRole,UINT32 nCD)
{
	if (!pGuild||!pRole)
	{
		return;
	}
	PtcM2C_GuildBuffCDParamNtf ntf;
	ntf.m_Data.set_param(nCD);
	for(auto iter = pGuild->GetGuildMember().begin(); iter!=pGuild->GetGuildMember().end(); iter++)
	{
		CRole *pNRole = CRoleManager::Instance()->GetByRoleID(iter->first);
		if (pNRole&&pRole!=pNRole)
		{
			pNRole->Send(ntf);
		}
	}
}