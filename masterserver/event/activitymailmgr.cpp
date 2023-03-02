#include "pch.h"
#include "activitymailmgr.h"
#include "timeutil.h"
#include "global/GlobalConfigDBBuild.h"
#include "util/XCommon.h"
#include "util/timespecificmgr.h"
#include "util/gametime.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "mail/mailmgr.h"
#include "spactivity/spactivitymgr.h"
#include "table/globalconfig.h"
#include "define/tlogenum.h"
#include "define/spactivityiddef.h"



INSTANCE_SINGLETON(ActivityMailMgr)

ActivityMailMgr::ActivityMailMgr()
{
	m_timer   = INVALID_TIMESPECIFIC;
	m_maxMail = 1000;
}

ActivityMailMgr::~ActivityMailMgr()
{
	Uninit();
}

bool ActivityMailMgr::Init()
{
	bool bRet = LoadFile();
	if (bRet==false)
		return false;
	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_timer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&ActivityMailMgr::OnDayTimer, this));
	
	TimeSpecific timet(HOUR_ONE_DAY_BEGIN, 3, 0);
	m_ducktimer = CTimeSpecificMgr::Instance()->RegisterEvent(timet, std::bind(&ActivityMailMgr::OnDayTimerDuck, this));
	return true;
}

void ActivityMailMgr::Uninit()
{
	ClearFile();
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timer);
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_ducktimer);
}


void ActivityMailMgr::ClearFile()
{
	m_Mail.Clear();
	Clear();
	ClearDuck();
}

bool ActivityMailMgr::LoadFile()
{
	ClearFile();
	bool bRet = m_Mail.LoadFile("table/AbyssPartyMail.txt");
	return bRet;
}

bool ActivityMailMgr::OnTrigger(std::vector<RoleMail>& vecMail)
{
	for (auto siter = vecMail.begin(); siter != vecMail.end(); siter++)
	{
		bool bRet =	CMailMgr::Instance()->SendMail(siter->nRole, siter->data);
		SSInfo << "ActivityMail-- has send mail--- "<< "RoleID=" << siter->nRole<<" time="<<GameTime::GetTime() <<" sucess="<<bRet<< END;
	}
	vecMail.clear();
	return true;
}

void ActivityMailMgr::ClearDuck()
{
	m_mapDuckMail.clear();
	for (auto iter = m_vecDuckTimer.begin(); iter != m_vecDuckTimer.end(); iter ++)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*iter);
	}
}


void ActivityMailMgr::Clear()
{
	for (auto iter = m_vecTimer.begin(); iter!=m_vecTimer.end();iter++)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*iter);
	}
	m_mapMail.clear();
}

bool ActivityMailMgr::OnDayTimer()
{
	Clear();
	UINT32 nTotal = 1;
	UINT32 nWeek =  GameTime::GetWeekDay();
	UINT32 nTime =  GameTime::GetTime();
	for (auto iter=CRoleSummaryMgr::Instance()->Begin(); iter!=CRoleSummaryMgr::Instance()->End(); iter++)
	{
		CRoleSummary *pRoleSum = iter->second;
		for (size_t i = 0; i < m_Mail.Table.size();i++)
		{
			AbyssPartyMail::RowData *pRowData = m_Mail.Table[i];
			UINT32 nLevel = pRoleSum->GetLevel();
			if (pRowData&&pRowData->day == nWeek&&nLevel>=pRowData->levelrange[0]&&nLevel<=pRowData->levelrange[1])
			{
				const MailConf* pConf = MailConfig::Instance()->GetMailConf(pRowData->mailid);
				if (pConf)
				{
					std::vector<ItemDesc> rewards;
					for (size_t i = 0; i < pConf->m_rewards.size(); ++i)
					{
						ItemDesc item(pConf->m_rewards[i][0], pConf->m_rewards[i][1]);
						rewards.push_back(item);
					}
					string szContent = pConf->m_content;
					string szTitle   = pConf->m_title;
					RoleMail roleMail;
					MailData oData(szTitle, szContent,rewards ,MAIL_EXPIRE_TIME);
					oData.SetReason(ItemFlow_AbyssLoginMail);
					roleMail.nRole = iter->second->GetID();
					roleMail.data  = oData;
					m_mapMail[nTotal/m_maxMail].push_back(roleMail);
					SSInfo << "ActivityMail--  mail--- RoleID=="<<pRoleSum->GetID()<< " MailID="<<pRowData->mailid <<" time="<<nTime<< END;
					nTotal++;
					break;
				}
			}
		}
	}
	bool bRet = CGlobalConfigDBBuild::Instance()->SaveData(eActivityMail,ToString(GameTime::GetDayBeginTime(GameTime::GetTime())));
	if (bRet)
	{
		SendMail(nTime, m_mapMail, m_vecTimer);
	}else
	{
		LogError("Save DataFail");
	}

	return true;
}

