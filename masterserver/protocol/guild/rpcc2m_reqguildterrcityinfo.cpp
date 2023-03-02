#include "pch.h"
#include "guild/rpcc2m_reqguildterrcityinfo.h"
#include "guild/guildterritorybattle.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "util/gametime.h"

#include "foreach.h"

// generate by ProtoGen at date: 2016/12/16 20:16:05

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildTerrCityInfo, ReqGuildTerrCityInfoArg, ReqGuildTerrCityInfo)

void RpcC2M_ReqGuildTerrCityInfo::OnCall(const ReqGuildTerrCityInfoArg &roArg, ReqGuildTerrCityInfo &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return ;
	}

	Guild* pGuildSelf = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	std::map<UINT32, CityUnit>& cityMap = CGuildTerritoryBattle::Instance()->GetCityMap(); 

	std::set<UINT32> idSet;
	CGuildTerritoryBattle::Instance()->GetAllCityId(idSet);

	// 当前是否有领地
	bool haveTerritory = false;
	if ( NULL != pGuildSelf )
	{
		for (auto it = cityMap.begin(); it != cityMap.end(); ++it)
		{
			if (it->second.m_guildId == pGuildSelf->GetID())
			{
				haveTerritory = true;
			}
		}
	}
	foreach (i in idSet)
	{
		KKSG::CityData* pData = roRes.add_cityinfo();
		pData->set_id(*i);
		pData->set_guildid(0);

		auto iter = cityMap.find(*i);
		Guild* pGuild = NULL;
		if (iter != cityMap.end())
		{
			pGuild = CGuildMgr::Instance()->GetGuild(iter->second.m_guildId); 
		}

		if (iter != cityMap.end() && pGuild != NULL)
		{
			pData->set_guildid(iter->second.m_guildId);
			pData->set_guildname(pGuild->GetName());
			if (pGuildSelf != NULL)
			{
				pData->set_type((KKSG::GUILDTERRTYPE)CGuildTerritoryBattle::Instance()->GetGameType(iter->first, haveTerritory,pGuildSelf));
			}
			else
			{
				pData->set_type(KKSG::TERR_NOT_OPEN);
			}

			if (GameTime::IsInSameDay(GameTime::GetTime(), iter->second.m_time, true))
			{
				pData->set_isgettoday(true);
			}
			else
			{
				pData->set_isgettoday(false);
			}
		}
		else
		{
			if (pGuildSelf != NULL)
			{
				pData->set_type((KKSG::GUILDTERRTYPE)CGuildTerritoryBattle::Instance()->GetGameType(*i, haveTerritory,pGuildSelf));
			}
			else
			{
				pData->set_type(KKSG::TERR_NOT_OPEN);
			}
			pData->set_isgettoday(false);
		}
	}

	roRes.set_type((KKSG::GUILDTERRTYPE)CGuildTerritoryBattle::Instance()->GetTimeType());
	if (pGuildSelf != NULL)
	{
		roRes.set_allianceid(CGuildTerritoryBattle::Instance()->GetAllianceId(pGuildSelf->GetID()));
	}
	else
	{
		roRes.set_allianceid(CGuildTerritoryBattle::Instance()->GetAllianceId(0));
	}

	if (pGuildSelf != NULL)
	{
		roRes.set_targetid(CGuildTerritoryBattle::Instance()->GetDeclareCityId(pGuildSelf->GetID()));
		roRes.set_max_territory_level(pGuildSelf->GetMaxTerritoryLevel());
	}
	else
	{
		roRes.set_targetid(0);
	}
}

void RpcC2M_ReqGuildTerrCityInfo::OnDelayReplyRpc(const ReqGuildTerrCityInfoArg &roArg, ReqGuildTerrCityInfo &roRes, const CUserData &roUserData)
{
}
