#include "pch.h"
#include "role/rpcg2m_personalcareertotalinfo.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "other/serverinfomgr.h"
#include "pk/pkmgr.h"
#include "ranklist/ranklist.h"
#include "ranklist/ranklistmgr.h"
#include "role/rolesummarymgr.h"
#include "table/ProfessionMgr.h"

// generate by ProtoGen at date: 2017/2/3 10:19:18

RPC_SERVER_IMPLEMETION(RpcG2M_PersonalCareerTotalInfo, PersonalCareerTotalInfoArg, PersonalCareerTotalInfoRes)

void RpcG2M_PersonalCareerTotalInfo::OnCall(const PersonalCareerTotalInfoArg &roArg, PersonalCareerTotalInfoRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.role_id());
	if (NULL == role)
	{
		SSWarn<<"role is NULL, sessionid:"<<roArg.role_id()<<END;
		return;
	}
	if (roArg.quest_type() == KKSG::PCRT_HOME_PAGE)
	{
		roRes.set_server_name(ZServerInfoMgr::Instance()->GetCurServerName());
		return;
	}
	UINT32 pk_rank = PkMgr::Instance()->GetRank(role->GetID());
	roRes.set_pk_rank(pk_rank);	
	roRes.set_profession_rank((UINT32)(-1));

	CRankList* ranklist = CRankListMgr::Instance()->GetRankList(KKSG::PkRealTimeRank);
	if (NULL == ranklist)
	{
		return;
	}
			
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
	if (NULL == summary)
	{			
		SSWarn<<"summary is NULL, role  id:"<<role->GetID()<<END;				
		return ;
	}

	KKSG::RankList rankdatas;
	UINT32 profession_id = CProfessionMgr::Instance()->GetBasicProfession(summary->GetProfession());
	ranklist->RankListToClient(&rankdatas, pk_rank, profession_id);

	for (UINT32 rank = 0; rank < rankdatas.rankdata_size(); ++rank)
	{
		if (rankdatas.rankdata(rank).roleid() == role->GetID())
		{
			roRes.set_profession_rank(rank+1);
		}
	}
}

void RpcG2M_PersonalCareerTotalInfo::OnDelayReplyRpc(const PersonalCareerTotalInfoArg &roArg, PersonalCareerTotalInfoRes &roRes, const CUserData &roUserData)
{
}
