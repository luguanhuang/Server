#include "pch.h"
#include "guild/rpcg2m_notifycheckinbox.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildcheckin.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/18 13:55:59

RPC_SERVER_IMPLEMETION(RpcG2M_NotifyCheckinBox, NotifyCheckinBoxArg, NotifyCheckinBoxRes)

void RpcG2M_NotifyCheckinBox::OnCall(const NotifyCheckinBoxArg &roArg, NotifyCheckinBoxRes &roRes)
{
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(roArg.roleid());
	if (guild == NULL)
	{
		roRes.set_errorcode(ERR_GUILD_NOT_IN_GUILD);
		return;
	}

	CGuildCheckin* pGuildcheckin = guild->Get<CGuildCheckin>();
	if (pGuildcheckin == NULL)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	pGuildcheckin->Update();

	UINT32 process = pGuildcheckin->GetPorcess();

	std::vector<ItemDesc> items = GuildCheckinMgr::Instance()->GetBoxReward(roArg.index(), process);

	if (items.empty())
	{
		roRes.set_errorcode(ERR_GUILDCHECKIN_BOXLIMIT);
		return;
	}

	foreach (i in items)
	{
		KKSG::ItemBrief* pItem = roRes.add_items();
		pItem->set_itemid(i->itemID);
		pItem->set_itemcount(i->itemCount);
	}

	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcG2M_NotifyCheckinBox::OnDelayReplyRpc(const NotifyCheckinBoxArg &roArg, NotifyCheckinBoxRes &roRes, const CUserData &roUserData)
{
}
