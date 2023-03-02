#include "pch.h"
#include "chatmgr.h"
#include "chathandler.h"
#include "chatcondition.h"
#include "unit/role.h"
#include "tsssdk/TssHandler.h"
#include "timeutil.h"
#include "chat/ptcg2c_worldchannellefttimesntf.h"
#include "chat/ptcg2m_gmsendchat.h"
#include "unit/rolemanager.h"
#include "chatrecord.h"
#include "payv2Record.h"
#include "table/PayTable.h"
#include "table/VIPTable.h"
#include "payv2Mgr.h"
#include "bag.h"
#include "bagtransition.h"
#include "define/tlogenum.h"
#include "network/mslink.h"
#include "rolesummarymgr.h"
#include "rolesummary.h"

INSTANCE_SINGLETON(ChatMgr);

//检测跨天的时间间隔;
const UINT32 checkChatTimesInterval = 2; 

ChatMgr::ChatMgr()
{
	m_dwLastUpTime = 0; 
}

ChatMgr::~ChatMgr()
{

}

bool ChatMgr::Init()
{
	m_dwLastUpTime = 0; 
	if (!m_table.LoadFile("table/Chat.txt"))
	{
		SSWarn<<"load file table/Chat.txt failed"<<END;
		return false;
	}
	/*
	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		UINT32 channel = (*i)->id;
		switch(channel)
		{
		case KKSG::WorldChannel:
			{
				ChatHandler* handler = (ChatHandler*)new ChatWorldHandler(*i);
				ParseCondition(KKSG::WorldChannel, *i, handler);
				m_handles[channel] = handler;
				break;
			}
		case KKSG::GuildChannel:
			{
				ChatHandler* handler = (ChatHandler*)new ChatGuildHandler(*i);
				ParseCondition(KKSG::GuildChannel, *i, handler);
				m_handles[channel] = handler;
				break;
			}
		case KKSG::PrivateChannel:
			{
				ChatHandler* handler = (ChatHandler*)new ChatPrivateHandler(*i);
				ParseCondition(KKSG::PrivateChannel, *i, handler);
				m_handles[channel] = handler;
				break;
			}
		case KKSG::TeamChannel:
			{
				ChatHandler* handler = (ChatHandler*)new ChatTeamHandler(*i);
				ParseCondition(KKSG::TeamChannel, *i, handler);
				m_handles[channel] = handler;
				break;
			}
		default:
			break;
		}
	}*/
	return true;
}

void ChatMgr::Uninit()
{

}

//检查条件;
UINT32 ChatMgr::CheckCondition(Role* role, UINT32 channel)
{
	auto i = m_handles.find( channel );
	if( i == m_handles.end() )
	{
		return KKSG::ERR_UNKNOWN;
	}
	ChatHandler* handler = i->second;

	ChatConditionParam param;
	param.level = role->GetLevel();
	param.roleid = role->GetID();
	param.length = 0; 
	KKSG::ChatInfo chatinfo;
	UINT32 errorcode = handler->CheckChatInfo(param);
	return errorcode;
}

