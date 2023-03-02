#include "pch.h"
#include "event/eventmgr.h"
#include "network/gslink.h"
#include "db/mysqlmgr.h"
#include "network/dblink.h"
#include "util.h"
#include "role/rolemanager.h"
#include "desvent/ptcm2d_updatedesignation.h"
#include "designation/rpcm2g_designationeventreq.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildbonus.h"
#include "util/gametime.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentoroneside.h"
#include "mentorship/mentortask.h"


INSTANCE_SINGLETON(EventMgr)

EventMgr::EventMgr():m_nIndex(0),m_nMaxIndex((1 << 12) -1){}

EventMgr::~EventMgr(){}

bool EventMgr::Init()
{
	m_nTime = GameTime::GetTime() + EVENT_REFRESH_TIME;
	_ReadAllRoleEventFromDB();
	return true;
}

void EventMgr::Uninit()
{
	_UpdateToDB(true);
}

void EventMgr::Update()
{
	if(GameTime::GetTime() < m_nTime) 
		return;
	m_nTime += EVENT_REFRESH_TIME;
	_UpdateToDB();
}


void EventMgr::OnChangedGS(CRole* pRole)
{
	_SendEvent(pRole);
}

void EventMgr::_AddEvent(UINT64 nRoleID,GlobalEvent::stEvent *pEvent)
{
	LogInfo("EventMgr::AddEvent====nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu, ntime=%u",
		nRoleID,pEvent->nID, pEvent->nParam1,pEvent->nParam2,pEvent->nParam3,pEvent->lParam1,pEvent->lParam2, pEvent->nTime);
	auto& ref = mapEvent[nRoleID];
	ref.bUpdate = true;
	ref.mapAllEvent.insert(std::pair<UINT64, GlobalEvent::stEvent>(NewEventID(pEvent->nID), *pEvent));
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(nRoleID);
	_SendEvent(pRole);
}


void EventMgr::AddOneEvent(UINT64 nRoleID,GlobalEvent::stEvent *pEvent)
{
	LogInfo("EventMgr::AddOneEvent====nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu, ntime=%u,op=%u,name=%s",
		nRoleID,pEvent->nID, pEvent->nParam1,pEvent->nParam2,pEvent->nParam3,pEvent->lParam1,pEvent->lParam2, pEvent->nTime,pEvent->op,pEvent->szName.c_str());
	auto& ref = mapEvent[nRoleID];
	ref.bUpdate = true;
	UINT64 nUUID = NewEventID(pEvent->nID);
	ref.mapAllEvent.insert(std::pair<UINT64, GlobalEvent::stEvent>(nUUID, *pEvent));
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(nRoleID);
	SendOneEvent(pRole, nUUID, pEvent);
}

void EventMgr::SendOneEvent(CRole* pRole,UINT64 nUUID,GlobalEvent::stEvent *pEvent)
{
	if (!pRole)
	{
		return;
	}
	UINT32 nLine = pRole->GetGsLine();
	if (nLine==INVALID_LINE_ID||pRole->IsInCrossGs())
	{
		return;
	}
	RpcM2G_DesignationEventReq* pRpc = RpcM2G_DesignationEventReq::CreateRpc();
	pRpc->m_oArg.set_roleid(pRole->GetID());
	KKSG::DesignationEvent *pMsgEvent=pRpc->m_oArg.add_event();
	pMsgEvent->set_nuuid(nUUID);
	pMsgEvent->set_neventid(pEvent->nID);
	pMsgEvent->set_nparam1(pEvent->nParam1);
	pMsgEvent->set_nparam2(pEvent->nParam2);
	pMsgEvent->set_nparam3(pEvent->nParam3);
	pMsgEvent->set_lparam1(pEvent->lParam1);
	pMsgEvent->set_lparam2(pEvent->lParam2);
	pMsgEvent->set_ntime(pEvent->nTime);
	pMsgEvent->set_name(pEvent->szName);
	pMsgEvent->set_nop(pEvent->op);
	LogDebug("EventMgr::_SendEvent====nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu op=%u name=%s",
		pRole->GetID(),pEvent->nID, pEvent->nParam1,pEvent->nParam2,pEvent->nParam3,pEvent->lParam1,pEvent->lParam2,pEvent->op,pEvent->szName.c_str());

	pRole->SendMsgToGS(*pRpc);
}

