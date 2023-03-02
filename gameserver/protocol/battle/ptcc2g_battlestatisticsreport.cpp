#include "pch.h"
#include "battle/ptcc2g_battlestatisticsreport.h"
#include "battle/ptcg2c_battlestatisticsntf.h"
#include "scene/securityskillstatistics.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/securitymobstatistics.h"
#include "component/XSkillComponent.h"
#include "component/XBuffComponent.h"
#include "buff/XBuff.h"

// generate by ProtoGen at date: 2017/7/25 20:35:42

void PtcC2G_BattleStatisticsReport::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	Scene* scene = role->GetCurrScene();
	if (NULL == scene)
	{
		return;
	}

	PtcG2C_BattleStatisticsNtf ntf;

	XSecuritySkillStatistics skillData;
	skillData.Reset();

	XSecuritySkillStatistics *pSkillStatistics = XSecuritySkillStatistics::TryGetStatistics(role);
	if (pSkillStatistics)
	{
		skillData.Merge(*pSkillStatistics);
	}

	const std::map<UINT32, XSecuritySkillStatistics::XSkillInfo*>& skillInfos = skillData.GetSkillInfos();
	for(auto it = skillInfos.begin(); it != skillInfos.end(); ++it)
	{
		XSecuritySkillStatistics::XSkillInfo* pInfo = it->second;
		ntf.m_Data.add_skillid(pInfo->_SkillID);
		ntf.m_Data.add_skillcount(pInfo->_CastCount);
		ntf.m_Data.add_skillvalue(pInfo->_AttackTotal);
	}

	//////////////////////////////////////////////////////////////////////////
	XSecurityMobStatistics mobData;
	mobData.Reset();

	XSecurityMobStatistics *pMobStatistics = XSecurityMobStatistics::TryGetStatistics(role);
	if (pMobStatistics)
	{
		mobData.Merge(*pMobStatistics);
	}

	///> 找到所有还没死的召唤物并把数据弄过来
	XSkillComponent* pSkillComp = role->GetXObject()->SkillComponent();
	if (pSkillComp)
	{
		const std::vector<UINT64>& skillMobs = pSkillComp->GetSkillMobs();
		for (auto it = skillMobs.begin(); it != skillMobs.end(); ++it)
		{
			mobData.Append(Unit::FindUnit(*it));
		}
	}

	XBuffComponent* pBuffComp = role->GetXObject()->BuffComponent();
	if (pBuffComp)
	{
		const std::list<XBuff*>& buffList = pBuffComp->GetBuffList();
		for (auto it = buffList.begin(); it != buffList.end(); ++it)
		{
			if ((*it)->IsValid() && (*it)->GetEffectData()->MobID)
			{
				mobData.Append(Unit::FindUnit((*it)->GetEffectData()->MobID));
			}
		}
	}

	const std::map<UINT32, XSecurityMobStatistics::XMobInfo*>& mobInfos = mobData.GetMobInfos();
	for(auto it = mobInfos.begin(); it != mobInfos.end(); ++it)
	{
		XSecurityMobStatistics::XMobInfo* pInfo = it->second;
		ntf.m_Data.add_mobid(pInfo->_TemplateID);
		ntf.m_Data.add_mobcount(pInfo->_CastCount);
		ntf.m_Data.add_mobvalue(pInfo->_AttackTotal);
	}

	role->Send(ntf);
}