/*UINT32 ChatMgr::HandleChatInfo(Role* role, UINT32 channel, const KKSG::ChatInfo* chatinfo, KKSG::ChatRes* res 
	, const std::vector<UINT64>* destroleids)
{
	SSInfo << "Enter HandleChatInfo, RoleID:" << role->GetID() << END;
	if (NULL == role || NULL == chatinfo)
	{
		return KKSG::ERR_UNKNOWN;	
	}
	auto i = m_handles.find(channel);
	if (i == m_handles.end())
	{
		SSWarn<<"channel:"<<channel<<" is not registered"<<END;
		return KKSG::ERR_UNKNOWN;	
	}
	ChatHandler* handler = i->second;

	ChatConditionParam param;
	param.level = role->GetLevel();
	param.roleid = role->GetID();
	param.length = chatinfo->info().size();
	///>如果是语音，则不限制长度
	if( chatinfo->audiolen() > 0 )
	{
		param.length = 0; 
	}
	if (channel == KKSG::PrivateChannel && destroleids->size() == 1)
	{
		param.destid = (*destroleids)[0];
	}
	UINT32 errorcode = handler->CheckChatInfo(param, *chatinfo);
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		if (0 != param.cooldown)
		{
			res->set_cooldown(param.cooldown);
		}
		res->set_errorcode(KKSG::ErrorCode(errorcode));
		return errorcode;
	}
	handler->PackChatInfo(role, channel, *chatinfo, destroleids);

	bool bFlag = true;
	if (TssHandler::Instance()->GetBusiInterf() != NULL && TssHandler::Instance()->GetSensitiveInterf() != NULL)
	{
		switch (channel)
		{
		case KKSG::WorldChannel:
		case KKSG::GuildChannel:
		case KKSG::TeamChannel:
		case KKSG::PrivateChannel:
			{
				bFlag = false;
				break;
			}
		default:
			break;
		}
	}
	//如果安全sdk不在了，直接用自己的发送;系统消息也自己发送
	if (bFlag || (chatinfo->has_issystem() && chatinfo->issystem()))
	{
		handler->FowardChatInfo(role, destroleids);
		SSInfo << "FlagTRUE ForwardChatInfo, channel:" << channel << ", RoleID:" << role->GetID() << END;
	}
	
	//handler->FowardChatInfo(role, destroleids);
	//SSInfo << "Normal ForwardChatInfo, channel:" << channel << ", RoleID:" << role->GetID() << END;
	
	res->set_errorcode(KKSG::ErrorCode(errorcode));
	return KKSG::ERR_SUCCESS;
}*/
		
void ChatMgr::ParseCondition(UINT32 channel, ChatTable::RowData* rowdata, ChatHandler* handler)
{
	if (0 != rowdata->level)
	{
		LevelChatCondition* condition = new LevelChatCondition(rowdata->level);
		handler->PushCondition(condition);
	}
	if (0 != rowdata->length)
	{
		LengthChatCondition* condition = new LengthChatCondition(rowdata->length);
		handler->PushCondition(condition);
	}
	if (0 != rowdata->cd)
	{
		TimeChatCondition* condition = new TimeChatCondition(rowdata->cd);
		handler->PushCondition(condition);
	}
	if (channel == KKSG::PrivateChannel)
	{
		BlackChatCondition* condition = new BlackChatCondition();
		handler->PushCondition(condition);
	}
}

void ChatMgr::SendWorldChannelLeftTimesNotify( Role* pRole )
{
	if( NULL == pRole )
	{
		SSError << "Role NULL" << END;
		return; 
	}

	ChatRecord* chatrecord = pRole->Get<ChatRecord>();
	if( NULL == chatrecord )
	{
		SSError << "Chat Record NULL, RoleID:" << pRole->GetID() << END;
		return; 
	}

	CPayV2Record* record = pRole->Get<CPayV2Record>();
	if( NULL == record )
	{
		SSError << "Pay Record NULL, RoleID:" << pRole->GetID() << END;
		return; 
	}

	VIPTable::RowData* pData = PayV2Mgr::Instance()->GetVipConf( record->GetVipLevel() );
	if( NULL == pData )
	{
		SSError << "Vip Config Not Find, RoleID:" << pRole->GetID() << ", VipLevel:" << record->GetVipLevel() << END;
		return; 
	}

	UINT32 leftTimes = 0; 
	if( pData->WorldChannelChatTimes > chatrecord->m_dwWorldChatTimes )
	{
		leftTimes = pData->WorldChannelChatTimes - chatrecord->m_dwWorldChatTimes;
	}

	PtcG2C_WorldChannelLeftTimesNtf ntf;
	ntf.m_Data.set_lefttimes( leftTimes );
	pRole->Send( ntf );
	///>SSDebug << "Finish Send G->C Message, CMD=PtcG2C_WorldChannelLeftTimesNtf, RoleID:" << pRole->GetID() << ", LeftTimes:" << ntf.m_Data.lefttimes() << END;
}