void EventMgr::_SendEvent(CRole* pRole)
{
	if (!pRole)
	{
		return;
	}
	auto find = mapEvent.find(pRole->GetID());
	if (find == mapEvent.end())
	{
		return;
	}

	if (find->second.mapAllEvent.empty())
	{
		return;
	}

	RpcM2G_DesignationEventReq* pRpc = RpcM2G_DesignationEventReq::CreateRpc();
	pRpc->m_oArg.set_roleid(pRole->GetID());
	for(auto it = find->second.mapAllEvent.begin(); it != find->second.mapAllEvent.end(); it++)
	{
		KKSG::DesignationEvent *pMsgEvent=pRpc->m_oArg.add_event();
		pMsgEvent->set_nuuid(it->first);
		pMsgEvent->set_neventid(it->second.nID);
		pMsgEvent->set_nparam1(it->second.nParam1);
		pMsgEvent->set_nparam2(it->second.nParam2);
		pMsgEvent->set_nparam3(it->second.nParam3);
		pMsgEvent->set_lparam1(it->second.lParam1);
		pMsgEvent->set_lparam2(it->second.lParam2);
		pMsgEvent->set_ntime(it->second.nTime);
		pMsgEvent->set_nop(it->second.op);
		pMsgEvent->set_name(it->second.szName);
		LogDebug("EventMgr::_SendEvent====nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu op=%u name=%s",pRole->GetID(),it->second.nID, it->second.nParam1,it->second.nParam2,it->second.nParam3,it->second.lParam1,it->second.lParam2,it->second.op,it->second.szName.c_str());
	}
	pRole->SendMsgToGS(*pRpc);
}

void EventMgr::AddDes(UINT64 nRoleID,UINT32 nDesID)
{
	LogInfo("idip add des= roleid=%llu desid=%u",nRoleID,nDesID);
	AddEvent(nRoleID,DESIGNATION_COM_TYPE_IDIP_ADD,nDesID);
}

void EventMgr::DelDes(UINT64 nRoleID,UINT32 nDesID)
{
	LogInfo("idip del des= roleid=%llu desid=%u",nRoleID,nDesID);
	AddEvent(nRoleID,DESIGNATION_COM_TYPE_IDIP_DEL,nDesID);
}


void EventMgr::AddAch(UINT64 nRoleID,UINT32 nDesID)
{
	LogInfo("idip add ach= roleid=%llu achid=%u",nRoleID,nDesID);
	AddEvent(nRoleID,ACH_EVENT_OFFSET + ACHIEVE_COM_IDIP_ADD,nDesID);
}

void EventMgr::DelAch(UINT64 nRoleID,UINT32 nDesID)
{
	LogInfo("idip del ach= roleid=%llu achid=%u",nRoleID,nDesID);
	AddEvent(nRoleID,ACH_EVENT_OFFSET+ACHIEVE_COM_IDIP_DEL,nDesID);
}

void EventMgr::AddEvent(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1/*=0*/,UINT32 nParam2/*=0*/,UINT32 nParam3/*=0*/,UINT64 lParam1/*=0*/,UINT64 lParam2/*=0*/, const std::string& szName)
{
	GlobalEvent::stEvent msgEvent;
	msgEvent.nID = nEventID;
	msgEvent.nParam1 = nParam1;
	msgEvent.nParam2 = nParam2;
	msgEvent.nParam3 = nParam3;
	msgEvent.lParam1 = lParam1;
	msgEvent.lParam2 = lParam2;
	msgEvent.nTime = GameTime::GetTime();
	msgEvent.op = GlobalEvent::EVENT_OP_ADD;
	msgEvent.szName = szName;
	AddOneEvent(nRoleID, &msgEvent);
}


void EventMgr::AddEvent(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1,UINT32 nOper,std::string szName)
{
	GlobalEvent::stEvent msgEvent;
	msgEvent.nID = nEventID;
	msgEvent.nParam1 = nParam1;
	msgEvent.nTime = GameTime::GetTime();
	msgEvent.op = nOper;
	msgEvent.szName = szName;
	AddOneEvent(nRoleID, &msgEvent);
}

