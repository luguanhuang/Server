#include "pch.h"
#include "roletask.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "task/roletaskmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "task/rpcm2g_dailytaskrefreshm2g.h"
#include "define/taskdef.h"
#include "task/ptcm2c_dailytaskeventntf.h"
#include "task/rpcm2d_updateroletaskdata.h"
#include "network/dblink.h"
#include "task/ptcm2c_taskrefreshntf.h"

void DailyTaskRefreshAskInfo::FillInfo(KKSG::DailyTaskRefreshRoleInfo& info)
{
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleId);
	if (pSu == NULL)
	{
		return;
	}
	pSu->FillDailyTaskRefreshRoleInfo(info);
	CRoleTask* pTask = RoleTaskMgr::Instance()->GetRoleTask(m_roleId);
	if (pTask)
	{
		info.set_luck(pTask->m_luck);
		info.set_refresh_num(pTask->GetCanRefreshNum());
		info.set_score(pTask->m_currScore);
	}
}

void DailyTaskRefreshAskInfo::FromKKSG(const KKSG::DailyTaskRefreshRoleInfo& info)
{
	m_roleId = info.roleid();
	m_time = info.time();
}

void DailyTaskRefreshAskInfo::ToKKSG(KKSG::DailyTaskRefreshRoleInfo& info)
{
	info.set_roleid(m_roleId);
	info.set_time(m_time);
}

void DailyTaskRefreshRecord::FromKKSG(const KKSG::DailyTaskRefreshInfo& info)
{
	m_roleId = info.roleid();
	m_score = info.score();
	m_oldScore = info.old_score();
	m_time = info.time();
	m_isNew = info.isnew();
}

void DailyTaskRefreshRecord::ToKKSG(KKSG::DailyTaskRefreshInfo& info)
{
	info.set_roleid(m_roleId);
	info.set_score(m_score);
	info.set_old_score(m_oldScore);
	info.set_time(m_time);
	info.set_isnew(m_isNew);
}

CRoleTask::CRoleTask()
:m_dirty(false)
,m_roldId(0)
,m_canRefresh(true)
,m_currScore(0)
,m_luck(0)
,m_dailyRefreshCount(0)
,m_extraDailyRefreshCount(0)
,m_dailyHelpCount(0)
{

}

void CRoleTask::FromKKSG(const KKSG::RoleTaskSaveData& data)
{
	m_canRefresh = data.can_refresh();
	m_currScore = data.cur_score();
	m_luck = data.luck();
	m_dailyRefreshCount = data.daily_refresh_count();
	m_extraDailyRefreshCount = data.extra_daily_refresh_count();
	m_dailyBuyCount = data.daily_buy_count();
	m_dailyHelpCount = data.daily_help_count();

	m_askedRoleId.clear();
	for (int i = 0; i < data.asked_roleid_size(); ++i)
	{
		m_askedRoleId.insert(data.asked_roleid(i));
	}

	m_beAskHelpInfo.clear();
	for (int i = 0; i < data.be_ask_help_info_size(); ++i)
	{
		DailyTaskRefreshAskInfo info;
		info.FromKKSG(data.be_ask_help_info(i));
		m_beAskHelpInfo.push_back(info);
	}
	m_refreshRecord.clear();
	for (int i = 0; i < data.refresh_record_size(); ++i)
	{
		DailyTaskRefreshRecord info;
		info.FromKKSG(data.refresh_record(i));
		m_refreshRecord.push_back(info);
	}
}

void CRoleTask::ToKKSG(KKSG::RoleTaskSaveData& data)
{
	data.set_can_refresh(m_canRefresh);
	data.set_cur_score(m_currScore);
	data.set_luck(m_luck);
	data.set_daily_refresh_count(m_dailyRefreshCount);
	data.set_extra_daily_refresh_count(m_extraDailyRefreshCount);
	data.set_daily_buy_count(m_dailyBuyCount);
	data.set_daily_help_count(m_dailyHelpCount);

	data.clear_asked_roleid();
	for (auto it = m_askedRoleId.begin(); it != m_askedRoleId.end(); ++it)
	{
		data.add_asked_roleid(*it);
	}

	data.clear_be_ask_help_info();
	for (auto it = m_beAskHelpInfo.begin(); it != m_beAskHelpInfo.end(); ++it)
	{
		it->ToKKSG(*data.add_be_ask_help_info());
	}

	data.clear_refresh_record();
	for (auto it = m_refreshRecord.begin(); it != m_refreshRecord.end(); ++it)
	{
		it->ToKKSG(*data.add_refresh_record());
	}
}

void CRoleTask::UpdateToDB()
{
	RpcM2D_UpdateRoleTaskData* rpc = RpcM2D_UpdateRoleTaskData::CreateRpc();
	rpc->m_oArg.set_roleid(m_roldId);
	rpc->m_oArg.set_type(KKSG::DBOper_Update);
	ToKKSG(*rpc->m_oArg.mutable_data());
	DBLink::Instance()->SendTo(*rpc);
	m_dirty = false;
}

