#include "pch.h"
#include "dragonguildbase.h"
#include "dragonguildmgr.h"

void DragonGuildBase::SetDirty(bool dirty)
{
	if (!m_dirty && dirty)
	{
		DragonGuildMgr::Instance()->AddToUpdateList(m_owner->GetID());	
	}
	m_dirty = dirty;
}