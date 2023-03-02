#include "pch.h"
#include "rolereporthandler.h"
#include "rolereport.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"
#include "loghelper/tlogr.h"

void IReportHandler::DoBanLog(SceneReportInfo* pSInfo, UINT64 roleId, UINT32 time)
{
	if (pSInfo == NULL)
	{
		return;
	}
	TRoleReportBanFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_SceneType = pSInfo->sceneType;
	oLog.m_Time = time;
	oLog.m_ReportNum = pSInfo->reportNum;
	oLog.m_BanEndTime = pSInfo->banEndTime;
	oLog.m_uRoleID = roleId;
	oLog.Do();
}

void ReportHeroBattleHandler::UpdateBan(SceneReportInfo* pSInfo, UINT64 roleId, UINT32 oldNum)
{
	if (pSInfo == NULL)
	{
		return;
	}
	UINT32 num = pSInfo->reportNum;
	for (auto it = GetGlobalConfig().HeroBattleBanTime.begin(); it != GetGlobalConfig().HeroBattleBanTime.end(); ++it)
	{
		auto& seq = *it;
		if (oldNum < seq[0] && num >= seq[0])
		{
			UINT32 time = seq[1] * 24 * 3600 + seq[2] * 3600 + seq[3] * 60 + seq[4];
			pSInfo->banEndTime = GameTime::GetTime() + time;
			DoSendHeroBattleBanMail(roleId, seq);
			DoBanLog(pSInfo, roleId, time);
		}
	}
}

void ReportHeroBattleHandler::DoSendHeroBattleBanMail(UINT64 roleId, Sequence<UINT32, 5>& timeSeq)
{
	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_HeroBeReport);
	if (conf == NULL)
	{
		return;
	}
	std::string	temp1 = MailConfig::Instance()->ReplaceString("%d", ToString(timeSeq[1]), conf->m_content);
	std::string	temp2 = MailConfig::Instance()->ReplaceString("%h", ToString(timeSeq[2]), temp1);
	std::string	temp3 = MailConfig::Instance()->ReplaceString("%m", ToString(timeSeq[3]), temp2);
	std::string	temp4 = MailConfig::Instance()->ReplaceString("%s", ToString(timeSeq[4]), temp3);

	std::vector<ItemDesc> items;
	MailData oData(conf->m_title, temp4, items, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(roleId, oData);
}

void ReportMobaBattleHandler::UpdateBan(SceneReportInfo* pSInfo, UINT64 roleId, UINT32 oldNum)
{
	if (pSInfo == NULL)
	{
		return;
	}
	UINT32 num = pSInfo->reportNum;
	for (auto it = GetGlobalConfig().HeroBattleBanTime.begin(); it != GetGlobalConfig().HeroBattleBanTime.end(); ++it)
	{
		auto& seq = *it;
		if (oldNum < seq[0] && num >= seq[0])
		{
			UINT32 time = seq[1] * 24 * 3600 + seq[2] * 3600 + seq[3] * 60 + seq[4];
			pSInfo->banEndTime = GameTime::GetTime() + time;
			DoSendMobaBattleBanMail(roleId, seq);
			DoBanLog(pSInfo, roleId, time);
		}
	}
}

void ReportMobaBattleHandler::DoSendMobaBattleBanMail(UINT64 roleId, Sequence<UINT32, 5>& timeSeq)
{
	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_MobaBeReport);
	if (conf == NULL)
	{
		return;
	}
	std::string	temp1 = MailConfig::Instance()->ReplaceString("%d", ToString(timeSeq[1]), conf->m_content);
	std::string	temp2 = MailConfig::Instance()->ReplaceString("%h", ToString(timeSeq[2]), temp1);
	std::string	temp3 = MailConfig::Instance()->ReplaceString("%m", ToString(timeSeq[3]), temp2);
	std::string	temp4 = MailConfig::Instance()->ReplaceString("%s", ToString(timeSeq[4]), temp3);

	std::vector<ItemDesc> items;
	MailData oData(conf->m_title, temp4, items, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(roleId, oData);
}