#include "pch.h"
#include "guild/rpcc2g_replypartyexchangeitemopt.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/ptcg2c_guildcamppartynotify.h"
#include "guild//ptcg2c_guildcamppartytradenotify.h"
#include "guild/guildcampparty.h"
#include "gamelogic/MultActivityMgr.h"

// generate by ProtoGen at date: 2017/5/6 15:40:56

RPC_SERVER_IMPLEMETION(RpcC2G_ReplyPartyExchangeItemOpt, ReplyPartyExchangeItemOptArg, ReplyPartyExchangeItemOptRes)

	void RpcC2G_ReplyPartyExchangeItemOpt::OnCall(const ReplyPartyExchangeItemOptArg &roArg, ReplyPartyExchangeItemOptRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
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
	KKSG::ErrorCode result = KKSG::ERR_SUCCESS;
	roRes.set_result(result);

	bool canTrade = false;
	switch(roArg.operate_type())
	{
	case 1:   // 拒绝
		{
			CGuildCampPartyMgr::Instance()->ClearTradeInvitation(role->GetRoleID(),roArg.lauch_role_id(),false);		
			break;
		}
	case 2:  // 清空
		{
			CGuildCampPartyMgr::Instance()->ClearTradeInvitation(role->GetRoleID(),roArg.lauch_role_id(),true);	
			break;
		}
	case 3: // 接受
		{			

			KKSG::ErrorCode result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(role);
			if ( result != KKSG::ERR_SUCCESS)
			{	
				roRes.set_result(result);
				if (result == KKSG::ERR_INVFIGHT_OTHER_SCENE)
				{
					roRes.set_result(KKSG::ERR_GUILDCAMP_SELF_NOTIN);
				}		
				return ;
			}

			// 2.检查双方是否已经在交易中
			if (CGuildCampPartyMgr::Instance()->CheckRoleTransactionStatus(role->GetRoleID()))
			{
				// 错误码
				roRes.set_result(KKSG::ERR_GUILDPARTY_TRADIING);				
				return;
			}
			if (CGuildCampPartyMgr::Instance()->CheckRoleTransactionStatus(roArg.lauch_role_id()))
			{
				// 错误码	
				roRes.set_result(KKSG::ERR_GUILDPARTY_TRADIING);				
				return;
			}

			canTrade = true;
			break;
		}
	default:
		break;
	}

	if (!canTrade)
	{
		UINT32 invation =   CGuildCampPartyMgr::Instance()->GetTardeInvitationCount(role->GetRoleID());
		if (invation < 1)
		{
			// 通知target
			PtcG2C_GuildCampPartyTradeNotify ntf;	
			ntf.m_Data.set_notify_type(KKSG::TRADE_INVITATION);
			ntf.m_Data.set_lauch_count(0);
			role->Send(ntf);
		}
		return;
	}

	Role *lauch_role =  RoleManager::Instance()->FindByRoleID(roArg.lauch_role_id());
	if (NULL == lauch_role)
	{
		roRes.set_result(KKSG::ERR_ROLE_NOT_ONLINE);
		// 对方已经下线 ，清理掉记录
		CGuildCampPartyMgr::Instance()->ClearTradeInvitation(role->GetRoleID(),roArg.lauch_role_id(),false);
		return;
	}

	result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(lauch_role);
	if (result != KKSG::ERR_SUCCESS)
	{	
		roRes.set_result(result);		
		// 这一笔作废
		CGuildCampPartyMgr::Instance()->ClearTradeInvitation(role->GetRoleID(),roArg.lauch_role_id(),false);	
		
		return ;
	}

	CGuildCampPartyMgr::Instance()->SetRoleInTransactionStatus(role->GetRoleID());
	CGuildCampPartyMgr::Instance()->SetRoleInTransactionStatus(roArg.lauch_role_id());
	// 只需要保留彼此的一条记录
	CGuildCampPartyMgr::Instance()->MakeTradeRecord(lauch_role,role);	

	// 通知双方，开始交易
	PtcG2C_GuildCampPartyTradeNotify ntf;
	ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);	
	ntf.m_Data.set_open_trade(true);
	ntf.m_Data.set_lauch_role_id(roArg.lauch_role_id());
	ntf.m_Data.set_target_role_id(role->GetRoleID());	

	ntf.m_Data.set_lauch_name(lauch_role->GetName());
	ntf.m_Data.set_lauch_profession(lauch_role->GetProfession());
	ntf.m_Data.set_target_name(role->GetName());
	ntf.m_Data.set_target_profession(role->GetProfession());

	lauch_role->Send(ntf);
	role->Send(ntf);	
}

void RpcC2G_ReplyPartyExchangeItemOpt::OnDelayReplyRpc(const ReplyPartyExchangeItemOptArg &roArg, ReplyPartyExchangeItemOptRes &roRes, const CUserData &roUserData)
{
}
