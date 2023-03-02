#include "pch.h"
#include "reportdata.h"
#include "config.h"
#include "util/gametime.h"
#include "appid.h"

ReportDataTask::ReportDataTask(Role* role)
{
	m_strUrl = GSConfig::Instance()->GetSdkUrl();
	BuildBaseData(role);
}

ReportDataTask::~ReportDataTask()
{

}

void ReportDataTask::BuildBaseData(Role* role)
{
	AddData(Tx_Reportdata_LoginTime, ToString(role->GetLoginTime()));
	AddData(Tx_Reportdata_PlatType, ToString(GSConfig::Instance()->GetPlatType()));
	AddData(Tx_Reportdata_AreaID, ToString(GSConfig::Instance()->GetAppType()));
	AddData(Tx_Reportdata_ServerID, ToString(role->GetServerId()));
	AddData(Tx_Reportdata_RoleID, ToString(role->GetID()));
	AddData(Tx_Reportdata_RoleName, ToString(role->GetName()));
	AddData(Tx_Reportdata_GuildID, ToString(role->getGuildId()));
	AddData(Tx_Reportdata_ProfessionID, ToString(role->GetProfession()));
	strAppID = MobileQQ_App_ID;
	strAppKey = MobileQQ_App_Key;
	strOpenId = role->GetAccount();
	strToken = role->GetClientConf().token();
}

