#include "pch.h"
#include "reportdatahandler.h"
#include "httptask/reportdata.h"
#include "httptask/httptaskmgr.h"
#include "bag.h"
#include "util/gametime.h"
#include "payv2Record.h"
#include "reportdatarecord.h"
#include "reportdatamgr.h"
#include "rolefashion.h"

ReportDataHandler ReportDataHandler::GlobalHandler;
void ReportDataHandler::OnLevelup(Role *poRole)
{
	if (NULL == poRole || poRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(poRole);
	poTask->AddData(Tx_Reportdata_Level, ToString(poRole->GetLevel()));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataHandler::OnLogin(Role* poRole)
{
	if (NULL == poRole || poRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	poRole->Get<CReportDataRecord>()->Update();
	UINT32 weeknum = XCommon::GetWeekNum(GameTime::GetTime());
	INT32 num = 0;
	if (weeknum == 6 || weeknum == 7)
	{
		num = 1;
	}
	ReportDataTask* poTask = new ReportDataTask(poRole);
	poTask->AddData(Tx_Reportdata_WeekEndLogin, ToString(num));
	poTask->AddData(Tx_Reportdata_WeekActiveDays, ToString(poRole->Get<CReportDataRecord>()->GetWeekLoginDays()));
	UINT32 ppt = ReportDataMgr::Instance()->GetRecommendPoint(poRole->GetLevel());
	if (ppt && poRole->Get<CReportDataRecord>()->CheckRecommondTime())
	{
		INT64 point = (INT64)poRole->GetAttr(TOTAL_POWERPOINT) - (INT64)ppt;
		poTask->AddData(Tx_Reportdata_RecommondPoint, ToString(point));
	}
	if (poRole->Get<RoleFashion>()->CheckSFashion())
	{
		poTask->AddData(Tx_Reportdata_SFashion, ToString(1));
	}
	bool bHasA = false;
	bool bHasS = false;
	poRole->Get<RoleFashion>()->CheckASFashionSuit(bHasA, bHasS);
	if (bHasA)
	{
		poTask->AddData(Tx_Reportdata_AllFashion, ToString(0));
	}
	if (bHasS)
	{
		poTask->AddData(Tx_Reportdata_AllFashion, ToString(1));
	}
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataHandler::OnChangeVirtualItem(Role* poRole, UINT32 item, INT64 count)
{
	if (NULL == poRole || poRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	if(item == DIAMOND)
	{
		ReportDataTask* poTask = new ReportDataTask(poRole);
		poTask->AddData(Tx_Reportdata_Money, ToString(poRole->Get<Bag>()->CountVirtualItem(DIAMOND)));
		poTask->BuildPostData();
		HttpTaskMgr::Instance()->AddTask(poTask);
	}
}

void ReportDataHandler::OnPay(Role* poRole, UINT32 totalPay, UINT32 PayNum)
{
	if (NULL == poRole || poRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(poRole);
	poTask->AddData(Tx_Reportdata_PayNum, ToString(PayNum/100));
	poTask->AddData(Tx_Reportdata_TotalPay, ToString(totalPay/100));
	poTask->AddData(Tx_Reportdata_PayTime, ToString(GameTime::GetTime()));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataHandler::OnChangeVip(Role* poRole)
{
	if (NULL == poRole || poRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(poRole);
	poTask->AddData(Tx_Reportdata_VipLevel, ToString(poRole->Get<CPayV2Record>()->GetVipLevel()));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}
