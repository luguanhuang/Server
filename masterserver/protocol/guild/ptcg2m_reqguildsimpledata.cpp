#include "pch.h"
#include "guild/ptcg2m_reqguildsimpledata.h"
#include "guild/ptcm2g_synguildsimpledata.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "network/gslink.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/12 20:34:28

void PtcG2M_ReqGuildSimpleData::Process(UINT32 dwConnID)
{
	const UINT32 SIZE_NUM = 50; // 每次发送50个公会的数据到gs

	const std::unordered_map<UINT64, Guild*>& allGuildMap = CGuildMgr::Instance()->GetGuilds();
	auto iter = allGuildMap.begin();

	while (true)
	{
		UINT32 index = 0;
		PtcM2G_SynGuildSimpleData msg;

		while (index < SIZE_NUM)
		{
			if (iter == allGuildMap.end())
			{
				if (index > 0)
				{
					GSLink::Instance()->SendToLine(m_Data.gslineid(), msg);
				}
				return ;
			}

			KKSG::GuildSimpleData* pSimpleData =  msg.m_Data.add_datas();
			pSimpleData->set_guildid(iter->second->GetID());
			//pSimpleData->set_guildleaderid(iter->second->GetLeaderID());
			pSimpleData->set_guildlvl(iter->second->GetLevel());
			pSimpleData->set_icon(iter->second->GetIcon());
			pSimpleData->set_guildname(iter->second->GetName());
			const std::map<UINT64, GuildMember>& allMember = iter->second->GetGuildMember();
			foreach (i in allMember)
			{
				KKSG::guildmemberSimple* pData = pSimpleData->add_addmembers(); 
				pData->set_roleid(i->first);
				pData->set_position(i->second.position);
			}
			++index;
			++iter;
		}
		GSLink::Instance()->SendToLine(m_Data.gslineid(), msg);
	}

}
