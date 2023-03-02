#include "pch.h"
#include "task/rpcc2m_donatememberitem.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildactivity.h"
#include "guild/guilddonate.h"
#include "task/rpcm2g_costdonateitem.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"
#include "table/ItemConfigMgr.h"
#include "task/roletaskmgr.h"

// generate by ProtoGen at date: 2016/11/16 14:51:34

RPC_SERVER_IMPLEMETION(RpcC2M_DonateMemberItem, DonateMemberItemArg, DonateMemberItemRes)

void RpcC2M_DonateMemberItem::OnCall(const DonateMemberItemArg &roArg, DonateMemberItemRes &roRes)
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
	MemberAskInfo* info = donate->GetAskInfo(roArg.id());
	if (!info)
	{
		roRes.set_result(KKSG::ERR_TASK_ASKITEM_REFRESH);
		return;
	}
	if (info->roleId == pRole->GetID())
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (info->getCount >= info->needCount)
	{
		roRes.set_result(KKSG::ERR_TASK_ASKITEM_REFRESH);
		return;	
	}
	if (info->donating)
	{
		roRes.set_result(KKSG::ERR_TASK_ASKITEM_REFRESH);
		return;
	}
	UINT32 donateNum = donate->GetTodayDonateNum(pRole->GetID());
	if (roArg.count() + donateNum > GetGlobalConfig().DailyTaskDonateNum)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	// 日常任务暂时只有itemid
	if (roArg.count())
	{
		if (roArg.count() > (info->needCount - info->getCount))
		{
			roRes.set_result(KKSG::ERR_TASK_ASKITEM_REFRESH);
			return;
		}
	}
	else if (roArg.itemuid_size())
	{
		if (roArg.itemuid_size() > (info->needCount - info->getCount))
		{
			roRes.set_result(KKSG::ERR_TASK_ASKITEM_REFRESH);
			return;
		}
	}
	else
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
	}
	info->donating = true;

	RpcM2G_CostDonateItem* rpc = RpcM2G_CostDonateItem::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_tasktype(info->tasktype);
	rpc->m_oArg.set_itemid(info->itemId);
	rpc->m_oArg.set_itemcount(roArg.count());
	rpc->m_oArg.mutable_itemuid()->CopyFrom(roArg.itemuid());
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_taskid(info->taskId);
	rpc->m_oArg.set_level(info->level);
	rpc->m_oArg.set_quality(info->quality);
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_DonateMemberItem::OnDelayReplyRpc(const DonateMemberItemArg &roArg, DonateMemberItemRes &roRes, const CUserData &roUserData)
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
	MemberAskInfo* info = donate->GetAskInfo(roArg.id());
	if (!info)
	{
		roRes.set_result(KKSG::ERR_TASK_NO_ASKINFO);
		return;
	}
	info->donating = false;

	roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS || roUserData.m_pUserPtr == NULL)
	{
		return;
	}
	KKSG::CostDonateItemRes* res = (KKSG::CostDonateItemRes*)roUserData.m_pUserPtr;

	UINT32 num = roArg.count() ? roArg.count() : roArg.itemuid_size();
	info->getCount += num;

	if (info->tasktype == KKSG::PeriodTaskType_Daily)
	{
		// 发邮件
		std::string mailtitle = "null";
		std::string mailcontent;
		const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_Guild_Donate);
		if(pMailConf)
		{
			mailtitle = pMailConf->m_title;
			mailcontent = pMailConf->m_content;
			XCommon::ReplaceOneStr(mailcontent, "%s", pRole->GetName());
			XCommon::ReplaceOneStr(mailcontent, "%s", CItemConfig::Instance()->GetItemName(info->itemId));
		}
		std::vector<ItemDesc> rewards;
		rewards.push_back(ItemDesc(info->itemId, roArg.count(), true));
		MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
		maildata.SetReason(ItemFlow_DonateMemberItem);
		CMailMgr::Instance()->SendMail(info->roleId, maildata);
	}
	else if (info->tasktype == KKSG::PeriodTaskType_Weekly)
	{
		RoleTaskMgr::Instance()->AddWeeklyTaskStepById(info->roleId, info->taskId, num);
		for (int i = 0; i < res->itemid_size(); ++i)
		{
			UINT32 itemID = res->itemid(i);
			UINT32 itemCount = res->itemcount(i);
			RoleTaskMgr::Instance()->AddWeeklyTaskHelpInfo(info->roleId, pRole->GetID(), pRole->GetName(), itemID, itemCount);
		}
	}

	// 捐献排名
	donate->UpdateDonateInfo(pRole->GetID(), res->todaydonatecount(), res->totaldonatecount());
	roRes.set_dailycount(res->todaydonatecount());
	roRes.set_totalcount(res->totaldonatecount());
	roRes.set_getcount(info->getCount);

	// 更新排名
	std::map<UINT64, MemberDonateInfo>& donateInfos = donate->GetAllDonateRecord();
	for (auto it = donateInfos.begin(); it != donateInfos.end(); ++it)
	{
		MemberDonateInfo& info = it->second;
		KKSG::GuildMemberDonateRankItem* data = roRes.add_rankitem();
		donate->FillMemberDonateInfo(info, *data);
	}

	// 捐献满的删掉
	if (info->getCount >= info->needCount)
	{
		donate->RemoveAskInfo(roArg.id());
	}
	donate->SetDirty();
}
