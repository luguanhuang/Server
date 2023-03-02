#include "pch.h"
#include <time.h>
#include "marriagemgr.h"
#include "marriage.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "util/timespecificmgr.h"
#include "util/gametime.h"
#include "util.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "network/dblink.h"
#include "marriage/ptcm2d_savemarriageinfontf.h"
#include "role/role.h"
#include "marriagemsg.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "mail/maildata.h"
#include "util/XCommon.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "table/globalconfig.h"
#include "network/gslink.h"
#include "event/eventmgr.h"
#include "foreach.h"
#include "marriagerelationmgr.h"

INSTANCE_SINGLETON(MarriageMgr)

#define UPDATE_INTERVAL 72*1000

MarriageMgr::MarriageMgr()
{

}

MarriageMgr::~MarriageMgr()
{
	foreach(i in m_marriageList)
	{
		delete i->second;
	}
	m_marriageList.clear();
	m_changedList.clear();
}

void MarriageMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	int now = GameTime::GetTime();
	for (auto iter = m_autoBreakList.begin(); iter != m_autoBreakList.end();)
	{
		if (now >= iter->second)
		{
			AutoBreak(GetMarriage(iter->first.m_srcID), GetMarriage(iter->first.m_destID));
			m_autoBreakList.erase(iter++);
		}
		else
			++iter;
	}

	Save2Db();
}

bool MarriageMgr::Init()
{
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, UPDATE_INTERVAL, -1, __FILE__, __LINE__);

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_dayTimer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&MarriageMgr::OnDayPass, this));

	LoadFromDb();

	return true;
}

void MarriageMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timer);
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_dayTimer);
}

bool MarriageMgr::OnDayPass()
{
	for (auto iter = m_marriageList.begin(); iter != m_marriageList.end(); ++iter)
	{
		iter->second->GetMarriageMsg()->Update();
		iter->second->OnDayPass();
	}

	if (GameTime::GetWeekDay() == 1)
	{
		OnWeekPass();
	}
	return true;
}

void MarriageMgr::OnWeekPass()
{

}

void MarriageMgr::OnLogin(CRole* role)
{
	Marriage* marriage = GetMarriage(role->GetID());
	if (marriage)
	{
		marriage->OnRoleLogin(role);
	}
}

void MarriageMgr::OnLogout(CRole* role)
{
	Marriage* marriage = GetMarriage(role->GetID());
	if (!marriage)
	{
		return;
	}
}

void MarriageMgr::OnEnterHall(CRole* role)
{
	Marriage* marriage = GetMarriage(role->GetID());
	if (marriage && marriage->HasHistory() && !marriage->IsInMarriagePos())
	{
		RpcM2G_MSConsumeItems* rpc1 = RpcM2G_MSConsumeItems::CreateRpc();
		rpc1->m_oArg.set_role_id(role->GetID());
		rpc1->m_oArg.set_reason(ItemFlow_Marriage);
		rpc1->m_oArg.set_subreason(ItemFlow_Marriage_Divorce);
		KKSG::ItemBrief* brief = rpc1->m_oArg.add_items();
		brief->set_itemid(GetGlobalConfig().MarriageRing);
		brief->set_itemcount(1);
		brief->set_isbind(true);

		brief = rpc1->m_oArg.add_items();
		brief->set_itemid(GetGlobalConfig().MarriageSplendidRing);
		brief->set_itemcount(1);
		brief->set_isbind(true);

		GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc1);
	}
}

bool MarriageMgr::LoadFromDb()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from marriage";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table marriage failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT32 count = 0;
	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT64 qwRoleID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			qwRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		KKSG::MarriageAllData2DB dbinfo;

		std::string strData;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
			dbinfo.ParseFromString(strData);
		}
		else
		{
			LogError("Read 'data' column failed");
			return false;
		}
		
		_ParseFromDb(qwRoleID, dbinfo);
		++count;
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all marriage data, size:%u from db use time: %llu ms", count,qwTimeUsed);

	return true;
}

void MarriageMgr::Save2Db(bool isAll)
{
	for (auto iter = m_changedList.begin(); iter != m_changedList.end(); ++iter)
	{
		Marriage* pMarriage = GetMarriage(*iter);
		if (pMarriage)
		{
			PtcM2D_SaveMarriageInfoNtf ntf;
			KKSG::MarriageAllData2DB tempData;
			pMarriage->Save(&tempData);
			ntf.m_Data.set_roleid(*iter);
			ntf.m_Data.set_info(tempData.SerializeAsString());
			DBLink::Instance()->SendTo(ntf);
		}
	}
	LogInfo("save marriage size:%u", m_changedList.size());
	m_changedList.clear();
}

void MarriageMgr::QuickSave(Marriage* pOneSide)
{
	if (!pOneSide)
	{
		return;
	}
	PtcM2D_SaveMarriageInfoNtf ntf;
	KKSG::MarriageAllData2DB tempData;
	pOneSide->Save(&tempData);
	ntf.m_Data.set_roleid(pOneSide->GetOwnerID());
	ntf.m_Data.set_info(tempData.SerializeAsString());
	DBLink::Instance()->SendTo(ntf);

	m_changedList.erase(pOneSide->GetOwnerID());

	LogInfo("quick save marriage size:%llu", pOneSide->GetOwnerID());
}

