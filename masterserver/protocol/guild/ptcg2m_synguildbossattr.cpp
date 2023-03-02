#include "pch.h"
#include "guild/ptcg2m_synguildbossattr.h"
#include "guild/guildboss.h"

// generate by ProtoGen at date: 2017/1/15 15:41:32

void PtcG2M_SynGuildBossAttr::Process(UINT32 dwConnID)
{
	CGuildBossMgr::Instance()->AddAttrCount(m_Data.guildid(), m_Data.sceneid());
}
