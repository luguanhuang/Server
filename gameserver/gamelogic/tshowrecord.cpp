#include "pch.h"
#include "tshowrecord.h"
#include "util/XCommon.h"
#include "pb/project.pb.h"
//#include "flower.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "table/globalconfig.h"
#include "foreach.h"

CTShowRecord::CTShowRecord(Role* role)
{
	m_updateday = 0;
	m_rolessend.clear();
	m_pRole = role;
	m_haveSendRank = false;
}

bool CTShowRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_tshowvoterecord())
	{
		const KKSG::TShowVoteRecord* record = &poRoleAllInfo->tshowvoterecord();
		m_updateday = record->updatetime();
		if (record->has_havesendrank())
		{
			m_haveSendRank = record->havesendrank();
		}
		for (int i = 0; i < record->votedata_size(); ++i)
		{
			VoteData data;
			data.freeCount = record->votedata(i).freecount();
			data.costCount = record->votedata(i).costcount();
			m_rolessend.insert(std::make_pair(record->votedata(i).roleid(), data));
		}
		Update();
	}
	return true;
}

void CTShowRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_ismodify.TestAndReset())
	{
		KKSG::TShowVoteRecord* record = poRoleAllInfo->mutable_tshowvoterecord();
		record->Clear();
		record->set_updatetime(m_updateday);
		record->set_havesendrank(m_haveSendRank);
		for (auto i = m_rolessend.begin(); i != m_rolessend.end(); ++i)
		{
			KKSG::TShowRoleDailyVoteData* data = record->add_votedata();
			data->set_roleid(i->first);
			data->set_freecount(i->second.freeCount);
			data->set_costcount(i->second.costCount);
		}

		roChanged.insert(record);
	}
}

void CTShowRecord::Update()
{
	UINT32 uniqueday = XCommon::GetTodayUnique();
	if (m_updateday != uniqueday)
	{
		m_rolessend.clear();
		m_updateday = uniqueday;
		m_ismodify.Set();
		m_haveSendRank = false;
	}
}

void CTShowRecord::DebugReset()
{
	UINT32 uniqueday = XCommon::GetTodayUnique();
	m_rolessend.clear();
	m_updateday = uniqueday;
	m_ismodify.Set();
	m_haveSendRank = false;
}

bool CTShowRecord::IsCountLimit(UINT64 roleID, int type)
{
	Update();

	if (type == 1)
	{
		return !GetLeftFreeVoteCount();
	}

	auto iter = m_rolessend.find(roleID);
	if (iter == m_rolessend.end())
	{
		return false;
	}
	
	/*int freeCount =  GetGlobalConfig().TShowFreeVoteNum;
	if (type == 1 && iter->second.freeCount >= freeCount)
	{
		return true;
	}*/

	return false;
}

UINT32 CTShowRecord::Vote(UINT64 roleid, int type)
{
	if (type == 2)
	{
		int itemID =  GetGlobalConfig().TShowVoteCostNum[0];
		int itemCount =  GetGlobalConfig().TShowVoteCostNum[1];
		BagTakeItemTransition transition(m_pRole);
		transition.SetReason(ItemFlow_TShow, ItemFlow_TShow_Vote);
		if (!transition.TakeItem(itemID, itemCount))
		{
			transition.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		 transition.NotifyClient();
	}

	Update();

	GiveFirstVoteRewardToday();

	if (type == 1)
		++m_rolessend[roleid].freeCount;
	else if(type == 2)
		++m_rolessend[roleid].costCount;

	m_ismodify.Set();

	return KKSG::ERR_SUCCESS;
}

UINT32 CTShowRecord::GetVotedCount(UINT64 roleID, int type)
{
	auto find = m_rolessend.find(roleID);
	if (find == m_rolessend.end())
	{
		return 0;
	}
	if (type == 1)
	{
		return find->second.freeCount;
	}
	else if (type == 2)
	{
		return find->second.costCount;
	}

	return 0;
}

void CTShowRecord::GiveFirstVoteRewardToday()
{
	if (!m_rolessend.empty())
	{
		return;
	}
	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_TShow, ItemFlow_TShow_Vote);
	foreach(i in  GetGlobalConfig().TShowFirstVoteReward)
	{
		transition.GiveItem((*i)[0], (*i)[1]);
	}
	transition.NotifyClient();
}

UINT32 CTShowRecord::GetLeftFreeVoteCount()
{
	int total = 0;
	foreach(i in m_rolessend)
	{
		total += i->second.freeCount;
		if (total >=  GetGlobalConfig().TShowFreeVoteNum)
		{
			return 0;
		}
	}

	return ( GetGlobalConfig().TShowFreeVoteNum - total);
}