void MarriageMgr::AddChanged(UINT64 roleID)
{
	m_changedList.insert(roleID);
}


void MarriageMgr::ChangeDesName(UINT64 roleID,const std::string &name)
{
	Marriage* pMarriage =  GetMarriage(roleID);
	if (pMarriage)
	{
		EventMgr::Instance()->AddEvent(pMarriage->GetCoupleID(),DESIGNATION_COM_TYPE_MARRY,pMarriage->GetMarriagePos(),GlobalEvent::EVENT_OP_MODIFY,name);
	}
}



Marriage* MarriageMgr::GetMarriage(UINT64 roleID, bool force)
{
	auto find = m_marriageList.find(roleID);
	if (find == m_marriageList.end())
	{
		if (!force)
		{
			auto findSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID);
			if (!findSummary)
			{
				return NULL;
			}
		}

		auto ptr = new Marriage(roleID);
		m_marriageList.insert(std::make_pair(roleID, ptr));

		return ptr;
	}

	return find->second;
}

void MarriageMgr::AutoBreak(Marriage* pMarriage1, Marriage* pMarriage2)
{
	if (!pMarriage1 || !pMarriage2)
	{
		return;
	}

	UINT32 type = pMarriage1->GetMarriageType();
	UINT32 ringID = 0;
	if (type == KKSG::WeddingType_Normal)
	{
		ringID = GetGlobalConfig().MarriageRing;
	}
	else if(type == KKSG::WeddingType_Luxury)
	{
		ringID = GetGlobalConfig().MarriageSplendidRing;
	}

	pMarriage1->Divorce();
	pMarriage2->Divorce();

	QuickSave(pMarriage1);
	QuickSave(pMarriage2);

	RpcM2G_MSConsumeItems* rpc1 = RpcM2G_MSConsumeItems::CreateRpc();
	rpc1->m_oArg.set_role_id(pMarriage1->GetOwnerID());
	rpc1->m_oArg.set_reason(ItemFlow_Marriage);
	rpc1->m_oArg.set_subreason(ItemFlow_Marriage_Divorce);
	KKSG::ItemBrief* brief = rpc1->m_oArg.add_items();
	brief->set_itemid(ringID);
	brief->set_itemcount(1);
	brief->set_isbind(true);
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc1);

	RpcM2G_MSConsumeItems* rpc2 = RpcM2G_MSConsumeItems::CreateRpc();
	rpc2->m_oArg.CopyFrom(rpc1->m_oArg);
	rpc2->m_oArg.set_role_id(pMarriage2->GetOwnerID());
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc2);

	CRoleSummary* summary1 = CRoleSummaryMgr::Instance()->GetRoleSummary(pMarriage1->GetOwnerID());
	CRoleSummary* summary2 = CRoleSummaryMgr::Instance()->GetRoleSummary(pMarriage2->GetOwnerID());
	if (!summary1 || !summary2)
	{
		return;
	}

	EventMgr::Instance()->AddEvent(pMarriage1->GetOwnerID(),DESIGNATION_COM_TYPE_MARRY,pMarriage2->GetMarriagePos(),GlobalEvent::EVENT_OP_DEL,summary2->GetName());
	EventMgr::Instance()->AddEvent(pMarriage2->GetOwnerID(),DESIGNATION_COM_TYPE_MARRY,pMarriage1->GetMarriagePos(),GlobalEvent::EVENT_OP_DEL,summary1->GetName());

	// ·¢ÓÊ¼þ
	std::string mailtitle = "null";
	std::string mailcontent;
	std::string mailcontent2;
	std::vector<ItemDesc> rewards;
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_MarriageDivorce);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
		XCommon::ReplaceOneStr(mailcontent, "%s", summary2->GetName());

		mailcontent2 = pMailConf->m_content;
		XCommon::ReplaceOneStr(mailcontent2, "%s", summary1->GetName());
	}
	MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(summary1->GetID(), maildata);

	MailData maildata2(mailtitle, mailcontent2, rewards, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(summary2->GetID(), maildata2);

	MarriageRelationMgr::Instance()->DoTxLog(pMarriage1->GetOwnerID(), pMarriage2->GetOwnerID(), TMarriageType5, type);
}

void MarriageMgr::AddAutoBreak(BreakApplyPair& pair, int time)
{
	m_autoBreakList.insert(std::make_pair(pair, time));
}

void MarriageMgr::RemoveAutoBreak(BreakApplyPair& pair)
{
	m_autoBreakList.erase(pair);
}

void MarriageMgr::CheckMarriageActivity(UINT32 endTime)
{
	for (auto iter = m_marriageList.begin(); iter != m_marriageList.end(); ++iter)
	{
		iter->second->CheckGetMarriageActivityReward(endTime);
	}
}

bool MarriageMgr::_ParseFromDb(UINT64 roleID, const KKSG::MarriageAllData2DB& data)
{
	Marriage* pMarriage = GetMarriage(roleID);
	if (!pMarriage)
	{
		return false;
	}

	pMarriage->Load(data);

	return true;
}

