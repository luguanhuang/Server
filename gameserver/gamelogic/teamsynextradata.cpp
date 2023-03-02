#include "pch.h"
#include "teamsynextradata.h"
#include "expeditionconfigmgr.h"
#include "ExpeditionTable.h"
#include "define/teamdef.h"
#include "teamrequest.h"
#include "team/ptcg2m_teamtrangs2ms.h"
#include "teamrecord.h"
#include "teammgr.h"
#include "team/teamcommon.h"

HTeamSynExtraData::HTeamSynExtraData()
{

}

HTeamSynExtraData::~HTeamSynExtraData()
{

}

void HTeamSynExtraData::TrySynExtraDataToTs(Role* pRole, int ttype)
{
	if(!NeedSynToTs(pRole, ttype))
	{
		return;
	}
	std::vector<Role*> roles;
	roles.push_back(pRole);
	TrySynExtraDataToTs(roles, ttype);
}

void HTeamSynExtraData::TrySynExtraDataToTs(std::vector<Role*> roles, int ttype)
{
	if(roles.empty())
	{
		return;
	}
	PtcG2M_TeamTranGs2Ms ptc;
	auto& data = ptc.m_Data;
    data.set_type(TEAM_TRANS_SYN_EXTRAINFO);
	data.set_teamtype(ttype);
	for(size_t i = 0; i < roles.size(); ++i)
	{
		Role* pRole = roles[i];
		if(NULL == pRole)
		{
			continue;
		}
		if(!NeedSynToTs(pRole, ttype))
		{
			continue;
		}
		auto& syn = *data.add_tsynextradata();
		FillExtraDataTType(pRole, ttype, syn);
	}

	if(data.tsynextradata_size() == 0)
	{
		return;
	}
	bool flag = false;
	for(size_t i = 0; i < roles.size(); ++i)
	{
		if(roles[i] && roles[i]->SendToMS(ptc))
		{
			flag = true;
			break;
		}
	}
	if(!flag)
	{
		SSError << "team syn extra data err " << END;
	}
}

void HTeamSynExtraData::FillExtraDataExpID(Role* pRole, int expID, KKSG::TeamSynExtraData& data)
{
	if(NULL == pRole)
	{
		return;
	}
	data.set_roleid(pRole->GetID());
	auto pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
	if(NULL == pConf)
	{
		return;
	}
	//if(TEAM_CCT_NONE == pConf->CostCountType)
	//{
	//	return ;
	//}
	FillExtraDataTType(pRole, pConf->Type, data, (TEAM_CCT_NONE != pConf->CostCountType));
}

void HTeamSynExtraData::FillExtraDataTType(Role* pRole, int ttype, KKSG::TeamSynExtraData& data, bool lcflag/* = true*/)
{
	if(NULL == pRole)
	{
		return;
	}
	data.set_roleid(pRole->GetID());
	if(lcflag)
	{
		data.set_leftcount(pRole->Get<CTeamRecord>()->GetTeamLeftCountToday(ttype));
	}
	else
	{
		data.set_leftcount(0);//²»ÄÜÎªnull
	}
	data.set_kingback(pRole->IsKingBack());
}

bool HTeamSynExtraData::NeedSynToTs(Role* pRole, int ttype)
{
	if(NULL == pRole)
	{
		return false;
	}
	if(GSConfig::Instance()->IsCrossGS())
	{
		return true;
	}
	auto pTeam = TeamCopyMgr::Instance()->GetTeamByRoleID(pRole->GetID());
	if(NULL == pTeam)
	{
		return false;
	}
	auto pConf = pTeam->GetConf();
	if(NULL == pConf)
	{
		return false;
	}
	if(HTeam::IsTsTeam(pTeam->GetTeamID()) && ttype == pConf->Type)
	{
		return true;
	}
	return false;
}
