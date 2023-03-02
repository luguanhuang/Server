#include "pch.h"
#include "tajiehelprecord.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "unit/role.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "tajiehelpconfig.h"
#include "scene/sceneconfig.h"
#include "tajie/ptcg2c_tajiehelpnotify.h"
#include "gamelogic/dragonrecord.h"
#include "scene/scene.h"
#include "scene/sceneteam.h"
#include "gamelogic/dragonconfig.h"
#include "table/DragonNestTable.h"
#include "define/systemiddef.h"

CTaJieHelpRecord::CTaJieHelpRecord(Role* role)
{
	m_pRole = role;
	m_updateTime = 0;
}

CTaJieHelpRecord::~CTaJieHelpRecord()
{

}

bool CTaJieHelpRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	const KKSG::TajieHelp2DB& tajieHelp = poRoleAllInfo->tajiehelprecord();
	for (int i = 0; i < tajieHelp.tajiehelpscenedata_size(); ++i)
	{
		const KKSG::TajieHelpSceneData& sceneData = tajieHelp.tajiehelpscenedata(i);
		TaJieHelpSceneData oneSceneData;
		oneSceneData.sceneID = sceneData.sceneid();
		oneSceneData.continueFailTimes = sceneData.intervalcontinuefailtimes();
		oneSceneData.failNoticeTimes = sceneData.intervalfailnoticetimes();

		m_tajieHelpSceneData[oneSceneData.sceneID] = oneSceneData;
	}
	m_updateTime = tajieHelp.updatetime();

	return true;
}

void CTaJieHelpRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::TajieHelp2DB* data = poRoleAllInfo->mutable_tajiehelprecord();
		data->Clear();
		for (auto iter = m_tajieHelpSceneData.begin(); iter != m_tajieHelpSceneData.end(); ++iter)
		{
			KKSG::TajieHelpSceneData* onescenedata = data->add_tajiehelpscenedata();
			onescenedata->set_sceneid(iter->second.sceneID);
			onescenedata->set_intervalcontinuefailtimes(iter->second.continueFailTimes);
			onescenedata->set_intervalfailnoticetimes(iter->second.failNoticeTimes);
		}
		data->set_updatetime(m_updateTime);
		roChanged.insert(data);
	}
}

void CTaJieHelpRecord::TajieSceneProcess(Scene* scene, bool success)
{
	if (!scene)
	{
		return;
	}
	if (!m_pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(SysTaJieHelp))
	{
		return;
	}
	if (success)
	{
		_TajieHelpSceneSuccess(scene);
	}
	else
	{
		_AddTajieHelpFailTimes(scene);
	}
}

void CTaJieHelpRecord::_AddTajieHelpFailTimes(Scene* pScene)
{
	UINT32 sceneID = pScene->GetSceneTemplateID();
	if (pScene->IsHelper(m_pRole->GetRoleID()))
	{
		return;
	}
	const TaJieHelpUrl::RowData* table =  TajieHelpConfig::Instance()->GetTaJieHelpDataByScene(sceneID);
	if (!table)
	{
		return;
	}
	TaJieHelpSceneData* data = NULL;
	auto find = m_tajieHelpSceneData.find(sceneID);
	if (find != m_tajieHelpSceneData.end())
	{
		data = &find->second;
		find->second.continueFailTimes += 1;
	}
	else
	{
		TaJieHelpSceneData oneData;
		oneData.sceneID = sceneID;
		oneData.continueFailTimes = 1;
		m_tajieHelpSceneData[sceneID] = oneData;
		data = &m_tajieHelpSceneData[sceneID];
	}
	
	if (data->continueFailTimes >= FAIL_NOTICE_TIMES && data->failNoticeTimes < table->Count)
	{
		++data->failNoticeTimes;
		_HelpNotice(pScene, false);
	}

	SetChanged();
}

void CTaJieHelpRecord::_TajieHelpSceneSuccess(Scene* scene)
{
	UINT32 sceneID = scene->GetSceneTemplateID();
	const TaJieHelpUrl::RowData* table =  TajieHelpConfig::Instance()->GetTaJieHelpDataByScene(sceneID);
	if (!table)
	{
		return;
	}

	auto find = m_tajieHelpSceneData.find(sceneID);
	if (find != m_tajieHelpSceneData.end() && find->second.continueFailTimes)
	{
		_HelpNotice(scene, true);
		find->second.continueFailTimes = 0;
		SetChanged();
	}
}

