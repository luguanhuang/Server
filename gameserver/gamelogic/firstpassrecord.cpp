#include "pch.h"
#include "firstpassrecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include <time.h>
#include "gamelogic/bagtransition.h"
#include "foreach.h"
#include "gamelogic/firstpassconfig.h"
#include "scene/scene.h"
#include "firstpass/ptcg2c_firstpassrewardntf.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "gamelogic/stagemgr.h"
#include "table/FirstPassTable.h"
#include "define/systemiddef.h"

CFirstPassRecord::CFirstPassRecord(Role* role)
{
	m_pRole = role;
}

CFirstPassRecord::~CFirstPassRecord()
{
	m_passStageInfos.clear();
}

bool CFirstPassRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_firstpassrecord())
	{
		const KKSG::FirstPassRecord* record = &poRoleAllInfo->firstpassrecord();
		for (int i = 0; i < record->infos_size(); ++i)
		{
			KKSG::FirstPassStageInfo  info = KKSG::FirstPassStageInfo(record->infos(i));
			if (info.hascommended() && info.commendedstarlevels_size() == 0)
			{
				info.add_commendedstarlevels(0);
				Change();
			}
			m_passStageInfos[info.firstpassid()] = info;
		}
		Update();
	}
	return true;
}

void CFirstPassRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::FirstPassRecord* record = poRoleAllInfo->mutable_firstpassrecord();
		record->Clear();
		for (auto iter = m_passStageInfos.begin(); iter != m_passStageInfos.end(); ++iter)
		{
			KKSG::FirstPassStageInfo* info = record->add_infos();
			info->CopyFrom(iter->second);
		}

		roChanged.insert(record);
	}
}

void CFirstPassRecord::Update()
{

}

void CFirstPassRecord::Change()
{
	m_switch.Set();
}

void CFirstPassRecord::OnDayPass()
{
	Update();
	OnFirstEnterScene();
}

void CFirstPassRecord::DebugReset()
{
	m_passStageInfos.clear();
	Update();
	OnFirstEnterScene();
	Change();
	auto data = FirstPassConfig::Instance()->GetFirstPassData();
	for (auto iter = data.begin(); iter != data.end(); ++iter)
	{
		foreach (i in (*iter)->SceneID)
		{
			m_pRole->Get<StageMgr>()->DebugResetStageRank(*i);
		}
	}
	m_passStageInfos.clear();
}

void CFirstPassRecord::OnFirstPass(Scene* scene)
{
	if (!scene)
	{
		return;
	}
	const FirstPassTable::RowData* data = FirstPassConfig::Instance()->GetFirstPassDataByScene(scene->GetSceneTemplateID());
	if (!data)
	{
		return;
	}

	UINT32 starLevel = 0;
	foreach(i in data->SceneID)
	{
		if ((*i) == scene->GetSceneTemplateID())
		{
			break;
		}
		++starLevel;
	}

	auto recordInfo = GetFirstPassStageInfo(data->ID);
	if (!recordInfo)
	{
		KKSG::FirstPassStageInfo info = KKSG::FirstPassStageInfo();
		info.set_firstpassid(data->ID);
		info.set_isgetreward(false);
		info.set_hascommended(false);

		m_passStageInfos[data->ID] = info;

		if (starLevel)
		{
			LogError("role, %llu,%s , first pass wrong sceneid:%u,starlevel:%u", m_pRole->GetID(), m_pRole->GetName().c_str(), scene->GetSceneTemplateID(), starLevel);
		}
	}

	Change();

	//NotifyFirstPassReward(true, false);
}

void CFirstPassRecord::UpdateStarOneFirstPassRank(int firstPassID, int rank)
{
	auto data = FirstPassConfig::Instance()->GetFirstPassDataByID(firstPassID);
	if (!data || data->SceneID.empty())
	{
		return;
	}
	bool passStarLevelOne = m_pRole->Get<StageMgr>()->GetRank(data->SceneID[0]) != INVALID_STAGERANK;
	if (!passStarLevelOne)
	{
		return;
	}

	auto recordInfo = GetFirstPassStageInfo(firstPassID);
	if (!recordInfo)
	{
		return;
	}

	if (recordInfo->has_rank())
	{
		return;
	}

	recordInfo->set_rank(rank);
	
	Change();

	LogInfo("role:%llu, %s first pass id:%u rank %u", m_pRole->GetID(), m_pRole->GetName().c_str(), data->ID, rank);
}

/*const */KKSG::FirstPassStageInfo* CFirstPassRecord::GetFirstPassStageInfo(int firstPassID)
{
	auto iter = m_passStageInfos.find(firstPassID);
	if (iter != m_passStageInfos.end())
	{
		return &iter->second;
	}

	return NULL;
}

void CFirstPassRecord::OnFirstEnterScene()
{
	if (!m_pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_FIRSTPASS))
	{
		return;
	}
	
	RpcG2M_GetRankInfo* rpc = RpcG2M_GetRankInfo::CreateRpc();
	rpc->m_sessionID = m_pRole->GetSessionID();
	rpc->m_oArg.set_ranktype(KKSG::FirstPassRank);

	m_pRole->SendToMS(*rpc);
}

void CFirstPassRecord::OnFirstEnterSceneReply(const KKSG::GetRankInfoRes& roRes)
{
	bool hasReward = false;
	for (int i = 0; i < roRes.firstpassrankinfos_size(); ++i)
	{
		UpdateStarOneFirstPassRank(roRes.firstpassrankinfos(i).firstpassid(), roRes.firstpassrankinfos(i).myrank());

		if(!hasReward && _HasReward(roRes.firstpassrankinfos(i)))
		{
			hasReward = true;
		}
	}
	if (hasReward)
	{
		NotifyFirstPassReward(true, true);
	}

}

