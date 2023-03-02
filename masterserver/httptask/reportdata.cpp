#include "pch.h"
#include "reportdata.h"
#include "config.h"
#include "util/gametime.h"
#include "appid.h"
#include "util.h"
#include "guild/guildmgr.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"

CReportDataTask::CReportDataTask(CRole* role)
{
	m_strUrl = MSConfig::Instance()->GetMsdkUrl();
	BuildBaseData(role);
}

CReportDataTask::~CReportDataTask()
{

}

void CReportDataTask::BuildBaseData(CRole* role)
{
	AddData(Tx_Reportdata_LoginTime, ToString(role->GetLoginTime()));
	AddData(Tx_Reportdata_PlatType, ToString(MSConfig::Instance()->GetPlatType()));
	AddData(Tx_Reportdata_AreaID, ToString(MSConfig::Instance()->GetAppType()));
	AddData(Tx_Reportdata_ServerID, ToString(role->GetServerId()));
	AddData(Tx_Reportdata_RoleID, ToString(role->GetID()));
	AddData(Tx_Reportdata_RoleName, ToString(role->GetName()));
	AddData(Tx_Reportdata_GuildID, ToString(CGuildMgr::Instance()->GetGuildIdByRoleId(role->GetID())));
	INT32 iPro = 0;
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
	if (pSum)
	{
		iPro = pSum->GetProfession();
	}
	AddData(Tx_Reportdata_ProfessionID, ToString(iPro));
	strAppID = MobileQQ_App_ID;
	strAppKey = MobileQQ_App_Key;
	strOpenId = role->GetAccount();
	strToken = role->GetToken();
}