void EventMgr::AddEventOnlyOnMS(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1/*=0*/,UINT32 nParam2/*=0*/,UINT32 nParam3/*=0*/,UINT64 lParam1/*=0*/,UINT64 lParam2/*=0*/)
{
	if (_IsBonusEvent(nEventID))
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(nRoleID);
		if (!pGuild)
		{
			return;
		}
		auto record = pGuild->Get<GuildBonusMgr>();
		UINT32 nID = nEventID - BONUS_EVENT_OFFSET;
		KKSG::GuildBonusType type = KKSG::GBONUS_MAX;
		switch(nID)
		{
		case GUILDBONUS_COM_TYPE_PK_WEEK_RANK:
			type = KKSG::GBONUS_TIANTIRANK;
			break;
		case GUILDBONUS_COM_TYPE_GUILDGMF_RANK:
			type = KKSG::GBONUS_GMF_RANK;
			break;
		case GUILDBONUS_COM_TYPE_SKYARENA_FLOOR:
			type = KKSG::GBONUS_SKYARENA_FLOOR;
			break;
		case GUILDBONUS_COM_TYPE_MAYHEMRANK:
			type = KKSG::GBONUS_MAYHEMRANK;
			break;
		default:
			break;
		}
		if (type != KKSG::GBONUS_MAX)
		{
			record->addGuildBonus(type, nParam1, nRoleID);
		}
	}
	else if (_IsMentorEvent(nEventID))
	{
		UINT32 nID = nEventID - MENTOR_EVENT_OFFSET;
		MentorShipOneSide* oneside = MentorMgr::Instance()->GetMentorShip(nRoleID);
		if (oneside)
		{
			oneside->GetMentorTask()->AddTaskValue(nID, nParam1, nParam2);
		}
	}
	LogInfo("EventMgr::AddEvent====nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu",
		nRoleID,nEventID, nParam1,nParam2,nParam3,lParam1,lParam2);
}

void EventMgr::AddEventFromGS(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1/*=0*/,UINT32 nParam2/*=0*/,UINT32 nParam3/*=0*/,UINT64 lParam1/*=0*/,UINT64 lParam2/*=0*/)
{
	if (_IsBonusEvent(nEventID))
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(nRoleID);
		if (!pGuild)
		{
			return;
		}
		auto record = pGuild->Get<GuildBonusMgr>();
		record->addGuildBonus(nParam1, nRoleID);
	}
	else if (_IsMentorEvent(nEventID))
	{
		UINT32 nID = nEventID - MENTOR_EVENT_OFFSET;
		MentorShipOneSide* oneside = MentorMgr::Instance()->GetMentorShip(nRoleID);
		if (oneside)
		{
			oneside->GetMentorTask()->AddTaskValue(nID, nParam1, nParam2, nParam3);
		}
	}
	else
		AddEvent(nRoleID, nEventID, nParam1,nParam2, nParam3, lParam1, lParam2);

}

void EventMgr::AddGuildEvent(Guild *pGuild,UINT32 nEventID,UINT32 nParam1,UINT32 nParam2,UINT32 nParam3,UINT64 lParam1,UINT64 lParam2)
{
	GlobalEvent::stEvent msgEvent;
	msgEvent.nID = nEventID;
	msgEvent.nParam1 = nParam1;
	msgEvent.nParam2 = nParam2;
	msgEvent.nParam3 = nParam3;
	msgEvent.lParam1 = lParam1;
	msgEvent.lParam2 = lParam2;
	msgEvent.nTime = GameTime::GetTime();
	const std::map<UINT64, GuildMember>& guildMemberVec = pGuild->GetGuildMember();
	for (auto iter = guildMemberVec.begin(); iter!=guildMemberVec.end();iter++)
	{
		AddOneEvent(iter->first, &msgEvent);
	}
}

void EventMgr::TestSave()
{
	_UpdateToDB(true);
}

UINT64 EventMgr::NewEventID(UINT32 nEventID)
{
	if (m_nIndex > m_nMaxIndex)
	{
		m_nIndex = 1;
	}
	UINT32 dwHigh = MakeUINT32(m_nIndex, nEventID);
	m_nIndex++;
	return MakeUINT64(m_nTime, dwHigh);
}

void EventMgr::_UpdateToDB(bool isAll)
{
	int onceSaveNum = 500;
	if (isAll)
	{
		onceSaveNum = mapEvent.size();
	}
	for(auto iter = mapEvent.begin(); iter != mapEvent.end() && onceSaveNum > 0; iter++)
	{
		if(iter->second.bUpdate)
		{
			bool bSave = false;
			if (isAll)
			{
				bSave =  true;
			}else
			{
				for (auto it = iter->second.mapAllEvent.begin(); it != iter->second.mapAllEvent.end(); it++)
				{
					if (m_nTime >= it->second.nTime+300)
					{
						bSave = true;
						break;
					}
				}
			}

			if (bSave)
			{
				PtcM2D_UpdateDesignation ptc;
				ptc.m_Data.set_roleid(iter->first);
				for (auto it = iter->second.mapAllEvent.begin(); it != iter->second.mapAllEvent.end(); it++)
				{
					KKSG::DesignationEvent*pEvent = ptc.m_Data.add_devent();
					pEvent->set_neventid(it->second.nID);
					pEvent->set_nparam1(it->second.nParam1);
					pEvent->set_nparam2(it->second.nParam2);
					pEvent->set_nparam3(it->second.nParam3);
					pEvent->set_lparam1(it->second.lParam1);
					pEvent->set_lparam2(it->second.lParam2);
					pEvent->set_ntime(it->second.nTime);
					pEvent->set_nop(it->second.op);
					pEvent->set_name(it->second.szName);
				}
				DBLink::Instance()->SendTo(ptc);
				mapEvent[iter->first].bUpdate = false;
				--onceSaveNum;
			}
		}
	}

}

