#include "pch.h"
#include <time.h>
#include "achievev2Record.h"
#include "pb/project.pb.h"
#include "util/XRandom.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "AchivementDef.h"
#include "rolenethandler.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"
#include "gamelogic/achievev2Mgr.h"

AchieveV2Record::AchieveV2Record(Role* role)
{
	m_pRole = role;
	m_mapAchieveInfo.clear();
	m_mapAchievePointRewardInfo.clear();
}

AchieveV2Record::~AchieveV2Record()
{

}

void AchieveV2Record::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->CanDoBackFlowInit(roleNum))
	{
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(m_pRole->GetProfession(), GSConfig::Instance()->GetBackFlowLevel());
		if (pData)
		{
			for (auto it = pData->Achieve.begin(); it != pData->Achieve.end(); ++it)
			{
				AchieveV2Mgr::Instance()->UpdateAchieve(m_pRole, *it);
			}
		}
	}
}

bool AchieveV2Record::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	m_mapAchieveInfo.clear();
	if(poRoleAllInfo->has_achieve())
	{
		const KKSG::AchieveDbInfo* pAchieveDb = &poRoleAllInfo->achieve();
		for (int i=0; i< pAchieveDb->achievedata_size(); i++)
		{
			STC_ACHIEVE_V2_INFO stc;  
			stc.dwAchieveID = pAchieveDb->achievedata(i).achieveid();
			stc.dwRewardStatus = pAchieveDb->achievedata(i).rewardstatus();
			m_mapAchieveInfo.insert( std::make_pair(stc.dwAchieveID, stc) );
		}
		m_mapAchievePointRewardInfo.clear();
		for (int i=0; i<pAchieveDb->achieveaward_size(); i++)
		{
			STC_ACHIEVE_POINT_REWARD_INFO stc;
			stc.dwRewardID = pAchieveDb->achieveaward(i).rewardid();
			stc.dwRewardStatus = pAchieveDb->achieveaward(i).rewardstatus();
			m_mapAchievePointRewardInfo.insert( std::make_pair(stc.dwRewardID, stc));
		}
		for (int i = 0; i < pAchieveDb->oldachievement_size(); ++i)
		{
			m_oldachievement[pAchieveDb->oldachievement(i).achieveid()] = pAchieveDb->oldachievement(i).rewardstatus();
		}
	}

	return true;
}

void AchieveV2Record::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if( !m_ismodify.TestAndReset() )
	{
		return; 
	}
	poRoleAllInfo->mutable_achieve()->Clear();
	for (auto i = m_mapAchieveInfo.begin(); i != m_mapAchieveInfo.end(); ++i)
	{
		KKSG::StcAchieveInfo* pInfo = poRoleAllInfo->mutable_achieve()->add_achievedata();
		pInfo->set_achieveid(i->second.dwAchieveID);
		pInfo->set_rewardstatus(i->second.dwRewardStatus);
	}
	for (auto i=m_mapAchievePointRewardInfo.begin(); i!= m_mapAchievePointRewardInfo.end(); ++i)
	{
		KKSG::STC_ACHIEVE_POINT_REWARD* pInfo = poRoleAllInfo->mutable_achieve()->add_achieveaward();
		pInfo->set_rewardid(i->second.dwRewardID);
		pInfo->set_rewardstatus(i->second.dwRewardStatus);
	}
	for (auto i = m_oldachievement.begin(); i != m_oldachievement.end(); ++i)
	{
		KKSG::StcAchieveInfo* pInfo = poRoleAllInfo->mutable_achieve()->add_oldachievement();
		pInfo->set_achieveid(i->first);
		pInfo->set_rewardstatus(i->second);
	}
	roChanged.insert(poRoleAllInfo->mutable_achieve());
}

void AchieveV2Record::Init()
{	
	m_mapAchieveInfo.clear();
	m_mapAchievePointRewardInfo.clear();
	m_ismodify.Set();
}

UINT32 AchieveV2Record::GetAchivementState(int aid)
{
	auto i = m_oldachievement.find(aid);
	return (i != m_oldachievement.end()) ? i->second : ACHST_NOT_ACHIVE;
}

void AchieveV2Record::SetAchivementState(int aid, int state)
{
	m_ismodify.Set();

	m_oldachievement[aid] = state;
	m_pRole->GetNetHandler()->SendAchivementNtf(aid, state);
}

