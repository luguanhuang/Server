#include "pch.h"
#include "guild/ptcm2g_mnewguildbonusntf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/ptcg2c_newguildbonusntf.h"
#include "guild/guildrecord.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildconfig.h"

// generate by ProtoGen at date: 2016/9/16 0:15:02


static void SendBonusNtf(Role* role, KKSG::MNewGuildBonusData& data)
{
	if (role)
	{
		auto record = role->Get<CGuildRecord>();
		PtcG2C_NewGuildBonusNtf ntf;
		ntf.m_Data.set_showiconinscreen(data.showiconinscreen());
		bool maxNum = true;
		for (int i = 0; i < data.bonustemplateids_size(); ++i)
		{
			auto info = GuildConfig::Instance()->GetBonusTemplateInfo(data.bonustemplateids(i));
			if (record->CheckAddGetBonusNum(info, info->GuildBonusID, 1) == KKSG::ERR_SUCCESS)
			{
				maxNum = false;
				break;
			}
		}
		if (maxNum)
		{
			return;
		}
		if (data.hasnormalbonus())
		{
			role->Send(ntf);
		}
		else if (data.hascheckinbonus())
		{
			if(role->Get<CGuildRecord>()->GetCheckInBonusNum() <  GetGlobalConfig().MaxGetGuildCheckInBonusNum)
			{
				role->Send(ntf);
			}
		}
	}

}

void PtcM2G_MNewGuildBonusNtf::Process(UINT32 dwConnID)
{
	if (m_Data.roleid())
	{
		Role* role = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
		if (role)
		{
			SendBonusNtf(role, m_Data);
		}
	}
	else if (m_Data.guildid())
	{
		CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildSimple(m_Data.guildid());
		if (guild)
		{
			const std::map<UINT64, UINT32>& members = guild->getAllMember();
			for(auto iter = members.begin(); iter != members.end(); ++iter)
			{
				bool isExclude = false;
				for (int i = 0; i < m_Data.excluderoleid_size(); ++i)
				{
					if (m_Data.excluderoleid(i) == iter->first)
					{
						isExclude = true;
						break;
					}
				}
				if (isExclude)
				{
					continue;
				}
				Role* role = RoleManager::Instance()->FindByRoleID(iter->first);
				if (role)
				{
					SendBonusNtf(role, m_Data);
				}
			}
		}
	}
}