///> 聊天是否合法
KKSG::ErrorCode ChatMgr::ChatLegal(Role* pRole, const KKSG::ChatInfo& chatinfo, KKSG::ChatCanSendRes& res)
{
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	ChatTable::RowData* pData = m_table.GetByid(chatinfo.channel());
	if (pData == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	ChatRecord* record = pRole->Get<ChatRecord>();
	if ((INT32)pData->level > pRole->GetLevel())
	{
		return KKSG::ERR_CHAT_LEVELLIMIT;
	}
	UINT32 nowTime = TimeUtil::GetTime();
	UINT32 lastChatTime = record->GetLastChatTime(chatinfo.channel());
	if (pData->cd + lastChatTime > nowTime)
	{
		UINT32 leftTime = pData->cd + lastChatTime - nowTime;
		res.set_cooldown(leftTime);
		return KKSG::ERR_CHAT_TIMELIMIT;
	}

	UINT32 infoLen = XCommon::CountUTF8Char(chatinfo.info());
	if (infoLen > pData->length)
	{
		return KKSG::ERR_CHAT_LENGTHLIMIT;
	}
	UINT32 worldLeftTimes = GetLeftWorldChatTimes(pRole);
	if (KKSG::WorldChannel == chatinfo.channel() && 0 == worldLeftTimes && 0 == pRole->Get<CPayV2Record>()->GetRemainChatCount())
	{
		///> 蓝鸟是否足够
		bool isEnough = true;
		BagTakeItemTransition oTakeTransition(pRole);
		oTakeTransition.SetReason(ItemFlow_Chat, ItemFlow_Chat_World);
		if(!oTakeTransition.TakeItem(BLUE_BIRD, 1))
		{
			oTakeTransition.RollBack();
			isEnough = false;
		}
		else
		{
			oTakeTransition.NotifyClient();
		}

		if(!isEnough)
		{
			///> 蓝鸟不够用金币
			BagTakeItemTransition oTakeTransition(pRole);
			oTakeTransition.SetReason(ItemFlow_Chat, ItemFlow_Chat_World);
			if(!oTakeTransition.TakeItem(DRAGON_COIN, GetGlobalConfig().blueBirdPrice))
			{
				oTakeTransition.RollBack();
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
			oTakeTransition.NotifyClient();
		}
	}
	///> 设置lastChatTime
	record->SetLastChatTime(chatinfo.channel());

	UINT32 uChatID = pRole->GetChatID();
	pRole->SetChatInfo(chatinfo);
	TssHandler::Instance()->judge_user_input_chat( pRole, CONTENT_CATEGORY_CHAT, chatinfo.channel(), 1, chatinfo.info().c_str(), uChatID);

	return KKSG::ERR_SUCCESS;
}

///>获取剩余世界聊天次数
UINT32 ChatMgr::GetLeftWorldChatTimes( Role* pRole )
{
	if( NULL == pRole )
	{
		return 0; 
	}
	ChatRecord* record = pRole->Get<ChatRecord>();
	CPayV2Record* payrecord = pRole->Get<CPayV2Record>();
	VIPTable::RowData* pVipData = PayV2Mgr::Instance()->GetVipConf( payrecord->GetVipLevel() );
	if( NULL == pVipData )
	{
		SSError << "Get VIP Config Failed. RoleID:" << pRole->GetID() << ", VIPLevel:" << payrecord->GetVipLevel() << END;
		return 0; 
	}
	UINT32 leftTimes = 0; 
	if( record->m_dwWorldChatTimes < pVipData->WorldChannelChatTimes)
	{
		leftTimes = pVipData->WorldChannelChatTimes - record->m_dwWorldChatTimes;
	}
	return leftTimes;
}

void ChatMgr::FillSourceData(KKSG::ChatSource* source)
{
	if (source == NULL) return;
	RoleSummary* pRoleSummary = RoleSummaryMgr::Instance()->GetRoleSummary(source->roleid());
	if (pRoleSummary == NULL)
	{
		return;
	}
	const KKSG::RoleSummaryStored& roleData = pRoleSummary->GetData();
	source->set_profession(roleData.profession());
	source->set_name(roleData.name());
	source->set_viplevel(roleData.viplevel());
	source->set_powerpoint(roleData.powerpoint());
	source->set_coverdesignationid(roleData.desigation());
	source->set_paymemberid(pRoleSummary->GetPayMemberID());
	source->set_military_rank(pRoleSummary->GetMilitaryRank());
	for (UINT32 i = 0; i < roleData.pre().setid_size(); i ++)
	{
		source->mutable_pre()->add_setid(roleData.pre().setid(i));
	}
}