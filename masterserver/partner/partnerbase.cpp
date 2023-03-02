#include "pch.h"
#include "partnerbase.h"
#include "partnermgr.h"

void PartnerBase::SetDirty(bool dirty)
{
	if (!m_dirty && dirty)
	{
		PartnerMgr::Instance()->AddToUpdateList(m_owner->GetID());	
	}
	m_dirty = dirty;
}