#include "pch.h"
#include "skyteambase.h"
#include "skyteam.h"
#include "skyteammgr.h"

void SkyTeamBase::SetDirty(bool dirty)
{
	if (m_dirty == false && dirty == true)
	{
		SkyTeamMgr::Instance()->AddToUpdateList(m_owner->GetId());
	}
	m_dirty = dirty;
}
