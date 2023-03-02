#include "pch.h"
#include "common/ptcg2m_reloadversionntf.h"
#include "config.h"

// generate by ProtoGen at date: 2016/8/10 0:16:39

void PtcG2M_ReloadVersionNtf::Process(UINT32 dwConnID)
{
	MSConfig::Instance()->HotLoadVersionFile();
}
