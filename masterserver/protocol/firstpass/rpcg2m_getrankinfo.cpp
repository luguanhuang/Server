#include "pch.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "ranklist/ranklist.h"
#include "ranklist/ranklistmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "firstpass/firstpassconfig.h"
#include "ranklist/firstpassranklist.h"

// generate by ProtoGen at date: 2016/8/24 21:04:28

RPC_SERVER_IMPLEMETION(RpcG2M_GetRankInfo, GetRankInfoArg, GetRankInfoRes)

void RpcG2M_GetRankInfo::OnCall(const GetRankInfoArg &roArg, GetRankInfoRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	if (roArg.ranktype() == KKSG::FirstPassRank)
	{
		if (roArg.has_firstpassid())
		{
			UINT32 type = CRankListMgr::Instance()->GetFirstPassRankKey(roArg.firstpassid());
			CRankList* ranklist = CRankListMgr::Instance()->GetRankList(type, role->GetServerId());
			if (ranklist)
			{
				KKSG::FirstPassRankInfo* info = roRes.add_firstpassrankinfos();
				info->set_firstpassid(roArg.firstpassid());
				info->set_myrank(ranklist->GetRank(role->GetID()));
				info->set_totalrank(((FirstPassRankList*)ranklist)->GetSize());
			}
			return;
		}
		auto data = CFirstPassConfig::Instance()->GetFirstPassData();
		for (auto iter = data.begin(); iter != data.end(); ++iter)
		{
			UINT32 type = CRankListMgr::Instance()->GetFirstPassRankKey((*iter)->ID);
			CRankList* ranklist = CRankListMgr::Instance()->GetRankList(type, role->GetServerId());
			if (ranklist)
			{
				KKSG::FirstPassRankInfo* info = roRes.add_firstpassrankinfos();
				info->set_firstpassid((*iter)->ID);
				info->set_myrank(ranklist->GetRank(role->GetID()));
				info->set_totalrank(((FirstPassRankList*)ranklist)->GetSize());
				auto rankInfo = ((FirstPassRankList*)ranklist)->GetRankInfo(1);
				if (rankInfo)
				{
					info->set_toprankstarlevel(rankInfo->m_starLevel);
				}
			}
		}
	}
}

void RpcG2M_GetRankInfo::OnDelayReplyRpc(const GetRankInfoArg &roArg, GetRankInfoRes &roRes, const CUserData &roUserData)
{
}
