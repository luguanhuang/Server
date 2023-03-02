#include "pch.h"
#include "guild/rpcc2g_guildcampexchangeoperate.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildcampparty.h"
#include "gamelogic/MultActivityMgr.h"
#include "guild/ptcg2c_guildcamppartytradenotify.h"

// generate by ProtoGen at date: 2017/5/2 21:51:43

RPC_SERVER_IMPLEMETION(RpcC2G_GuildCampExchangeOperate, GuildCampExchangeOperateArg, GuildCampExchangeOperateRes)

	void RpcC2G_GuildCampExchangeOperate::OnCall(const GuildCampExchangeOperateArg &roArg, GuildCampExchangeOperateRes &roRes)
{	
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role )
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;	
		return;
	}
	/*if(!MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_GUILDCAMP_PARTY))
	{
	roRes.set_result(KKSG::ERR_GUILDACMPPATY_NOTOPEN);
	return;
	}*/

	TradeData * data = CGuildCampPartyMgr::Instance()->GetTradeDataByRole(role->GetRoleID());
	if (data == NULL)
	{
		// 没有交易 关闭交易界面
		PtcG2C_GuildCampPartyTradeNotify ntf;
		ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
		ntf.m_Data.set_open_trade(false);
		role->Send(ntf);
		
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	Role *launch_role = RoleManager::Instance()->FindByRoleID(data->lauch_role_id);
	Role *target_role = RoleManager::Instance()->FindByRoleID(data->target_role_id);
	if (NULL == launch_role || NULL == target_role)
	{
		// 有人不在线，没法继续
		roRes.set_result(KKSG::ERR_GUILDCAMP_OTHER_NOTIN);

		// 清理这笔交易
		CGuildCampPartyMgr::Instance()->DeleteTrade(data->lauch_role_id);

		// 通知在线的，关闭交易界面
		PtcG2C_GuildCampPartyTradeNotify ntf;
		ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
		ntf.m_Data.set_open_trade(false);
		if (NULL != launch_role)
		{
			launch_role->Send(ntf);
		}
		if (NULL != target_role)
		{
			target_role->Send(ntf);
		}

		return;
	}
	KKSG::ErrorCode result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(target_role);
	if ( result != KKSG::ERR_SUCCESS)
	{	
		CGuildCampPartyMgr::Instance()->CancleTrade(role,launch_role,target_role);
		roRes.set_result(result);
		return ;
	}
	result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(launch_role);
	if (result != KKSG::ERR_SUCCESS)
	{	
		CGuildCampPartyMgr::Instance()->CancleTrade(role,launch_role,target_role);
		roRes.set_result(result);
		return ;
	}	

	if (launch_role->getGuildId() != target_role->getGuildId())
	{
		SSWarn<<"not the sanme guild"<<END;
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);		
		return;
	}

	switch (roArg.operate_type())
	{
	case KKSG::SWINGUPITEM :
		{
			// 上架物品的时候，如果对方是确认状态，需要修改为非确认，不然容易被对方撤销物品
			if (data->lauch_role_id == role->GetRoleID())
			{
				data->target_status = false;
			}
			else
			{
				data->lauch_status = false;
			}
			// 上架物品	
			UINT32 item_id = CGuildCampPartyMgr::Instance()->CheckSwingUpItem(role,roArg.item_uid()) ;
			if (item_id > 0)
			{
				CGuildCampPartyMgr::Instance()->SwingUpItem(role,roArg.item_uid(),item_id,launch_role,target_role);
			}
			else
			{
				roRes.set_result(KKSG::ERR_ITEM_NOTEXIST);
			}
			break;
		}
	case KKSG::SWINGDOWNITEM :
		{
			//下架物品			
			CGuildCampPartyMgr::Instance()->SwingUpItem(role,0,0,launch_role,target_role);
			break;
		}
	case KKSG::CONFIRM :
		{
			// 确认交易
			CGuildCampPartyMgr::Instance()->MakeSureTrade(role,roArg.confirm(),launch_role,target_role);
			break;
		}
	case KKSG::CANCEL :
		{
			CGuildCampPartyMgr::Instance()->CancleTrade(role,launch_role,target_role);
			break;
		}
	case KKSG::AUDIOCHAT :
		{
			CGuildCampPartyMgr::Instance()->AudioChat(role,roArg.audio_id(),roArg.audio_time(),roArg.chat_text(),launch_role,target_role);
			break;
		}
	case KKSG::TEXTCHAT:
		{
			CGuildCampPartyMgr::Instance()->TextChat(role,roArg.chat_text(),launch_role,target_role);			
			break;
		}
	default:
		break;
	}

	CGuildCampPartyMgr::Instance()->NoticeTradeInvitationDisappear(launch_role);
	CGuildCampPartyMgr::Instance()->NoticeTradeInvitationDisappear(target_role);	
}

void RpcC2G_GuildCampExchangeOperate::OnDelayReplyRpc(const GuildCampExchangeOperateArg &roArg, GuildCampExchangeOperateRes &roRes, const CUserData &roUserData)
{
}
