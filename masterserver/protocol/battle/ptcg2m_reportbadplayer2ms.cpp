#include "pch.h"
#include "battle/ptcg2m_reportbadplayer2ms.h"
#include "report/reportmgr.h"
#include "loghelper/tlogr.h"

// generate by ProtoGen at date: 2017/5/11 2:11:35

void PtcG2M_ReportBadPlayer2Ms::Process(UINT32 dwConnID)
{
	RoleReportInfo* pInfo = ReportMgr::Instance()->GetRoleReportInfo(m_Data.roleid(), true);
	if (pInfo == NULL)
	{
		return;
	}
	pInfo->AddReportNum(m_Data.scenetype(), m_Data.score());
	LogInfo("Report sceneType [%u] Role[%llu] by Role[%llu] score[%d]", m_Data.scenetype(), m_Data.roleid(), m_Data.report_roleid(), m_Data.score());

	TRoleReportFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_SceneType = m_Data.scenetype();
	oLog.m_Num = m_Data.score();
	oLog.m_ReportNum = pInfo->GetReportNum(m_Data.scenetype());
	oLog.m_uRoleID = m_Data.roleid();
	//oLog.m_strOpenId;
	oLog.m_RoleGroup = m_Data.group();
	oLog.m_RoleId2 = m_Data.report_roleid();
	oLog.m_strOpenId2 = m_Data.report_openid();
	oLog.m_RoleGroup2 = m_Data.report_group();
	std::ostringstream ss;
	for (INT32 i = 0; i < m_Data.reason_size(); i ++)
	{
		if (i > 0)
		{
			ss << ",";
		}
		ss << m_Data.reason(i);
	}
	oLog.m_strReason = ss.str();
	oLog.m_op = m_Data.ismvp()?1:0;
	oLog.Do();
}
