#include "pch.h"
#include "dragonguild/ptcg2m_reqdragonguildsimpledata.h"
#include "dragonguild/ptcm2g_syndragonguildsimpledata.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "network/gslink.h"
#include "table/globalconfig.h"
#include "foreach.h"

// generate by ProtoGen at date: 2017/9/6 15:37:41

void PtcG2M_reqdragonguildsimpledata::Process(UINT32 dwConnID)
{
	//const UINT32 SSIZE_MAX = 50;

	const std::unordered_map<UINT64, DragonGuild*>& allDragonGuildMap = DragonGuildMgr::Instance()->GetDragonGuilds();
	auto iter = allDragonGuildMap.begin();

	while (true)
	{
		UINT32 index = 0;
		PtcM2G_SynDragonGuildSimpleData msg;

		while (index < GetGlobalConfig().DragonGuildSyncDataMaxSize)
		{
			if (iter == allDragonGuildMap.end())
			{
				if (index > 0)
				{
					GSLink::Instance()->SendToLine(m_Data.gsline(), msg);
				}
				return ;
			}

			KKSG::dragonguildSimpleData* pSimpleData = msg.m_Data.add_data();
			pSimpleData->set_dragonguildid(iter->second->GetID());
			pSimpleData->set_dragonguildlv(iter->second->GetLevel());
			pSimpleData->set_dragonguildname(iter->second->GetName());

			const std::map<UINT64, DragonGuildMember>& allMember = iter->second->GetAllMember();
			foreach (i in allMember)
			{
				KKSG::dragonguildSimpleRoleInfo* pData = pSimpleData->add_addmembers(); 
				pData->set_roleid(i->second.roleId);
				pData->set_position(i->second.position);
			}

			++index;
			++iter;
		}
		GSLink::Instance()->SendToLine(m_Data.gsline(), msg);
	}
}
