#include "pch.h"
#include "qa/rpcc2g_getguildqadatareq.h"
#include "unit/rolemanager.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/QARecord.h"
#include "timeutil.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "gamelogic/MultActivityMgr.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/7/9 14:33:17


RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildQADataReq, GetGuildQADataReq, GetGuildQADataRes)

void RpcC2G_GetGuildQADataReq::OnCall(const GetGuildQADataReq &roArg, GetGuildQADataRes &roRes)
{
	roRes.set_result(ERR_UNKNOWN);
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	CGuildSimple* pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(role->getGuildId());
	if(!pGuild)
	{
		roRes.set_result(KKSG::ERR_QA_NO_GUILD);
		SSWarn << "Player Not Join Guild, RoleID:" << role->GetID() << END;
		return;
	}

	KKSG::ErrorCode code = ERR_SUCCESS;
	UINT32 status = 0; 
	UINT32 lefttime = 0; 
	do
	{
		if (!MultActivityMgr::Instance()->IsActivityOnDay(MULT_ACTIVITY_GUILD_QA, XCommon::GetWeekNum(GameTime::GetTime())))
		{
			status = KKSG::NOT_OPEN_DAY;
			goto label;
		}
		MultActivityTime openTime;
		KKSG::MulActivityTimeState eState = MultActivityMgr::Instance()->GetOpenTimeState(MULT_ACTIVITY_GUILD_QA, lefttime, openTime);
		if (eState == KKSG::MULACTIVITY_BEfOREOPEN)
		{
			status = KKSG::BEFORE_OPEN;
		}
		else if (eState == KKSG::MULACTIVITY_RUNNING)
		{
			bool isopen = QAMgr::Instance()->HasGuildRoom(pGuild->GetGuildId());
			bool rolePlaying = role->Get<QARecord>()->IsPlayingQA();
			bool canPlay = QAMgr::Instance()->CanRoleEnter(role, QATYPE_GUILD) == KKSG::ERR_SUCCESS ? true : false;
			if (rolePlaying)
			{
				status = KKSG::IN_TIME_OPENING;
			}
			else if (!canPlay)
			{
				status = KKSG::AFTER_OPEN;
			}
			else if (isopen)
			{
				status = KKSG::IN_TIME_OPENING;
			}
			else
			{
				UINT32 todayZero = (UINT32)XCommon::GetTodayBeginTime(false);
				UINT32 lastOpenTime = QAMgr::Instance()->GetGuildLastOpenTime(pGuild->GetGuildId());
				UINT32 startTime = todayZero + openTime.startSec;
				// 已经开启过了
				if (lastOpenTime > startTime)
				{
					status =KKSG::AFTER_OPEN;
				}
				else
				{
					status = KKSG::IN_TIME_NOT_OPEN;
				}
			}
		}
		// 结束时间内，房间可能还没结束
		else if (eState == KKSG::MULACTIVITY_END || eState == KKSG::MULACTIVITY_UNOPEN_TODAY)
		{
			bool isopen = QAMgr::Instance()->HasGuildRoom(pGuild->GetGuildId());
			bool rolePlaying = role->Get<QARecord>()->IsPlayingQA();
			bool canPlay = QAMgr::Instance()->CanRoleEnter(role, QATYPE_GUILD) == KKSG::ERR_SUCCESS ? true : false;
			if (rolePlaying)
			{
				status = KKSG::IN_TIME_OPENING;
			}
			else if (isopen && canPlay)
			{
				status = KKSG::IN_TIME_OPENING;
			}
			else 
			{
				status = KKSG::AFTER_OPEN;
			}
		}
	} while (0);
label:
	roRes.set_time(lefttime);
	roRes.set_status(status);
	roRes.set_result(code);
	SSInfo << "Finish Send G->C Message, CMD=RpcC2G_GetGuildQADataRes, RoleID:" << role->GetID() << ", Status:" << roRes.status() << ", LeftTime:" << roRes.time() << ", ErrorCode:" << roRes.result() << END;
}

void RpcC2G_GetGuildQADataReq::OnDelayReplyRpc(const GetGuildQADataReq &roArg, GetGuildQADataRes &roRes, const CUserData &roUserData)
{
}