void CRoleTask::OnDayPass()
{
	bool isBack = false;
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roldId);
	if (pSu) isBack = pSu->IsBackFlowRole();
	m_luck = RoleTaskMgr::Instance()->RandomLuck(isBack);
	m_dailyRefreshCount = 0;
	m_dailyBuyCount = 0;
	m_dailyHelpCount = 0;
	m_extraDailyRefreshCount = 0;
	m_beAskHelpInfo.clear();
	SetDirty();
	UpdateLuckToGS();
}

void CRoleTask::OnNewAccept()
{
	m_canRefresh = true;
	m_askedRoleId.clear();
	m_refreshRecord.clear();
	SetDirty();
}

void CRoleTask::OnRoleLogin(CRole* pRole)
{
	UpdateLuckToGS(pRole);
	CheckHelpIcon(pRole);
	CheckBeHelpIcon(pRole);
}

bool CRoleTask::IsAsked(UINT64 roleId)
{
	return m_askedRoleId.find(roleId) != m_askedRoleId.end();
}

bool CRoleTask::CanBeRefresh()
{
	if (!m_canRefresh)
	{
		return false;
	}
	if (GetCanRefreshNum() == 0 || m_currScore == DTFQ_S)
	{
		return false;
	}
	return true;
}

bool CRoleTask::CanShowInOtherHelp()
{
	if (!m_canRefresh)
	{
		return false;
	}
	if (m_currScore == DTFQ_S)
	{
		return false;
	}
	return true;
}

UINT32 CRoleTask::GetRemainFreeRefreshNum()
{
	UINT32 maxNum = GetGlobalConfig().DailyTaskMaxRefreshTaskTimes;
	return maxNum <= m_dailyRefreshCount ? 0 : maxNum - m_dailyRefreshCount;
}

UINT32 CRoleTask::GetRemainHelpNum()
{
	return GetGlobalConfig().DailyTaskMaxHelpCount - m_dailyHelpCount;
}

KKSG::ErrorCode CRoleTask::AskHelp(UINT64 roleId)
{
	if (roleId == m_roldId)
	{
		return KKSG::ERR_FAILED;
	}
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pSu == NULL)
	{
		return KKSG::ERR_ROLE_NOTEXIST;
	}
	/*
	if (m_askedRoleId.find(roleId) != m_askedRoleId.end())
	{
		return KKSG::ERR_FAILED;
	}
	*/
	if (GetCanRefreshNum() == 0)
	{
		return KKSG::ERR_TASK_NO_REFRESH_COUNT;
	}
	CRoleTask* pOTask = RoleTaskMgr::Instance()->GetRoleTask(roleId);
	if (pOTask == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	if (pOTask->GetRemainHelpNum() == 0)
	{
		return KKSG::ERR_DAILY_TASK_OTHER_NO_HELPCOUNT;
	}
	pOTask->AddBeAskHelpInfo(m_roldId);
	m_askedRoleId.insert(roleId);
	SetDirty();

	return KKSG::ERR_SUCCESS;
}

void CRoleTask::AddBeAskHelpInfo(UINT64 roleId)
{
	DailyTaskRefreshAskInfo info;
	info.m_roleId = roleId;
	info.m_time = GameTime::GetTime();

	bool found = false;
	for (auto it = m_beAskHelpInfo.begin(); it != m_beAskHelpInfo.end(); ++it)
	{
		if (it->m_roleId == roleId)
		{
			*it = info;
			found = true;
			break;
		}
	}
	if (!found)
	{
		m_beAskHelpInfo.push_back(info);
	}

	SetDirty();
	CheckHelpIcon();
}