void ActivityMailMgr::SendMail(UINT32 nTime, std::map<UINT32, std::vector<RoleMail>>& mailmap, std::vector<UINT32>& vecTimer)
{
	for (auto iter = mailmap.begin(); iter != mailmap.end(); iter++)
	{
		TimeSpecific startts(nTime++);
		UINT32 dwStartHandle = CTimeSpecificMgr::Instance()->RegisterEvent(startts, std::bind(&ActivityMailMgr::OnTrigger, this, iter->second));
		vecTimer.push_back(dwStartHandle);
	}
}

bool ActivityMailMgr::OnDayTimerDuck()
{

	UINT32 nTime =  GameTime::GetTime();
	const OneActivityData* pdata = SpActivityMgr::Instance()->GetActivityData(SpActivity_Duck);
	if (NULL == pdata || nTime < pdata->m_startTime ||  pdata->m_endTime < nTime)
	{
		return true;
	}
	ClearDuck();
	
	bool bRet = CGlobalConfigDBBuild::Instance()->SaveData(eDuckTicketsMail,ToString(GameTime::GetDayBeginTime(GameTime::GetTime())));
	if (!bRet) return true;

	UINT32 nWeek =  GameTime::GetWeekDay();
	UINT32 nTotal = 1;
	for (auto iter=CRoleSummaryMgr::Instance()->Begin(); iter!=CRoleSummaryMgr::Instance()->End(); iter++)
	{
		CRoleSummary *pRoleSum = iter->second;
		if (pdata->m_minlevel<= pRoleSum->GetLevel())
		{
			std::string strMailTitle;
			std::string strMailContent;
			const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_DuckTicketsAward);
			if (pConf)
			{
				strMailTitle = pConf->m_title;
				strMailContent = pConf->m_content;
			}
			std::vector<ItemDesc> rewards;
			UINT32 itemcount = GetGlobalConfig().DayDuckTickets.seq[1];
			if (nWeek == 6 || nWeek == 7)
			{
				itemcount *= 2;
			}
			ItemDesc item(GetGlobalConfig().DayDuckTickets.seq[0], itemcount);
			rewards.push_back(item);
			RoleMail roleMail;
			MailData oData(strMailTitle, strMailContent, rewards ,MAIL_EXPIRE_TIME);
			oData.SetReason(ItemFlow_DuckDayTickets);
			roleMail.nRole = pRoleSum->GetID();
			roleMail.data  = oData;
			m_mapDuckMail[nTotal/m_maxMail].push_back(roleMail);
			SSInfo << "DuckTicketsMail--- RoleID==" << pRoleSum->GetID() <<" time="<<nTime<< END;
			nTotal++;
		}
	}
	SendMail(nTime, m_mapDuckMail, m_vecDuckTimer);
	return true;
}

void ActivityMailMgr::OnSartUp()
{
	std::string szMailTime =  CGlobalConfigDBBuild::Instance()->GetConfigValue(eActivityMail);
	UINT32 nRecTime    =  0;
	if(!szMailTime.empty())
	{
		nRecTime = convert<UINT32>(szMailTime);
	}
	UINT32 nTime      = GameTime::GetDayBeginTime(GameTime::GetTime());
	if (nRecTime&&nTime>nRecTime)
	{
		LogInfo("ActivityMailMgr OnSartUp");
		OnDayTimer();
	}
}

void ActivityMailMgr::CheckDuckUp()
{
	std::string szTime = CGlobalConfigDBBuild::Instance()->GetConfigValue(eDuckTicketsMail);
	UINT32 nLastTime = 0;
	if (!szTime.empty())
	{
		nLastTime = convert<UINT32>(szTime);
	}
	UINT32 nTime      = GameTime::GetDayBeginTime(GameTime::GetTime());
	if (nLastTime && nTime > nLastTime)
	{
		LogInfo("DuckTickest Send OnStartUp");
		OnDayTimerDuck();
	}
}

void ActivityMailMgr::SetMaxMail(UINT32 nMax)
{
	if (nMax == 0)
	{
		return;
	}
	m_maxMail = nMax;
}
