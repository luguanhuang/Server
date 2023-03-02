#include "pch.h"
#include "survivemgr.h"
#include "network/dblink.h"
#include "util/gametime.h"
#include "global/GlobalConfigDBBuild.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/surviveranklist.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "mail/maildata.h"
#include "event/eventmgr.h"

CSurviveMgr::CSurviveMgr()
{
	m_handler = 0;
}

CSurviveMgr::~CSurviveMgr()
{

}

bool CSurviveMgr::Init()
{
	if(!ReadData())
	{
		return false;
	}
	if(!LoadFile())
	{
		return false;
	}
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);	
	return true;
}

void CSurviveMgr::Uninit()
{
	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}
}

void CSurviveMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if(!DBLink::Instance()->IsConnected())	
	{
		return;
	}
	UINT32 now = UINT32(GameTime::GetTime());
	UINT32 changedFlag = false;
	if(0 == m_data.lastweekuptime())
	{
		m_data.set_lastweekuptime(now);
		changedFlag = true;
	}
	if(!GameTime::IsInSameWeek(m_data.lastweekuptime(), now, true))
	{
		changedFlag = true;
		m_data.set_lastweekuptime(now);

		WeekZero();
	}

	if(changedFlag)
	{
		WriteData();
	}
}

bool CSurviveMgr::ReadData()
{
	std::string tmp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eSurviveGlobalData);
	if(!tmp.empty())
	{
		if(!m_data.ParseFromString(tmp))
		{
			SSError << " parse survive global data failed " << END;
			return false;
		}
	}
	else
	{
		SSInfo << " first time " << END;
	}
	return true;
}

void CSurviveMgr::WriteData()
{
	std::string tmp;
	if(!m_data.SerializeToString(&tmp))
	{
		SSError << " survive global data serialize to string failed " << END;
		return;
	}
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eSurviveGlobalData, tmp);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eSurviveGlobalData);
}

void CSurviveMgr::WeekZero()
{
	auto srlist = GetRankList();
	if(NULL == srlist)
	{
		return;
	}
	std::vector<SurviveRankInfo*> allInfo;
	srlist->GetAllInfo(allInfo);
	
	for(size_t i = 0; i < allInfo.size(); ++i)
	{
		auto info = allInfo[i]; 
		if(NULL == info)
		{
			continue;
		}
		UINT32 rank = i + 1;
		UINT64 roleID = info->RoleID();
		UINT32 serverID = info->ServerID();
		//if(!MSConfig::Instance()->HasServerId(serverID))	
		//{
		//	continue;
		//}
		GiveReward(roleID, rank);
	}

	srlist->Clear();
	allInfo.clear();
}

void CSurviveMgr::InfoPrint()
{
	SSInfo << " lastweekuptime = " << m_data.lastweekuptime() << END;

	auto srlist = GetRankList();
	if(srlist)
	{
		srlist->InfoPrint();
	}
}

SurviveRankList* CSurviveMgr::GetRankList()
{
	auto pRankList = CRankListMgr::Instance()->GetRankList(KKSG::SurviveRank);
	if(NULL == pRankList)
	{
		return NULL;
	}
	SurviveRankList* srlist = static_cast<SurviveRankList*>(pRankList);
	return srlist;
}

void CSurviveMgr::GiveReward(UINT64 roleID, UINT32 rank)
{
	if (rank<=10)
	{
		EventMgr::Instance()->AddEvent(roleID, DESIGNATION_COM_TYPE_SURVIVE,rank);
	}

	SSDebug << __FUNCTION__ << " role = " << roleID << " rank = " << rank << END;
	auto pConf = GetConf(rank);
	if(NULL == pConf)
	{
		SSError << " get conf failed rank = " << rank << " role = " << roleID << END;
		return;
	}

	std::string mailtitle = "null";
	std::string mailcontent = "null";
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(pConf->mailconf);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
	}
	std::vector<ItemDesc> rewards;
	for(size_t i = 0; i < pConf->reward.size(); ++i)
	{
		ItemDesc itemd(pConf->reward[i][0], pConf->reward[i][1], true);
		rewards.push_back(itemd);
	}
	MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
	maildata.SetReason(ItemFlow_Survive_RankRward);
	CMailMgr::Instance()->SendMail(roleID, maildata);
}

bool CSurviveMgr::LoadFile()
{
	if(!CheckConfig())
	{
		return false;
	}

	ClearConfig();

	m_rrTable.LoadFile("table/ChickenDinnerRankReward.txt");

	return true;
}

bool CSurviveMgr::CheckConfig()
{
	ChickenDinnerRankReward tmpRrTable;	
	if(!tmpRrTable.LoadFile("table/ChickenDinnerRankReward.txt"))
	{
		SSError << " load file ChickenDinnerRankReward.txt failed " << END;
		return false;
	}
	return true;
}

void CSurviveMgr::ClearConfig()
{
	m_rrTable.Clear();
}

ChickenDinnerRankReward::RowData* CSurviveMgr::GetConf(UINT32 rank)
{
	for(size_t i = 0; i < m_rrTable.Table.size(); ++i)
	{
		auto pConf = m_rrTable.Table[i];
		if(pConf->rank[0] <= rank && rank <= pConf->rank[1])
		{
			return pConf;
		}
	}
	return NULL;
}
