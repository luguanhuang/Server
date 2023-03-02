#include "pch.h"
#include "rolereport.h"
#include "report/reportmgr.h"
#include "db/rpcm2d_updaterolereportdata.h"
#include "network/dblink.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"

void ReportInfo::FromKKSG(const KKSG::ReportBadPlayerData& data)
{
	reportRoleId = data.report_roleid();
	time = data.time();
	sceneType = data.scenetype();
	reasons.clear();
	for (int i = 0; i < data.reason_size(); ++i)
	{
		reasons.push_back(data.reason(i));
	}
}

void ReportInfo::ToKKSG(KKSG::ReportBadPlayerData& data)
{
	data.set_report_roleid(reportRoleId);
	data.set_time(time);
	data.set_scenetype(sceneType);
	data.clear_reason();
	for (auto it = reasons.begin(); it != reasons.end(); ++it)
	{
		data.add_reason(*it);
	}
}

void SceneReportInfo::FromKKSG(const KKSG::RoleSceneReportData& data)
{
	sceneType = data.scenetype();
	banEndTime = data.banendtime();
	reportNum = data.reportnum();
}

void SceneReportInfo::ToKKSG(KKSG::RoleSceneReportData& data)
{
	data.set_scenetype(sceneType);
	data.set_banendtime(banEndTime);
	data.set_reportnum(reportNum);
}

RoleReportInfo::RoleReportInfo(): 
m_dirty(false), m_roleId(0), m_updateTime(0)
{
	m_updateTime = GameTime::GetTime();
}

void RoleReportInfo::FromKKSG(const KKSG::RoleReportData& data)
{
	m_updateTime = data.updatetime();
	m_infos.clear();
	for (int i = 0; i < data.data_size(); ++i)
	{
		const KKSG::RoleSceneReportData& sceneInfo = data.data(i);
		m_infos[sceneInfo.scenetype()].FromKKSG(sceneInfo);
	}
}

void RoleReportInfo::ToKKSG(KKSG::RoleReportData& data)
{
	data.set_updatetime(m_updateTime);
	data.clear_data();
	for (auto it = m_infos.begin(); it != m_infos.end(); ++it)
	{
		it->second.ToKKSG(*data.add_data());
	}
}

void RoleReportInfo::UpdateToDB()
{
	if (m_dirty == false)
	{
		return;
	}
	m_dirty = false;
	RpcM2D_UpdateRoleReportData* rpc = RpcM2D_UpdateRoleReportData::CreateRpc();
	rpc->m_oArg.set_roleid(m_roleId);
	ToKKSG(*rpc->m_oArg.mutable_data());
	rpc->m_oArg.set_type(KKSG::DBOper_Update);
	DBLink::Instance()->SendTo(*rpc);
}

SceneReportInfo* RoleReportInfo::GetSceneReportInfo(UINT32 sceneType, bool add)
{
	auto it = m_infos.find(sceneType);
	if (it != m_infos.end())
	{
		return &it->second;
	}
	if (!add)
	{
		return NULL;
	}
	SceneReportInfo info;
	info.sceneType = sceneType;
	auto p = m_infos.insert(std::make_pair(sceneType, info));
	return &p.first->second;
}

void RoleReportInfo::AddReportNum(UINT32 sceneType, int num)
{
	SceneReportInfo* pSReport = GetSceneReportInfo(sceneType, true);
	if (pSReport == NULL)
	{
		return;
	}
	UINT32 oldNum = pSReport->reportNum;
	if (num < 0 && pSReport->reportNum < (-num))
	{
		pSReport->reportNum = 0;
	}
	else
	{
		pSReport->reportNum += num;
	}
	SetDirty(true);

	if (oldNum == 0 && pSReport->reportNum > 0)
	{
		ReportMgr::Instance()->AddTotalNum(sceneType);
	}
	if (oldNum > 0 && pSReport->reportNum == 0)
	{
		ReportMgr::Instance()->DecTotalNum(sceneType);
	}

	IReportHandler* pHandler = ReportMgr::Instance()->GetHandler(sceneType);
	if (pHandler)
	{
		pHandler->UpdateBan(pSReport, m_roleId, oldNum);
	}
}

UINT32 RoleReportInfo::GetReportNum(UINT32 sceneType)
{
	SceneReportInfo* pSReport = GetSceneReportInfo(sceneType);
	if (pSReport == NULL)
	{
		return 0;
	}
	return pSReport->reportNum;
}

void RoleReportInfo::SetDirty(bool dirty)
{
	if (m_dirty == false && dirty)
	{
		ReportMgr::Instance()->AddToDirtyList(m_roleId);
	}
	m_dirty = dirty;
}

UINT32 RoleReportInfo::GetBanTime(UINT32 sceneType)
{
	SceneReportInfo* pSceneInfo = GetSceneReportInfo(sceneType, false);
	if (pSceneInfo == NULL)
	{
		return 0;
	}
	UINT32 now = GameTime::GetTime();
	return  (pSceneInfo->banEndTime > now) ? (pSceneInfo->banEndTime - now) : 0;
}

void RoleReportInfo::ResetBanTime(UINT32 sceneType)
{
	SceneReportInfo* pSceneInfo = GetSceneReportInfo(sceneType, false);
	if (pSceneInfo == NULL)
	{
		return;
	}
	pSceneInfo->banEndTime = 0;
}

void RoleReportInfo::CheckWeekReset()
{
	if (XCommon::IsWeekSame(GameTime::GetTime(), m_updateTime))
	{
		return ;
	}
	m_updateTime = GameTime::GetTime();
	m_infos.clear();
	SetDirty(true);
}