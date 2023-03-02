#include "pch.h"
#include "partner/ptcg2m_addpartnerlivenessg2m.h"
#include "partner/partnermgr.h"
#include "marriage/marriagemgr.h"
#include "marriage/marriage.h"
#include "dragonguild/dragonguildmgr.h"
// generate by ProtoGen at date: 2016/12/10 17:02:08

void PtcG2M_AddPartnerLivenessG2M::Process(UINT32 dwConnID)
{
	UINT64 roleId = m_Data.roleid();
	/*
		modify by wyx 2017-09-04 龙本小分队
	Partner* p = PartnerMgr::Instance()->GetPartnerByRole(roleId);
	if (p)
	{
		p->GetPartnerLiveness().AddPartnerLiveness(roleId, m_Data.actid(), m_Data.value());
	}
	*/
	DragonGuild* dragonGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(roleId);
	if(dragonGuild)
	{
		dragonGuild->GetDragonGuildLiveness().AddPartnerLiveness(roleId, m_Data.actid(), m_Data.value());
	}
	
	Marriage* pMr = MarriageMgr::Instance()->GetMarriage(roleId);
	if (pMr && pMr->IsInMarriagePos())
	{
		Marriage* pMrOther = MarriageMgr::Instance()->GetMarriage(pMr->GetCoupleID());
		if (pMrOther)
		{
			pMr->AddLivenessRecord(roleId, m_Data.actid(), m_Data.value());
			pMrOther->AddLivenessRecord(roleId, m_Data.actid(), m_Data.value());
		}
	}
}
