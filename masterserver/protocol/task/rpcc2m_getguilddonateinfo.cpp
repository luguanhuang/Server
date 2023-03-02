#include "pch.h"
#include "task/rpcc2m_getguilddonateinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildactivity.h"
#include "guild/guilddonate.h"

// generate by ProtoGen at date: 2016/11/16 14:31:01

RPC_SERVER_IMPLEMETION(RpcC2M_GetGuildDonateInfo, GetGuildDonateInfoArg, GetGuildDonateInfoRes)


void RpcC2M_GetGuildDonateInfo::OnCall(const GetGuildDonateInfoArg &roArg, GetGuildDonateInfoRes &roRes)
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
	std::vector<MemberAskInfo>& infos = donate->GetAllMemberAskInfo();
	for (auto it = infos.begin(); it != infos.end(); ++it)
	{
		MemberAskInfo& info = *it;
		KKSG::GuildMemberAskInfo* data = roRes.add_info();
		donate->FillMemberAskInfo(info, *data);
	}

	std::map<UINT64, MemberDonateInfo>& donateInfos = donate->GetAllDonateRecord();
	for (auto it = donateInfos.begin(); it != donateInfos.end(); ++it)
	{
		MemberDonateInfo& info = it->second;
		KKSG::GuildMemberDonateRankItem* data = roRes.add_rankitem();
		donate->FillMemberDonateInfo(info, *data);
	}
	roRes.set_donatenum(donate->GetTodayDonateNum(pRole->GetID()));
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_GetGuildDonateInfo::OnDelayReplyRpc(const GetGuildDonateInfoArg &roArg, GetGuildDonateInfoRes &roRes, const CUserData &roUserData)
{
}
