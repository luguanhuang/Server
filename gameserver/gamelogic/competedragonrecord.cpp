#include "pch.h"
#include "competedragonrecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include <time.h>
#include "gamelogic/bagtransition.h"
#include "foreach.h"
#include "table/DragonNestTable.h"
#include "gamelogic/dragonconfig.h"
#include <time.h>
#include "util/gametime.h"
#include "scene/sceneteam.h"
#include "scene/scene.h"
#include "table/expeditionconfigmgr.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/competedragonconfig.h"
#include "common/ptcg2c_hintnotify.h"
#include "define/systemiddef.h"

CCompeteDragonRecord::CCompeteDragonRecord(Role* role)
{
	m_pRole = role;
	m_updateDay = 0;
	m_throughCount = 0;
	m_getRewardCount = 0;
}

CCompeteDragonRecord::~CCompeteDragonRecord()
{

}

bool CCompeteDragonRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_competedragoninfo())
	{
		const KKSG::CompeteDragonInfo2DB* dbInfo = &poRoleAllInfo->competedragoninfo();
		m_updateDay = dbInfo->updateday();
		m_throughCount = dbInfo->throughcount();
		m_getRewardCount = dbInfo->getrewardcount();
	}

	Update();

	return true;
}

void CCompeteDragonRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::CompeteDragonInfo2DB* record = poRoleAllInfo->mutable_competedragoninfo();
		record->Clear();
		record->set_updateday(m_updateDay);
		record->set_throughcount(m_throughCount);
		record->set_getrewardcount(m_getRewardCount);

		roChanged.insert(record);
	}
}

void CCompeteDragonRecord::Change()
{
	m_switch.Set();
}

void CCompeteDragonRecord::Update()
{
	int time = CompeteDragonConfig::Instance()->GetResetTime();
	if (time && time != m_updateDay)
	{
		Reset(time);
	}
}

void CCompeteDragonRecord::Reset(int time)
{
	m_updateDay = CompeteDragonConfig::Instance()->GetResetTime();
	m_throughCount = 0;
	m_getRewardCount = 0;
	Change();

	LogInfo("%llu, %s, reset", m_pRole->GetID(), m_pRole->GetName().c_str());
}


void CCompeteDragonRecord::DebugReset()
{
	Reset(GameTime::GetTime());
	Change();
}

void CCompeteDragonRecord::DebugSetThrough(UINT32 count)
{
	m_throughCount = count;
	Change();
}

bool CCompeteDragonRecord::CheckRoleCondition(int expID)
{
	if (CompeteDragonConfig::Instance()->GetExpID() != expID)
	{
		return false;
	}
	return true;
}

void CCompeteDragonRecord::AddThroughCount()
{
	++m_throughCount;
	Change();

	HintNotify();
}

UINT32 CCompeteDragonRecord::GetReward()
{
	if (m_getRewardCount >= m_throughCount)
	{
		return KKSG::ERR_FAILED;
	}

	auto data = CompeteDragonConfig::Instance()->GetCompeteDragonInfo();
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}

	if (m_getRewardCount >= data->MCount)
	{
		return KKSG::ERR_FAILED;
	}
	
	++m_getRewardCount;
	Change();

	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_CompeteDragon, ItemFlow_CompeteDragonThrough);
	for (auto iter = data->Reward.begin(); iter != data->Reward.end(); ++iter)
	{
		transition.GiveItem(iter->seq[0], iter->seq[1]);
	}
	transition.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

UINT32 CCompeteDragonRecord::FillCompeteDragonInfo2Client(KKSG::GetCompeteDragonInfoRes& roRes)
{
	auto data = CompeteDragonConfig::Instance()->GetCompeteDragonInfo();
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}

	int canRewardCount = 0;
	int maxCanGetCount = m_throughCount >= data->MCount ? data->MCount : m_throughCount;
	canRewardCount = maxCanGetCount > m_getRewardCount ? maxCanGetCount - m_getRewardCount : 0;
	roRes.set_leftrewardcount(data->MCount > m_getRewardCount ? data->MCount - m_getRewardCount : 0);
	roRes.set_totalrewardcount(data->MCount);
	roRes.set_cancangetrewardcount(canRewardCount);
	roRes.set_curdnexpid(CompeteDragonConfig::Instance()->GetExpID());

	return KKSG::ERR_SUCCESS;
}

void CCompeteDragonRecord::HintNotify()
{
	auto data = CompeteDragonConfig::Instance()->GetCompeteDragonInfo();
	if (data && data->MCount > m_getRewardCount && m_getRewardCount < m_throughCount)
	{
		PtcG2C_HintNotify notify;
		notify.m_Data.add_systemid(SYS_COMPETEDRAGON);
		notify.m_Data.set_isremove(false);
		m_pRole->Send(notify);
	}
}

void CCompeteDragonRecord::OnLogin()
{
	HintNotify();
}

