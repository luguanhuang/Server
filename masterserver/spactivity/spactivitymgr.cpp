#include "pch.h"
#include "spactivitymgr.h"
#include "pb/project.pb.h"
#include "define/spactivityiddef.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/ranklist.h"
#include "util/gametime.h"
#include "ranklist/flowerranklist.h"
#include "social/socialmgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "network/dblink.h"
#include "event/activitymailmgr.h"
#include "marriage/marriagemgr.h"

INSTANCE_SINGLETON(SpActivityMgr)

bool SpActivityMgr::Init()
{
	return true;
}

void SpActivityMgr::Uninit()
{
}

void SpActivityMgr::SyncActivityData(const KKSG::SyncActivityEnd2MSArg &roArg)
{
	++m_syncTimes;
	int oldFlowerActivityStage = 0;
	int newFlowerActivityStage = 0;
	int oldMarriageActivityStage = 0;
	time_t now = GameTime::GetTime();

	auto find = m_activityData.find(SpActivity_FlowerId);
	if (find != m_activityData.end())
	{
		oldFlowerActivityStage = find->second.m_stage;
	}
	find = m_activityData.find(SpActivity_Marriage);
	if (find != m_activityData.end())
	{
		oldMarriageActivityStage = find->second.m_stage;
	}
	m_activityData.clear();
	for (int i = 0; i < roArg.spactivitydata_size(); ++i)
	{
		OneActivityData temp;
		temp.m_actid = roArg.spactivitydata(i).actid();
		temp.m_stage = roArg.spactivitydata(i).actstage();
		temp.m_startTime = roArg.spactivitydata(i).starttime();
		temp.m_endTime = roArg.spactivitydata(i).endtime();
		temp.m_minlevel = roArg.spactivitydata(i).minlevel();

		m_activityData[temp.m_actid] = temp;
	}

	find = m_activityData.find(SpActivity_FlowerId);
	if (find != m_activityData.end())
	{
		newFlowerActivityStage = find->second.m_stage;
		if (m_syncTimes == 1)
		{
			SocialMgr::Instance()->UpdateFlowerActivityInfo();
		}

		OnFlowerActivityStageChange(oldFlowerActivityStage, newFlowerActivityStage);
	}

	find = m_activityData.find(SpActivity_Duck);
	if (find != m_activityData.end())
	{
		if (!m_bIsCheckDuck && DBLink::Instance()->IsConnected())
		{
			ActivityMailMgr::Instance()->CheckDuckUp();
			m_bIsCheckDuck = true;
		}
	}

	find = m_activityData.find(SpActivity_Marriage);
	if (find != m_activityData.end())
	{
		int newMarriageActivityStage = find->second.m_stage;
		if (oldMarriageActivityStage != newMarriageActivityStage && newMarriageActivityStage == 1 && now < find->second.m_endTime)
		{
			MarriageMgr::Instance()->CheckMarriageActivity(find->second.m_endTime);
		}
	}
}

const OneActivityData* SpActivityMgr::GetActivityData(UINT32 actID)
{
	auto find = m_activityData.find(actID);
	if (find == m_activityData.end())
	{
		return NULL;
	}

	return &find->second;
}

void SpActivityMgr::OnFlowerActivityStageChange(int oldFlowerActivityStage, int newFlowerActivityStage)
{
	if (oldFlowerActivityStage != newFlowerActivityStage && oldFlowerActivityStage && newFlowerActivityStage == 2)
	{
		auto begin = CRoleManager::Instance()->Begin();
		auto end = CRoleManager::Instance()->End();
		for (auto iter = begin; iter != end; ++iter)
		{
			SocialMgr::Instance()->OnLogin(*iter);
		}

		CRankList* ranklist = CRankListMgr::Instance()->GetRankList(KKSG::FlowerActivityRank);
		if (!ranklist)
		{
			return;
		}

		((FlowerRankList*)ranklist)->SendActiviyDayReward();
	}

}

