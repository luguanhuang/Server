#include "pch.h"
#include "leagueteam.h"
#include "leagueteammgr.h"

void LeagueTeamBase::SetDirty(bool dirty)
{
	if (!m_dirty && dirty)
	{
		LeagueTeamMgr::Instance()->AddToUpdateList(m_owner->GetId());
	}
	m_dirty = dirty;
}