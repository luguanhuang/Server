#include "pch.h"
#include "dblink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "chat/chatmgr.h"
#include "levelseal/levelsealMgr.h"
#include "guild/guildmgr.h"
#include "social/socialmgr.h"
#include "process.h"
#include "rslink.h"
#include "garden/garden_manager.h"
#include "guild/guildmgr.h"
#include "partner/partnermgr.h"
#include "event/activitymailmgr.h"
#include "ranklist/ranklistmgr.h"
#include "task/roletaskmgr.h"

INSTANCE_SINGLETON(DBLink)

DBLink::DBLink()
:m_dwConnID(INVALID_CONN_ID)
{
}

DBLink::~DBLink()
{
}

void DBLink::OnConnectSucceed(UINT32 dwConnID)
{
	if(m_dwConnID != INVALID_CONN_ID)
	{
		LogError("more than one db is connected!");
		return;
	}
	SetMsSaveDBEnd(false);

	SetConnID(dwConnID);
	LogInfo("connect to db success");

	///>起服时是否需要回写封印数据
	if (CLevelSealMgr::Instance()->GetStartServerSaveFlag())
	{
		CLevelSealMgr::Instance()->SetStartServerSaveFlag(false);
		CLevelSealMgr::Instance()->SendLevelSealInfoToDB();
	}

	SocialMgr::Instance()->StartUpFlowerEvent();
	GardenManager::Instance()->StartUpGardenEvent();
	CGuildMgr::Instance()->StartUpGuildmgrEvent();
	PartnerMgr::Instance()->CheckDayPass();
	ActivityMailMgr::Instance()->OnSartUp();
	CRankListMgr::Instance()->OnSartUp();
	RoleTaskMgr::Instance()->CheckDayPass();


	RSLink::Instance()->NotifyRouter2DB();	
}

void DBLink::OnConnectFailed()
{
	LogError("connect to db failed!");
}

void DBLink::OnClose(UINT32 dwConnID, int err)
{
	SetConnID(INVALID_CONN_ID);
	LogInfo("db link was disconnected");
	SetMsSaveDBEnd(true);

	/*PtcG2T_OpenGate oPtc;
	oPtc.m_Data.set_flag(false);
	GatetLink::Instance()->BroadCast(oPtc);
	*/
}

bool DBLink::SendTo(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roPtc);
}

bool DBLink::SendTo(CRpc &roRpc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roRpc);
}

const std::string& DBLink::GetName()
{
	static const std::string LinkName = "dblink";
	return LinkName;
}
