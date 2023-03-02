#include "pch.h"
#include "event/eventmgr.h"
#include "db/mysqlmgr.h"
#include "network/mslink.h"
#include "util.h"
#include "pb/project.pb.h"
#include "timeutil.h"
#include "designation/rpcg2m_gsdesignationeventreq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildconfig.h"
#include "guild/guildrecord.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/designationMgr.h"
#include "network/mslink.h"
#include "config.h"
#include "servermgr/servermgr.h"

INSTANCE_SINGLETON(EventMgr)

EventMgr::EventMgr():m_nIndex(0),m_nMaxIndex((1 << 12) -1){}

EventMgr::~EventMgr(){}

bool EventMgr::Init()
{
	m_nTime = TimeUtil::GetTime() + EVENT_REFRESH_TIME;
	//加载称号、成就表格

	return true;
}

void EventMgr::Uninit()
{
}

void EventMgr::Update()
{
	if(TimeUtil::GetTime() < m_nTime) return;
	m_nTime += EVENT_REFRESH_TIME;
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


void EventMgr::_AddEvent(UINT64 nRoleID,GlobalEvent::stEvent *pEvent)
{
	LogDebug("EventMgr::AddEvent====nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu",
		nRoleID,pEvent->nID, pEvent->nParam1,pEvent->nParam2,pEvent->nParam3,pEvent->lParam1,pEvent->lParam2);
	RpcG2M_GSDesignationEventReq* pRpc		= RpcG2M_GSDesignationEventReq::CreateRpc();
	KKSG::DesignationEvent *pMsgEvent			= pRpc->m_oArg.add_event();
	UINT64 nEventID = NewEventID(pEvent->nID);
	pRpc->m_oArg.set_roleid(nRoleID);
	pMsgEvent->set_nuuid(nEventID);
	pMsgEvent->set_neventid(pEvent->nID);
	pMsgEvent->set_nparam1(pEvent->nParam1);
	pMsgEvent->set_nparam2(pEvent->nParam2);
	pMsgEvent->set_nparam3(pEvent->nParam3);
	pMsgEvent->set_lparam1(pEvent->lParam1);
	pMsgEvent->set_lparam2(pEvent->lParam2);
	pMsgEvent->set_ntime(pEvent->nTime);

	if (GSConfig::Instance()->IsCrossGS()==false)
	{
		MSLink::Instance()->SendTo(*pRpc);
	}else
	{
		UINT32 nSvrID = ServerMgr::Instance()->GetFromServerIdByRoleId(nRoleID);
		ServerMgr::Instance()->SendTo(nSvrID,*pRpc);
	}
	mapEvent[nRoleID].bUpdate = true;
	mapEvent[nRoleID].mapAllEvent.insert(std::pair<UINT64, GlobalEvent::stEvent>(nEventID, *pEvent));

}

void EventMgr::AddEvent(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1/*=0*/,UINT32 nParam2/*=0*/,UINT32 nParam3/*=0*/,UINT64 lParam1/*=0*/,UINT64 lParam2/*=0*/)
{
	GlobalEvent::stEvent msgEvent;
	msgEvent.nID = nEventID;
	msgEvent.nParam1 = nParam1;
	msgEvent.nParam2 = nParam2;
	msgEvent.nParam3 = nParam3;
	msgEvent.lParam1 = lParam1;
	msgEvent.lParam2 = lParam2;
	msgEvent.nTime = TimeUtil::GetTime();
	_AddEvent(nRoleID, &msgEvent);
}

void EventMgr::AddEventOnlyOnGS(Role* role,UINT32 nEventID,UINT32 nParam1/*=0*/,UINT32 nParam2/*=0*/,UINT32 nParam3/*=0*/,UINT64 lParam1/*=0*/,UINT64 lParam2/*=0*/)
{
	if (!role)
	{
		return;
	}
	if (nEventID >= ACH_EVENT_OFFSET && nEventID < BONUS_EVENT_OFFSET)
	{
		UINT32 nAch = nEventID - ACH_EVENT_OFFSET;
		switch(nAch)
		{
		case ACHIEVE_COM_DRAGON_SCENE:
			{
				AchieveV2Mgr::Instance()->OnPassDragonScene(role, nParam1, nParam2, nParam3);
				DesignationMgr::Instance()->OnPassDragonScene(role,nParam1);
			}
			break;
		default:
			break;
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
		LogDebug("nRoleID===%llu, eventid=%d, param1=%d param2=%d param3=%d lparam1=%llu lparam2=%llu",
			nRoleID,iter->second.nID, iter->second.nParam1,iter->second.nParam2,iter->second.nParam3,iter->second.lParam1,iter->second.lParam2);
		find->second.mapAllEvent.erase(iter);
		find->second.bUpdate = true;
	}
	if (find->second.mapAllEvent.empty())
	{
		mapEvent.erase(find);
	}
}

void EventMgr::Clear(UINT64 nRoleID)
{
	auto find = mapEvent.find(nRoleID);
	if (find == mapEvent.end())
	{
		return;
	}
	
	find->second.bUpdate = true;
	find->second.mapAllEvent.clear();
	mapEvent.erase(find);
}

void EventMgr::AddGuildBonusEvent(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1/*=0*/,UINT32 nParam2/*=0*/,UINT32 nParam3/*=0*/,UINT64 lParam1/*=0*/,UINT64 lParam2/*=0*/)
{
	if (nEventID < BONUS_EVENT_OFFSET)
	{
		return;
	}
	Role* role = RoleManager::Instance()->FindByRoleID(nRoleID);
	if (!role)
	{
		return;
	}
	KKSG::GuildBonusType type = KKSG::GBONUS_MAX;
	UINT32 nID = nEventID - BONUS_EVENT_OFFSET;
	switch(nID)
	{
	case GUILDBONUS_COM_TYPE_KILLDRAGON:
		type = KKSG::GBONUS_KILLSTAGEDRAGON;
		break;
	case GUILDBONUS_COM_TYPE_CHARGE:
		type = KKSG::GBONUS_CHARGETIMES;
		break;
	case GUILDBONUS_COM_TYPE_VIPLEVEL:
		type = KKSG::GBONUS_VIPLEVEL;
		break;
	case GUILDBONUS_COM_TYPE_DRAWLOTTERY_TEN:
		type = KKSG::GBONUS_DRAWLOTTERY_TEN;
		break;
	case GUILDBONUS_COM_TYPE_TITLE:
		type = KKSG::GBONUS_TITLE;
		break;
	case GUILDBONUS_COM_TYPE_STRENGTHEN:
		type = KKSG::GBONUS_STRENGTHEN;
		break;
	case GUILDBONUS_COM_TYPE_TOWER:
		type = KKSG::GBONUS_TOWER;
		break;
	case GUILDBONUS_COM_TYPE_PET:
		type = KKSG::GBONUS_HORSE_QUALITY;
		break;
	case GUILDBONUS_COM_TYPE_PRIVILEGE:
		type = KKSG::GBONUS_CHARGEPRIVILEGE;
		break;
	case GUILDBONUS_COM_TYPE_FUND:
		type = KKSG::GBONUS_PURCHASEFUND;
		break;
	case GUILDBONUS_COM_TYPE_ALLDRAGONJADE_LEVEL:
		type = KKSG::GBONUS_DRAGONJADEL_ALLLEVEL;
		break;
	case GUILDBONUS_COM_TYPE_GUILDGMF_RANK:
		type = KKSG::GBONUS_GMF_RANK;
		break;
	case GUILDBONUS_COM_TYPE_SKYARENA_FLOOR:
		type = KKSG::GBONUS_SKYARENA_FLOOR;
		break;
	case GUILDBONUS_COM_TYPE_DRAGON_NEST:
		type = KKSG::GBONUS_DRAGONNEST_SCENE;
		break;
	case GUILDBONUS_COM_TYPE_BOSSRUSH:
		type = KKSG::GBONUS_BOSSRUSH;
		break;
	case GUILDBONUS_COM_TYPE_BUYIBSHOP:
		type = KKSG::GBONUS_BUYIBSHOP;
        break;
	case GUILDBONUS_COM_TYPE_USEITEM:
		type = KKSG::GBONUS_USEITEM;
		break;
	case GUILDBONUS_COM_TYPE_USETHREESUIT:
		type = KKSG::GBONUS_USETHREESUIT;
		break;
	default:
		break;
	}
	if (type == KKSG::GBONUS_MAX)
	{
		return;
	}
	
	std::vector<UINT32> contentTypeVec;
	if (type == KKSG::GBONUS_TOWER)
	{
		std::map<UINT32, UINT32>* temp = GuildConfig::Instance()->GetBonusContent(type);
		if (temp)
		{
			for (auto iter = temp->begin(); iter != temp->end(); ++iter)
			{
				if (nParam1 < iter->first && iter->first <= nParam2)
				{
					contentTypeVec.push_back(iter->second);
				}
			}
		}
	}
	else
	{
		UINT32 templateID = 0;
		templateID = GuildConfig::Instance()->GetBonusTemplateID(type, nParam1);
		if (templateID)
		{
			if (!role->Get<CGuildRecord>()->CheckSendGuildBonus(type, nParam1))
			{
				return;
			}
			contentTypeVec.push_back(templateID);
		}

	}
	if (contentTypeVec.empty())
	{
		return;
	}
	for (auto iter = contentTypeVec.begin(); iter != contentTypeVec.end(); ++iter)
	{
		GlobalEvent::stEvent msgEvent;
		msgEvent.nID = nEventID;
		msgEvent.nParam1 = *iter;
		msgEvent.nParam2 = nParam2;
		msgEvent.nParam3 = nParam3;
		msgEvent.lParam1 = lParam1;
		msgEvent.lParam2 = lParam2;
		msgEvent.nTime = TimeUtil::GetTime();
		_AddEvent(nRoleID, &msgEvent);
	}
}

void EventMgr::OnConnectMS(UINT32 nServerID)
{
	for (auto iter = mapEvent.begin(); iter != mapEvent.end();)
	{
		if (iter->second.mapAllEvent.empty())
		{
			mapEvent.erase(iter++);
			continue;
		}
		RpcG2M_GSDesignationEventReq* pRpc		= RpcG2M_GSDesignationEventReq::CreateRpc();
		pRpc->m_oArg.set_roleid(iter->first);
		for(auto it = iter->second.mapAllEvent.begin(); it != iter->second.mapAllEvent.end(); ++it)
		{
			KKSG::DesignationEvent *pMsgEvent=pRpc->m_oArg.add_event();
			pMsgEvent->set_neventid(it->second.nID);
			pMsgEvent->set_nparam1(it->second.nParam1);
			pMsgEvent->set_nparam2(it->second.nParam2);
			pMsgEvent->set_nparam3(it->second.nParam3);
			pMsgEvent->set_lparam1(it->second.lParam1);
			pMsgEvent->set_lparam2(it->second.lParam2);
			pMsgEvent->set_ntime(it->second.nTime);
		}
		if (GSConfig::Instance()->IsCrossGS())
		{
			ServerMgr::Instance()->SendTo(nServerID,*pRpc);
		}else
		{
			MSLink::Instance()->SendTo(*pRpc);
		}
		++iter;
	}
}
