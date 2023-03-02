#include "pch.h"
#include "guildcastle/ptcg2m_gcfsyng2m.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "guildcastle/guildcastlerequest.h"

// generate by ProtoGen at date: 2016/12/19 10:33:47

void PtcG2M_GCFSynG2M::Process(UINT32 dwConnID)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if(NULL == pRole)
	{
		return ;
	}

	GcfRequest req(pRole);

	switch(m_Data.type())
	{
	case KKSG::GCF_SYN_KILL:
		{
			req.Kill(m_Data.uid(), m_Data.continuekill());				
		}
		break;
	case KKSG::GCF_SYN_OCCUPY:
		{
			req.Occupy(m_Data.jvdian(), m_Data.uid());
		}
		break;
	case KKSG::GCF_SYN_BACK_TO_READY:
		{
			req.BackToReady(m_Data.uid());
		}
		break;
	case KKSG::GCF_SYN_LEAVE_BATTLE:
		{
			req.OnLeave(m_Data.uid(), m_Data.sceneid());
		}
		break;
	default:
		break;
	}
}