void EventMgr::DelEvent(UINT64 nRoleID,UINT64 nEventID)
{
	auto find = mapEvent.find(nRoleID);
	if (find == mapEvent.end())
	{
		return;
	}
	std::map<UINT64, GlobalEvent::stEvent>::iterator iter = find->second.mapAllEvent.find(nEventID);
	if (iter!=find->second.mapAllEvent.end())
	{
		LogInfo("nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu",
			nRoleID,iter->second.nID, iter->second.nParam1,iter->second.nParam2,iter->second.nParam3,iter->second.lParam1,iter->second.lParam2);
		find->second.mapAllEvent.erase(iter);
		find->second.bUpdate = true;
	}
}

void EventMgr::DelEvent(UINT64 nRoleID,std::vector<UINT64>& vecEventIDs)
{
	auto find = mapEvent.find(nRoleID);
	if (find == mapEvent.end())
	{
		return;
	}
	if (find->second.mapAllEvent.empty())
	{
		return;
	}

	for(auto i = vecEventIDs.begin(); i != vecEventIDs.end(); ++i)
	{
		std::map<UINT64, GlobalEvent::stEvent>::iterator iter = find->second.mapAllEvent.find(*i);
		if (iter!=find->second.mapAllEvent.end())
		{
			LogInfo("nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu",
				nRoleID,iter->second.nID, iter->second.nParam1,iter->second.nParam2,iter->second.nParam3,iter->second.lParam1,iter->second.lParam2);
			find->second.mapAllEvent.erase(iter);
			find->second.bUpdate = true;
		}
	}
}

bool EventMgr::_ReadAllRoleEventFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	std::stringstream ss;
	ss<< "select * from designationevent";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table mail failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	while(poRes->FetchRow())
	{
		char* pVal		= NULL;
		UINT32 nLen	= 0;
		UINT64 nRoleID = 0;
		if(poRes->GetFieldValue(0, &pVal, nLen))
		{
			nRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'roleid' column failed");
			return false;
		}

		std::string strData;
		if(poRes->GetFieldValue(1, &pVal, nLen))
		{
			strData.assign(pVal, nLen);
		}
		else
		{
			LogError("Read 'designationevent' column failed");
			return false;
		}

		KKSG::AllDesignationEvent allEvent;
		if(!allEvent.ParseFromString(strData))
		{
			LogError("Parse designationevent failed, roleID %llu", nRoleID);
			return false;
		}

		for (UINT32 i = 0; i < allEvent.devent_size(); i++)
		{
			const KKSG::DesignationEvent *pEvent	= &allEvent.devent(i);
			UINT64 nEventID = NewEventID(pEvent->neventid());
			mapEvent[nRoleID].bUpdate = false;
			mapEvent[nRoleID].mapAllEvent[nEventID].nID = pEvent->neventid();
			mapEvent[nRoleID].mapAllEvent[nEventID].nParam1 = pEvent->nparam1();
			mapEvent[nRoleID].mapAllEvent[nEventID].nParam2 = pEvent->nparam2();
			mapEvent[nRoleID].mapAllEvent[nEventID].nParam3 = pEvent->nparam3();
			mapEvent[nRoleID].mapAllEvent[nEventID].lParam1 = pEvent->lparam1();
			mapEvent[nRoleID].mapAllEvent[nEventID].lParam2 = pEvent->lparam2();
			mapEvent[nRoleID].mapAllEvent[nEventID].nTime = pEvent->ntime();
			mapEvent[nRoleID].mapAllEvent[nEventID].op = pEvent->nop();
			mapEvent[nRoleID].mapAllEvent[nEventID].szName = pEvent->name();

		}
	}
	poRes->Release();
	return true;
}

bool EventMgr::_IsBonusEvent(UINT32 eventID)
{
	if (eventID >= BONUS_EVENT_OFFSET && eventID < MENTOR_EVENT_OFFSET)
		return true;

	return false;
}

bool EventMgr::_IsMentorEvent(UINT32 eventID)
{
	if (eventID >= MENTOR_EVENT_OFFSET && eventID < MIX_EVENT_OFFSET)
		return true;

	return false;
}

UINT32 EventMgr::GetSize()
{
	UINT32 nSize = 0;
	for (auto iter = mapEvent.begin(); iter != mapEvent.end();iter++)
		nSize += iter->second.mapAllEvent.size();
	return nSize;
}