void CFirstPassRecord::NotifyFirstPassReward(bool hasPass, bool hasCommend)
{
	PtcG2C_FirstPassRewardNtf ntf;
	ntf.m_Data.set_hasfirstpassreward(hasPass);
	ntf.m_Data.set_hascommendreward(hasCommend);
	m_pRole->Send(ntf);
}

UINT32 CFirstPassRecord::GetFirstPassReward(int firstPassID, int rank)
{
	auto find = m_passStageInfos.find(firstPassID);
	if (find == m_passStageInfos.end())
	{
		return KKSG::ERR_FAILED;
	}

	if (!find->second.has_rank())
	{
		return KKSG::ERR_FAILED;
	}

	if (find->second.isgetreward())
	{
		return KKSG::ERR_FAILED;
	}
	
	auto data = FirstPassConfig::Instance()->GetFirstPassDataByID(firstPassID);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	if (data->SceneID.empty())
	{
		return KKSG::ERR_FAILED;
	}
	bool passStarLevelOne = m_pRole->Get<StageMgr>()->GetRank(data->SceneID[0]) != INVALID_STAGERANK;
	if (!passStarLevelOne)
	{
		return KKSG::ERR_FAILED;
	}

	auto rewardData = FirstPassConfig::Instance()->GetFirstPassRewardData(data->RewardID, find->second.rank());
	if (!rewardData)
	{
		return KKSG::ERR_FAILED;
	}

	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_FirstPass, ItemFlow_FirstPassRank);
	for (int i = 0; i < rewardData->Reward.size(); ++i)
	{
		transition.GiveItem(rewardData->Reward[i].seq[0], rewardData->Reward[i].seq[1]);
	}	
	transition.NotifyClient();

	find->second.set_isgetreward(true);
	Change();

	LogInfo("role:%llu, %s get first pass reward id :%u", m_pRole->GetID(), m_pRole->GetName().c_str(), firstPassID);

	return KKSG::ERR_SUCCESS;
}

/*void CFirstPassRecord::BeforeGetCommendReward(int firstPassID)
{
	m_passStageInfos[firstPassID].set_hascommended(true);
	m_passStageInfos[firstPassID].set_firstpassid(firstPassID);
}

void CFirstPassRecord::RevertGetCommendReward(int firstPassID)
{
	m_passStageInfos[firstPassID].set_hascommended(false);
}*/

UINT32 CFirstPassRecord::GetCommendReward(int firstPassID, int starLevel)
{
	UINT32 error = KKSG::ERR_FAILED;
	auto data = FirstPassConfig::Instance()->GetFirstPassDataByID(firstPassID);
	if (!data)
	{
		return error;
	}

	auto recordInfo = GetFirstPassStageInfo(firstPassID);
	if (recordInfo)
	{
		for (int i = 0; i < recordInfo->commendedstarlevels_size(); ++i)
		{
			if (recordInfo->commendedstarlevels(i) == starLevel)
			{
				return error;
			}
		}
	}

	if (starLevel >= data->SceneID.size())
	{
		return error;
	}

	UINT32 sceneID = data->SceneID[starLevel];

	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_FirstPass, ItemFlow_FirstPassCommend);
	for (int i = 0; i < data->CommendReward.size(); ++i)
	{
		if (data->CommendReward[i].seq[2] == sceneID)
		{
			transition.GiveItem(data->CommendReward[i].seq[0], data->CommendReward[i].seq[1]);
		}
	}	
	transition.NotifyClient();

	m_passStageInfos[firstPassID].set_firstpassid(firstPassID);
	m_passStageInfos[firstPassID].set_hascommended(true);
	m_passStageInfos[firstPassID].add_commendedstarlevels(starLevel);
	Change();

	LogInfo("role:%llu, %s commend first pass id :%u, starLevel:%u", m_pRole->GetID(), m_pRole->GetName().c_str(), firstPassID, starLevel);

	error = KKSG::ERR_SUCCESS;

	return error;
}

/*UINT32 CFirstPassRecord::CheckCommend(int firstPassID, int starLevel)
{
	auto find = m_passStageInfos.find(firstPassID);
	if (find == m_passStageInfos.end())
	{
		return KKSG::ERR_SUCCESS;
	}

	if (find->second.hascommended())
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}*/

void CFirstPassRecord::OnOpenSystem()
{
	OnFirstEnterScene();
}

bool CFirstPassRecord::_HasReward(const KKSG::FirstPassRankInfo& info)
{
	auto recordInfo = GetFirstPassStageInfo(info.firstpassid());
	auto data = FirstPassConfig::Instance()->GetFirstPassDataByID(info.firstpassid());
	if (!data || data->SceneID.empty())
	{
		return false;
	}
	if (!info.totalrank())
	{
		return false;
	}
	bool hasReward = false;
	if (recordInfo)
	{
		bool canCommend = true;
		for (int i = 0; i < recordInfo->commendedstarlevels_size(); ++i)
		{
			if (recordInfo->commendedstarlevels(i) == info.toprankstarlevel())
			{
				canCommend = false;
				break;
			}
		}
		//if (!recordInfo->hascommended())
		if(canCommend)
		{
			hasReward = true;
			//break;
		}
		else if ((!recordInfo->has_isgetreward() || !recordInfo->isgetreward()) && recordInfo->has_rank())
		{
			if (data && m_pRole->Get<StageMgr>()->GetRank(data->SceneID[0]) != INVALID_STAGERANK)
			{
				auto rewardData = FirstPassConfig::Instance()->GetFirstPassRewardData(data->RewardID, recordInfo->rank());
				if (rewardData)
				{
					hasReward = true;
					//break;
				}
			}
		}
	}
	if (!recordInfo && info.totalrank())
	{
		hasReward = true;
	}

	return hasReward;
}