KKSG::ErrorCode CRoleTask::RefuseHelp(UINT64 roleId)
{
	for (auto it = m_beAskHelpInfo.begin(); it != m_beAskHelpInfo.end(); ++it)
	{
		if (it->m_roleId == roleId)
		{
			m_beAskHelpInfo.erase(it);
			break;
		}
	}
	SetDirty();
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CRoleTask::Refresh(CRole* pRole, UINT64 roleId)
{
	if (roleId == 0 || roleId == m_roldId)
	{
		return RefreshByRole(pRole);
	}
	bool found = false;
	for (auto it = m_beAskHelpInfo.begin(); it != m_beAskHelpInfo.end(); ++it)
	{
		if (it->m_roleId == roleId)	
		{
			found = true;
			break;
		}
	}
	if (found == false)
	{
		return KKSG::ERR_FAILED;
	}

	if (m_dailyHelpCount >= GetGlobalConfig().DailyTaskMaxHelpCount)
	{
		return KKSG::ERR_FAILED;
	}
	CRoleTask* pOTask = RoleTaskMgr::Instance()->GetRoleTask(roleId);
	if (pOTask == NULL)
	{
		return KKSG::ERR_ROLE_NOTEXIST;
	}
	KKSG::ErrorCode code = pOTask->RefreshByRole(pRole, m_luck);
	bool remove = false;
	if (code == KKSG::ERR_SUCCESS)
	{
		m_dailyHelpCount++;
		if (!pOTask->CanShowInOtherHelp())
		{
			remove = true;
		}
	}
	else
	{
		remove = true;
	}

	if (remove)
	{
		for (auto it = m_beAskHelpInfo.begin(); it != m_beAskHelpInfo.end(); ++it)
		{
			if (it->m_roleId == roleId)	
			{
				m_beAskHelpInfo.erase(it);
				break;
			}
		}
	}
	SetDirty();

	return code;
}

KKSG::ErrorCode CRoleTask::RefreshByRole(CRole* pRole, UINT32 luck)
{
	if (m_dailyRefreshCount >= GetGlobalConfig().DailyTaskMaxRefreshTaskTimes && m_extraDailyRefreshCount == 0)
	{
		return KKSG::ERR_DAILY_TASK_NO_REFRESH_COUNT;
	}
	if (!m_canRefresh)
	{
		return KKSG::ERR_DAILY_TASK_CANNT_REFRESH;
	}
	if (m_currScore == DTFQ_S)
	{
		return KKSG::ERR_DAILY_TASK_QUALITY_MAX;
	}
	if (pRole->GetID() == m_roldId)
	{
		luck = m_luck;
	}
	DailyTaskFormat::RowData* pData = RoleTaskMgr::Instance()->RandomDailyTaskFormat(luck);
	if (pData == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	UINT32 oldScore = m_currScore;
	m_currScore = pData->score;
	if (m_dailyRefreshCount < GetGlobalConfig().DailyTaskMaxRefreshTaskTimes)
	{
		m_dailyRefreshCount++;
	}
	else
	{
		m_extraDailyRefreshCount--;
	}
	SetDirty();

	RoleTaskMgr::Instance()->AddDailyTaskRefresh(m_roldId, pData->id);

	DailyTaskRefreshRecord info;
	info.m_roleId = pRole->GetID();
	info.m_score = pData->score;
	info.m_oldScore = oldScore;
	info.m_time = GameTime::GetTime();
	info.m_isNew = (pRole->GetID() != m_roldId);
	m_refreshRecord.push_back(info);

	CheckBeHelpIcon();
	if (pRole->GetID() != m_roldId)
	{
		PushTaskRefresh();
	}
	return KKSG::ERR_SUCCESS;
}

void CRoleTask::SetDirty(bool dirty)
{
	if (dirty == m_dirty)
	{
		return;
	}
	m_dirty = dirty;
	if (m_dirty)
	{
		RoleTaskMgr::Instance()->AddToDirty(m_roldId);
	}
}

void CRoleTask::UpdateLuckToGS(CRole* pRole)
{
	if (pRole == NULL)
	{
		pRole = CRoleManager::Instance()->GetByRoleID(m_roldId);
	}
	if (pRole == NULL)
	{
		return;
	}
	RpcM2G_DailyTaskRefreshM2G* rpc = RpcM2G_DailyTaskRefreshM2G::CreateRpc();
	rpc->m_sessionID = pRole->GetSessionID();
	rpc->m_oArg.set_type(TaskRefreshType_UpdateLuck);
	rpc->m_oArg.set_luck(m_luck);
	pRole->SendMsgToGS(*rpc);
}

void CRoleTask::CheckHelpIcon(CRole* pRole)
{
	if (pRole == NULL)
	{
		pRole = CRoleManager::Instance()->GetByRoleID(m_roldId);
	}
	if (pRole == NULL)
	{
		return;
	}
	if (GetRemainHelpNum() == 0)
	{
		return;
	}
	for (auto it = m_beAskHelpInfo.begin(); it != m_beAskHelpInfo.end(); )
	{
		CRoleTask* pOTask = RoleTaskMgr::Instance()->GetRoleTask(it->m_roleId);
		if (pOTask && !pOTask->CanShowInOtherHelp())
		{
			it = m_beAskHelpInfo.erase(it++);
			SetDirty();
		}
		else
		{
			it++;
		}
	}
	if (m_beAskHelpInfo.empty())
	{
		return;
	}

	PtcM2C_DailyTaskEventNtf ntf;
	ntf.m_Data.set_type(KKSG::DailyTaskIcon_AskHelp);
	pRole->Send(ntf);
}

void CRoleTask::CheckBeHelpIcon(CRole* pRole)
{
	if (pRole == NULL)
	{
		pRole = CRoleManager::Instance()->GetByRoleID(m_roldId);
	}
	if (pRole == NULL)
	{
		return;
	}
	if (m_refreshRecord.empty())
	{
		return;
	}
	if (m_refreshRecord.rbegin()->m_roleId != m_roldId && m_refreshRecord.rbegin()->m_isNew)
	{
		PtcM2C_DailyTaskEventNtf ntf;
		ntf.m_Data.set_type(KKSG::DailyTaskIcon_BeHelp);
		pRole->Send(ntf);
	}
}

UINT32 CRoleTask::GetCanRefreshNum()
{
	return GetRemainFreeRefreshNum() + m_extraDailyRefreshCount;
}

void CRoleTask::PushTaskRefresh()
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_roldId);
	if (pRole == NULL)
	{
		return;
	}
	PtcM2C_TaskRefreshNtf ntf;
	ntf.m_Data.set_score(m_currScore);
	ntf.m_Data.set_remain_refresh_count(GetCanRefreshNum());
	pRole->Send(ntf);
}