void CTaJieHelpRecord::_HelpNotice(Scene* pScene, bool success)
{
	UINT32 sceneID = pScene->GetSceneTemplateID();
	SceneInfo* info = SceneConfig::Instance()->FindSceneInfo(sceneID);
	if (!info || !info->m_pConf)
	{
		return;
	}

	PtcG2C_TajieHelpNotify ntf;
	ntf.m_Data.set_sceneid(sceneID);
	ntf.m_Data.set_scenetype(info->m_pConf->type);
	ntf.m_Data.set_isiconappear(!success);
	if (info->m_pConf->type == KKSG::SCENE_DRAGON && !success)
	{
		auto dragonRecord = m_pRole->Get<CDragonRecord>();
		SceneTeam* pTeam = pScene->GetSceneTeam();
		if (!pTeam)
		{
			return;
		}
		auto dragonTable = DragonConfig::Instance()->GetDragonTableData(pTeam->GetExpID());
		if(!dragonTable)
			return;
		int passTime = DragonConfig::Instance()->GetGlobalPassTimeByExpID(pTeam->GetExpID());
		if (!passTime)
		{
			ntf.m_Data.set_dragonstatus(0);
			ntf.m_Data.set_dragonweaklefttime(0);
		}
		else
		{
			int now = GameTime::GetTime();
			std::vector<UINT32> refreshDays;
			if (dragonTable->DragonNestDifficulty == 1)
			{
				refreshDays = GetGlobalConfig().DragonResetWeekDay;
			}
			else if (dragonTable->DragonNestDifficulty == 2)
			{
				refreshDays = GetGlobalConfig().HardDragonResetWeekDay;
			}
			else if (dragonTable->DragonNestDifficulty == 0)
			{
				refreshDays = GetGlobalConfig().SmallDragonResetWeekDay;
			}
			int time = CDragonRecord::GetNextActivityTime(refreshDays);
			if (!time)
			{
				return;
			}

			int refreshTimes = CDragonRecord::CalcDragonRefreshTimes(passTime, dragonTable->DragonNestDifficulty);
			if (dragonTable->WeakInfo.empty())
			{
				ntf.m_Data.set_dragonstatus(2);
				ntf.m_Data.set_dragonweaklefttime(0);
			}
			else if(refreshTimes >= dragonTable->WeakInfo.back().seq[0])
			{
				ntf.m_Data.set_dragonstatus(2);
				ntf.m_Data.set_dragonweaklefttime(0);
			}
			else
			{
				ntf.m_Data.set_dragonstatus(1);
				ntf.m_Data.set_dragonweaklefttime(time > now ? time - now : 0);
			}
		}
	}

	m_pRole->Send(ntf);
}

void CTaJieHelpRecord::OnDayPass()
{
	int now = GameTime::GetTime();
	if (!XCommon::IsDaySame(m_updateTime, now))
	{
		int openTime = m_pRole->Get<CRoleSystem>()->GetSystemOpenTime(SysTaJieHelp);
		if (!openTime)
		{
			return;
		}
		for (auto iter = m_tajieHelpSceneData.begin(); iter != m_tajieHelpSceneData.end(); ++iter)
		{
			auto data = TajieHelpConfig::Instance()->GetTaJieHelpDataByScene(iter->first);
			if (!data || !data->Cycle)
			{
				continue;
			}

			int updateIntervalCount = m_updateTime > openTime ? (m_updateTime - openTime) / data->Cycle : 0;
			int nowIntervalCount = now > openTime ? (now - openTime) / data->Cycle : 0;
			if (updateIntervalCount != nowIntervalCount)
			{
				iter->second.failNoticeTimes = 0;
			}

		}
		m_updateTime = now;
		SetChanged();
	}
}

void CTaJieHelpRecord::SetChanged()
{
	m_switch.Set();
}

void CTaJieHelpRecord::OnLogin()
{
	OnDayPass();
}
