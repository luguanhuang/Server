#include "pch.h"
#include "broadcast/ptcm2t_broadcastm2t.h"
#include "session/sessionmanager.h"
#include "netproc.h"

// generate by ProtoGen at date: 2016/10/11 14:31:05

void PtcM2T_BroadCastM2T::Process(UINT32 dwConnID)
{
	switch(m_Data.data().broadcasttype())
	{
	case KKSG::BroadCastToAll:
		for (auto iter = SessionManager::Instance()->Begin(); iter != SessionManager::Instance()->End(); ++iter)
		{
			CNetProcessor::Instance()->Send(iter->m_connID, m_Data.data().broadcastdata().c_str(), m_Data.data().datalen());
		}
		break;
	default:
		break;
	}
}
