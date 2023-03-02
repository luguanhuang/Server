#include "pch.h"
#include "guild/ptcg2m_noticeguildbossdie.h"
#include "guild/ptcm2g_noticeguildbossdiegs.h"
#include "commondef.h"
#include "network/gslink.h"
#include "guild/guildboss.h"
#include "notice/noticemgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/8/6 17:55:39

void PtcG2M_NoticeGuildBossDie::Process(UINT32 dwConnID)
{
	CGuildBossMgr::Instance()->AddFinishGuild(m_Data.id());
	NoticeMgr::Instance()->NoticeGuildBossLvlup(m_Data.id());
	
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(m_Data.id()); 
	if (pGuild != NULL)
	{
		CGuildBossMgr::Instance()->HintNotify(pGuild, true); // 取消强弹通知
		pGuild->RecordBossDps(m_Data.dpsid());
		pGuild->SetBossIndex(m_Data.bossindex());  // 更新boss序列号
	}

	std::map<UINT64, std::set<GuildBossRoleDamage> >& ranks = CGuildBossMgr::Instance()->GetGuildDamage();
	std::set<GuildBossRoleDamage>& damageSet = ranks[m_Data.id()]; 
	for (UINT32 cnt = 0; cnt < m_Data.roledamages_size(); ++cnt)
	{
		GuildBossRoleDamage data;
		data.roleid = m_Data.roledamages(cnt).roleid();
		data.damage = m_Data.roledamages(cnt).damage();
		damageSet.insert(data);
	}

	if (damageSet.size() > 0)
	{
		CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(damageSet.begin()->roleid);
		if (pSum != NULL)
		{
			NoticeMgr::Instance()->NoticeGuildBossDps(pSum->GetName(), m_Data.id());
		}
	}

}
