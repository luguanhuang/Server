#include "pch.h"
#include "guild/ptcg2m_gmfupdatedatag2m.h"
#include "gmf/guildmatchfightmgr.h"

// generate by ProtoGen at date: 2016/9/20 18:11:46

void PtcG2M_GmfUpdateDataG2M::Process(UINT32 dwConnID)
{
	GuildMatchFightMgr::Instance()->HandlerGmfUpdate(m_Data);
}
