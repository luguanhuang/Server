#include "pch.h"
#include "title/ptcg2m_getmaxtitlentf.h"
#include "global/GlobalConfigDBBuild.h"
#include "notice/notice.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2016/9/11 18:21:17

void PtcG2M_GetMaxTitleNtf::Process(UINT32 dwConnID)
{
	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_Data.uid());
	if(NULL == pRoleSummary)
	{
		LogError("can't find rolesummary:%llu in masterserver", m_Data.uid());
		return;
	}
	UINT32 titleMax = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eTitleMaxLevel));
	UINT32 titleMaxCnt = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eTitleMaxLevelCnt));
	if(m_Data.titlemaxlevelnow() != titleMax)
	{
		titleMax = m_Data.titlemaxlevelnow();
		titleMaxCnt = 1;
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eTitleMaxLevel, ToString(titleMax));
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eTitleMaxLevel);
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eTitleMaxLevelCnt, ToString(titleMaxCnt));
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eTitleMaxLevelCnt);

		Notice notice(TitleFirstGetMax);
		notice.AddRole(pRoleSummary);
		notice.Replace("$C", m_Data.titlename());
		notice.Send();
	}
	else
	{
		++titleMaxCnt;
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eTitleMaxLevelCnt, ToString(titleMaxCnt));
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eTitleMaxLevelCnt);
		if(titleMaxCnt <= 10)
		{
			Notice notice(TitleNextGetMax);
			notice.AddRole(pRoleSummary);
			notice.Add(titleMaxCnt);
			notice.Replace("$C", m_Data.titlename());
			notice.Send();
		}
	}
}
