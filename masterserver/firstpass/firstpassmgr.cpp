#include "pch.h"
#include <time.h>
#include "firstpassmgr.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "firstpass/ptcm2g_firstpassnewcommendrewardntf.h"
#include "scene/scene.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "notice/noticemgr.h"
#include "pb/project.pb.h"
#include "firstpass/firstpassconfig.h"
#include "network/gslink.h"
#include "ranklist/ranklistmgr.h"
#include "table/globalconfig.h"
#include "foreach.h"
#include "event/eventmgr.h"
#include "ranklist/ranklist.h"
#include "loghelper/tlogr.h"
#include "util/gametime.h"

INSTANCE_SINGLETON(CFirstPassMgr)


CFirstPassMgr::CFirstPassMgr()
{

}

CFirstPassMgr::~CFirstPassMgr()
{

}

bool CFirstPassMgr::Init()
{
	return true;
}

void CFirstPassMgr::Uninit()
{

}

void CFirstPassMgr::UpdataFirstPassRank(const KKSG::UpdateMSRankListArg& roArg, KKSG::UpdateMSRankListRes& roRes)
{
	roRes.set_error(KKSG::ERR_FAILED);
	if (!(roArg.ranktype() == KKSG::FirstPassRank && roArg.has_info()))
	{
		return;
	}

	std::vector<UINT64> roleids;
	for (int i = 0; i < roArg.info().roleids_size(); ++i)
	{
		roleids.push_back(roArg.info().roleids(i));
	}	

	const FirstPassTable::RowData* data = CFirstPassConfig::Instance()->GetFirstPassDataByID(roArg.firstpassid());
	if (!data)
	{
		return;
	}

	std::map<int, int> mapServerRank;
	//int rank = CRankListMgr::Instance()->OnChangeFirstPassRank(roleids, roArg.firstpassid(), roArg.info().starlevel());
	mapServerRank = CRankListMgr::Instance()->OnChangeFirstPassRank(roleids, roArg.firstpassid(), roArg.info().starlevel());
	if (mapServerRank.empty())
	{
		return;
	}
	bool allInvalidRank = true;
	for (auto iter = mapServerRank.begin(); iter != mapServerRank.end(); ++iter)
	{
		if (iter->second != INVALID_RANK)
		{
			allInvalidRank = false;
			break;
		}
	}
	if (allInvalidRank)
	{
		return;
	}

	UINT32 tempPassSceneID = 0;
	if (roArg.info().starlevel() >= data->SceneID.size())
	{
		return;
	}
	tempPassSceneID = data->SceneID[roArg.info().starlevel()];

	std::vector<int> commendVec;
	for (auto iter = mapServerRank.begin(); iter != mapServerRank.end(); ++iter)
	{
		std::vector<CRoleSummary*> roles;
		for (int i = 0; i < roArg.info().roleids_size(); ++i)
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.info().roleids(i));
			if (summary)
			{
				roles.push_back(summary);
			}
		}

		int rank = iter->second;
		if (rank <= GetGlobalConfig().GetInt("FirstPassRank", 20) && rank > 0)
		{
			const SceneConf* conf = CSceneConfig::Instance()->GetSceneConf(tempPassSceneID);
			if (!conf)
			{
				return;
			}

			if (rank == 1)
			{
				//NoticeNewCommendReward(roArg.firstpassid());
				commendVec.push_back(iter->first);
			}

			NoticeMgr::Instance()->FirstPassNotice(iter->first, roles, conf->Comment, rank);
		}

		if (rank > 0)
		{
			OnFirstPassRank(rank, tempPassSceneID, roleids);
			TFirstPassFlow oLog(TLogMgr::Instance()->GetTagString());
			oLog.m_ID = roArg.firstpassid();
			oLog.m_SceneID = tempPassSceneID;
			oLog.m_Rank = rank;
			oLog.m_StarLv = roArg.info().starlevel();
			oLog.SetData(roles);
			oLog.Do();
		}

		auto onerank = roRes.add_rank();
		onerank->set_key(iter->first);
		onerank->set_value(rank);
	}

	roRes.set_error(KKSG::ERR_SUCCESS);

	NoticeNewCommendReward(commendVec, roArg.firstpassid());
}

void CFirstPassMgr::OnFirstPassRank(int nRank, UINT32 nSceneID ,std::vector<UINT64>& roleIDs)
{
	if (nRank!=1){return;}
	for (auto iter = roleIDs.begin(); iter!=roleIDs.end();iter++)
	{
		UINT64 nRoleID = *iter;
		EventMgr::Instance()->AddEvent(nRoleID, DESIGNATION_COM_TYPE_FIRSTPASS, nSceneID);
	}
}

void CFirstPassMgr::NoticeNewCommendReward(std::vector<int>& serverIDs, int firstPassID)
{
	if (serverIDs.empty())
	{
		return;
	}
	PtcM2G_FirstPassNewCommendRewardNtf ntf;
	ntf.m_Data.set_firstpassid(firstPassID);
	for (int i = 0; i < serverIDs.size(); ++i)
	{
		ntf.m_Data.add_serverids(serverIDs[i]);
	}
	GSLink::Instance()->SendToAllLine(ntf);
}
