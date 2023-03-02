#include "pch.h"
#include "teamstartbattle.h"
#include "team.h"
#include "table/expeditionconfigmgr.h"
#include "scene/sceneconfig.h"
#include "teammember.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "table/globalconfig.h"
#include "team/rpcm2g_teamstartbattlem2g.h"
#include "network/gslink.h"
#include "friend/friendop.h"
#include "friend/friendconfig.h"
#include "partner/partner.h"
#include "partner/partnermgr.h"
#include "tablemgr/partnertablemgr.h"
#include "marriage/marriagemgr.h"
#include "marriage/marriage.h"
#include "dragonguild/dragonguildmgr.h"
#include "tablemgr/dragonguildtablemgr.h"

TeamStartBattle::TeamStartBattle(Team* pTeam)
	:m_pTeam(pTeam)
{

}

void TeamStartBattle::GetRoles(std::vector<CRole*>& roles)
{
	const auto& mems = m_pTeam->GetMember();
	for(auto it = mems.begin(); it != mems.end(); ++it)
	{
		if((*it)->IsRobot())
		{
			continue;
		}
		CRole* pRole = CRoleManager::Instance()->GetByRoleID((*it)->m_qwRoleID);
		if(pRole)
		{
			roles.push_back(pRole);
		}
	}
}

void TeamStartBattle::FillBuffs(std::vector<CRole*>& roles, KKSG::SceneSwitchTeamData& data)
{
	//buff
	for(size_t i = 0; i < roles.size(); ++i)
	{
		std::vector<Sequence<int,2>> buffs;
		GetBuffs(roles[i], roles, buffs);
		if(buffs.empty())
		{
			continue;
		}
		for(size_t j = 0; j < buffs.size(); ++j)
		{
			KKSG::RoleBuff* rb = data.add_rolebufflist();
			rb->set_roleid(roles[i]->GetID());
			KKSG::Buff* kkbf = rb->mutable_buff();
			kkbf->set_buffid(buffs[j][0]);
			kkbf->set_bufflevel(buffs[j][1]);
		}
	}
}

void TeamStartBattle::GetBuffs(CRole* self, const std::vector<CRole*>& roles, std::vector<Sequence<int,2>>& buffs)
{
	buffs.clear();
	UINT64 selfid = self->GetID();
	UINT64 guildid = CGuildMgr::Instance()->GetRoleGuild(selfid);

	bool haveSameGuild = false;
	UINT32 partnerLevel = 0;
	UINT32 weddingType = 0;
	UINT32 dragonguildLevel = 0;

	for (size_t i = 0; i < roles.size(); ++i)
	{
		CRole* role = roles[i];
		if (selfid == role->GetID())
		{
			continue;
		}

		// 人数不影响buf
		if (partnerLevel == 0 && PartnerMgr::Instance()->IsPartner(selfid, role->GetID()))
		{
			Partner* p = PartnerMgr::Instance()->GetPartnerByRole(selfid);
			if (p)
			{
				partnerLevel = p->GetLevel();
			}
		}

		if (dragonguildLevel == 0 && DragonGuildMgr::Instance()->IsInsameDragonGuild(selfid, role->GetID()))
		{
			DragonGuild* p = DragonGuildMgr::Instance()->GetDragonGuildByRole(selfid);
			if (p)
			{
				dragonguildLevel = p->GetLevel();
			}
		}

		// guild buf
		if (0 != guildid && guildid == CGuildMgr::Instance()->GetRoleGuild(role->GetID()))
		{
			haveSameGuild = true;
		}
		if (weddingType == 0 && MarriageMgr::Instance()->GetMarriage(selfid)->GetCoupleID() == role->GetID())
		{
			weddingType = MarriageMgr::Instance()->GetMarriage(selfid)->GetMarriageType();
		}
	}

	if (partnerLevel > 0)
	{
		PartnerTable::RowData* data = PartnerTableMgr::Instance()->GetPartnerData(partnerLevel);
		if (data)
		{
			buffs.push_back(data->buf);
		}
	}

	if (dragonguildLevel > 0)
	{
		DragonGuildTable::RowData* data = DragonGuildTableMgr::Instance()->GetDragonGuildData(dragonguildLevel);
		if (data)
		{
			buffs.push_back(data->buf);
		}
	}

	if (haveSameGuild)
	{
		Sequence<int,2> seq;
		seq[0] = GetGlobalConfig().TeamGuildBuff[0];
		seq[1] = GetGlobalConfig().TeamGuildBuff[1];
		buffs.push_back(seq);
	}

	if (weddingType == KKSG::WeddingType_Normal)
	{
		Sequence<int,2> seq;
		seq[0] = GetGlobalConfig().NormalMarriageBuff.seq[0];
		seq[1] = GetGlobalConfig().NormalMarriageBuff.seq[1];
		buffs.push_back(seq);
	}
	else if (weddingType == KKSG::WeddingType_Luxury)
	{
		Sequence<int,2> seq;
		seq[0] = GetGlobalConfig().SplendidMarriageBuff.seq[0];
		seq[1] = GetGlobalConfig().SplendidMarriageBuff.seq[1];
		buffs.push_back(seq);
	}
}

bool TeamStartBattle::StartBattle()
{
	auto pConf = m_pTeam->GetConf();
	if(NULL == pConf)
	{
		return false;
	}
	//std::vector<UINT32> sceneids;
	//if(pConf->PreReadyScene)
	//{
	//	sceneids.push_back(pConf->PreReadyScene);
	//}
	//int n = (int)pConf->RandomSceneIDs.size();
	//for(int i = 0; i < n; ++i)
	//{
	//	UINT32 sid = ExpeditionConfigMgr::Instance()->GetRandomScene(pConf->RandomSceneIDs[i]);
	//	sceneids.push_back(sid);
	//}

	//if(sceneids.empty())
	//{
	//	SSWarn << __FUNCTION__ << " sceneid empty teamid = " << m_pTeam->GetID() << END;
	//	return false;
	//}

	std::vector<CRole*> roles;
	GetRoles(roles);
	if (roles.empty())
	{
		return false;
	}

	//auto *pSceneInfo = CSceneConfig::Instance()->GetSceneConf(sceneids[0]);
	//if (NULL == pSceneInfo)
	//{
	//	SSError << " find not scene conf mapid = " << sceneids[0] << END;
	//	return false;
	//}

	KKSG::SceneSwitchData oData;
	KKSG::SceneSwitchTeamData* data = oData.mutable_teamdata();

	data->set_airsceneid(pConf->PreReadyScene);//飞艇场景
	
	//data->set_cursceneindex(0);
	//for(size_t i = 0; i < sceneids.size(); ++i)
	//{
	//	data->add_sceneids(sceneids[i]);
	//}

	const auto& mems = m_pTeam->GetMember();
	for(auto it = mems.begin(); it != mems.end(); ++it)
	{
		CTeamMember* pMember = (*it);
		if(pMember->IsRobot())
		{
			CRoleSummary* pSummary = pMember->GetRobotSummry();
			if(pSummary)
			{
				data->add_robotid(pMember->RoleID());
				*data->add_robotsummarystored() = pSummary->GetData();
			}
		}
	}

	FillBuffs(roles, *data);

	m_pTeam->SaveSynData(*data->mutable_teamsyndata());

	ToGSStartBattle(oData);

	return true;
}

void TeamStartBattle::ToGSStartBattle(KKSG::SceneSwitchData& oData)
{
	RpcM2G_TeamStartBattleM2G* rpc = RpcM2G_TeamStartBattleM2G::CreateRpc();
	rpc->m_oArg.mutable_ssdata()->CopyFrom(oData);
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
}
