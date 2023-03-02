#include "pch.h"
#include "task/rpcg2m_taskdonateg2m.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildarena.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "foreach.h"
#include "guild/guilddonate.h"
#include "util/gametime.h"
#include "define/taskdef.h"

// generate by ProtoGen at date: 2017/9/8 16:05:38

RPC_SERVER_IMPLEMETION(RpcG2M_TaskDonateG2M, TaskDonateG2MArg, TaskDonateG2MRes)

void RpcG2M_TaskDonateG2M::OnCall(const TaskDonateG2MArg &roArg, TaskDonateG2MRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}
	GuildDonate* donate = pGuild->Get<GuildDonate>();

	if (roArg.type() == TaskDonateOper_Add)
	{
		MemberAskInfo info;
		info.tasktype = roArg.tasktype();
		info.roleId = pRole->GetID();
		info.publishTime = GameTime::GetTime();
		info.itemId = roArg.itemid();
		info.needCount = roArg.itemcount();
		info.level = roArg.level();
		info.quality = roArg.quality();
		info.taskId = roArg.taskid();
		info.itemType = roArg.itemtype();
		info.itemQuality = roArg.itemquality();
		info.index = roArg.index();
		UINT32 uid = donate->AddAskItemInfo(info);
		roRes.set_result(KKSG::ERR_SUCCESS);
		roRes.set_ask_uid(uid);
	}
	else if (roArg.type() == TaskDonateOper_Update)
	{
		MemberAskInfo* pInfo = donate->GetAskInfo(roArg.roleid(), roArg.index());
		if (pInfo)
		{
			pInfo->needCount = roArg.itemcount();
		}
	}
	else if (roArg.type() == TaskDonateOper_Remove)
	{
		donate->RemoveAskInfo(roArg.roleid(), roArg.taskid());
	}
	else if (roArg.type() == TaskDonateOper_RemoveAllWeek)
	{
		donate->RemoveWeekAskInfoByRoleId(roArg.roleid());
	}
}

void RpcG2M_TaskDonateG2M::OnDelayReplyRpc(const TaskDonateG2MArg &roArg, TaskDonateG2MRes &roRes, const CUserData &roUserData)
{
}
