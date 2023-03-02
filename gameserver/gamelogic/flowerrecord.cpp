#include "pch.h"
#include "flowerrecord.h"
#include "util/XCommon.h"
#include "pb/project.pb.h"
#include "flower.h"
#include "gamelogic/bagtransition.h"
#include "unit/role.h"
#include "social/ptcg2c_flowerrankrewardntf.h"
#include "foreach.h"
#include "rolesummarymgr.h"
#include "social/rpcg2m_msgetflwoerrankreward.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/tshowrecord.h"
#include "commondef.h"

CFlowerRecord::CFlowerRecord(Role* role)
{
	m_pRole = role;
	m_updateday = 0;
	m_getRankReward = false;
	m_getFlowerTime = 0;
	m_getNum = 0;
}

bool CFlowerRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_flowerrecord())
	{
		const KKSG::FlowerRecord* record = &poRoleAllInfo->flowerrecord();
		m_updateday = record->updateday();
		for (int i = 0; i < record->roleid_size(); ++i)
		{
			m_rolessend.insert(std::make_pair(record->roleid(i), record->count(i)));
		}
		if(record->has_getflowertime())
			m_getFlowerTime = record->getflowertime();
		if(record->has_getflowernum())
			m_getNum = record->getflowernum();
		if (record->has_getrankreward())
			m_getRankReward = record->getrankreward();
		Update();
	}
	return true;
}

void CFlowerRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_ismodify.TestAndReset())
	{
		KKSG::FlowerRecord* record = poRoleAllInfo->mutable_flowerrecord();
		record->Clear();
		record->set_updateday(m_updateday);
		for (auto i = m_rolessend.begin(); i != m_rolessend.end(); ++i)
		{
			record->add_roleid(i->first);
			record->add_count(i->second);
		}
		record->set_getflowertime(m_getFlowerTime);
		record->set_getrankreward(m_getRankReward);
		record->set_getflowernum(m_getNum);

		roChanged.insert(record);
	}
}

void CFlowerRecord::Update()
{
	UINT32 uniqueday = XCommon::GetTodayUnique();
	if (m_updateday != uniqueday)
	{
		m_rolessend.clear();
		m_updateday = uniqueday;
		m_ismodify.Set();
		m_getRankReward = false;
		m_getNum = 0;
	}
}

bool CFlowerRecord::IsCountLimit()
{
	return (int)m_rolessend.size() >=  GetGlobalConfig().DailyFlowerCount;
}

bool CFlowerRecord::IsRoleLimit(UINT64 roleid)
{
	/*for (auto i = m_rolessend.begin(); i != m_rolessend.end(); ++i)
	{
		if (roleid == i->first && FlowerDoubleNine != i->second)
		{
			return true;
		}
	}*/
	return false;
}

void CFlowerRecord::SendFlower(UINT64 roleid, UINT32 count)
{
	m_rolessend.insert(std::make_pair(roleid, count));
	m_ismodify.Set();
}

UINT32 CFlowerRecord::GetTotalSend()
{
	UINT32 total = 0; 
	for( auto i=m_rolessend.begin(); i!= m_rolessend.end(); i++)
	{
		total += i->second;
	}
	return total; 
}

UINT32 CFlowerRecord::GetLeftFlowerTime()
{
	if (m_getNum >=  GetGlobalConfig().GetFlowerNum)
	{
		return -1;
	}
	if (m_getFlowerTime)
	{
		if ( GetGlobalConfig().GetFlowerTime.size() != 3)
		{
			return 0;
		}
		int now = TimeUtil::GetTime();
		if (now > (m_getFlowerTime +  GetGlobalConfig().GetFlowerTime[1]))
		{
			return 0;
		}
		else
			return (m_getFlowerTime +  GetGlobalConfig().GetFlowerTime[1] - now);
	}

	return 0;
}

UINT32 CFlowerRecord::GetFlower()
{
	if (GetLeftFlowerTime())
	{
		return KKSG::ERR_FAILED;
	}
	if ( GetGlobalConfig().GetFlowerTime.size() != 3)
	{
		return KKSG::ERR_FAILED;
	}

	BagGiveItemTransition transiton(m_pRole);
	transiton.SetReason(ItemFlow_Flower, ItemFlow_Flower_Get);
	transiton.GiveItem( GetGlobalConfig().GetFlowerTime[0],  GetGlobalConfig().GetFlowerTime[2]);	
	transiton.NotifyClient();

	m_getFlowerTime = time(0);
	m_ismodify.Set();
	++m_getNum;

	return KKSG::ERR_SUCCESS;
}

bool CFlowerRecord::CheckGetRankReward()
{
	Update();
	if (m_getRankReward)
	{
		return false;
	}

	return true;
}

UINT32 CFlowerRecord::GetRankReward(int rank)
{
	Update();
	if (m_getRankReward)
	{
		return KKSG::ERR_FAILED;
	}

	//FlowerRankList* list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerYesterdayRank);
	//if (list)
	{
		/*UINT32 rank = list->GetRank(m_pRole->GetID());*/
		if (rank == INVALID_RANK)
			return KKSG::ERR_FAILED;

		const FlowerRankRewardTable::RowData* conf = FlowerConfig::Instance()->GetFlowerRewardData(rank);
		if (!conf)
			return KKSG::ERR_FAILED;
		BagGiveItemTransition transition(m_pRole);
		transition.SetReason(ItemFlow_Flower, ItemFlow_Flower_GetRankReward);
		foreach(i in conf->reward)
		{
			transition.GiveItem((*i).seq[0], (*i).seq[1]);
		}
		transition.NotifyClient();
	}
	//else
	//	return KKSG::ERR_FAILED;

	m_getRankReward = true;
	m_ismodify.Set();
	return KKSG::ERR_SUCCESS;
}

void CFlowerRecord::OnFirstEnterScene()
{
	if (m_getRankReward)
	{
		return;
	}
	RpcG2M_MSGetFlwoerRankReward* rpc = RpcG2M_MSGetFlwoerRankReward::CreateRpc();

	rpc->m_sessionID = m_pRole->GetSessionID();

	m_pRole->SendToMS(*rpc);
}

void CFlowerRecord::OnFirstEnterSceneReply(UINT32 rank, UINT32 totalRank)
{
	if (rank != INVALID_RANK && FlowerConfig::Instance()->GetFlowerRewardData(rank))
	{
		PtcG2C_FlowerRankRewardNtf ntf;
		m_pRole->Send(ntf);
	}
	if (rank == INVALID_RANK)
	{
		m_getRankReward = true;
		m_ismodify.Set();
	}

	CTShowRecord* record = m_pRole->Get<CTShowRecord>();
	if (!record->HaveSendRank())
	{
		/*if (totalRank != INVALID_RANK && totalRank <= 3)
		{
			NoticeMgr::Instance()->TShowTop3NoticeSelf(m_pRole, totalRank);
		}
		if (rank != INVALID_RANK && rank <= 10)
		{
			NoticeMgr::Instance()->TShowWalkNotice(m_pRole, rank);
		}*/

		record->SetSendRank(true);
	}
}

void CFlowerRecord::OnDayPass()
{
	Update();
	//OnFirstEnterScene();
}

