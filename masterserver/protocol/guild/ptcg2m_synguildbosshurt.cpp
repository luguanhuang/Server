#include "pch.h"
#include "guild/ptcg2m_synguildbosshurt.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"
#include "loghelper/tlogr.h"
#include "account/punishmgr.h"

// generate by ProtoGen at date: 2016/9/16 12:07:20

void PtcG2M_SynGuildBossHurt::Process(UINT32 dwConnID)
{
	if (!CGuildBossMgr::Instance()->IsSceneOpen(m_Data.guildid()))
	{
		SSWarn << " guild boss hurt scene not open guildid : " << m_Data.guildid() << END;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(m_Data.guildid()); 
	if (pGuild == NULL)
	{
		SSWarn << " guild boss hurt guild is null guildid : " << m_Data.guildid() << END;
		return ;
	}
	
	double allHurt = m_Data.hurt();
	UINT64 roleId = 0;
	std::set<UINT32> lineSet;
	std::map<UINT64, std::set<GuildBossRoleDamage> >& allDamageMap = CGuildBossMgr::Instance()->GetGuildDamage();
	std::set<GuildBossRoleDamage>& damageSet = allDamageMap[m_Data.guildid()];
	for (size_t cnt = 0; cnt < m_Data.rolehurt_size(); ++cnt)
	{
		const KKSG::GuildBossRoleHurt& roleHurt = m_Data.rolehurt(cnt);
		allHurt += roleHurt.hurt();

		KKSG::PunishData punishData;
		if (CPunishMgr::Instance()->GetPunishInfo(roleHurt.roleid(), PUNISH_USER_ROLE_GUILDBOSS, punishData)) // 检查是否被封停
		{
			continue;
		}

		if (roleHurt.hurt() > 0)
		{
			GuildBossRoleDamage damage;
			damage.roleid = roleHurt.roleid(); 
			damage.damage = roleHurt.hurt();
			SSInfo << "guild boss hurt roleid : " << damage.roleid << " hurt: " << damage.damage << END; 

			for (auto i = damageSet.begin(); i != damageSet.end(); ++i)
			{
				if (i->roleid == roleHurt.roleid())
				{
					damage.damage += i->damage;
					damageSet.erase(i);
					break;
				}
			}

			damageSet.insert(damage);
			roleId = roleHurt.roleid();

		}
	}



	CGuildBossMgr::Instance()->DelGuildBossHp(m_Data.guildid(), allHurt, m_Data.sceneid(), roleId);
}
