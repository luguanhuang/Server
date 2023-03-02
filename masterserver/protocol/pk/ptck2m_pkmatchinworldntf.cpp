#include "pch.h"
#include "pk/ptck2m_pkmatchinworldntf.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "pk/pkmgr.h"

// generate by ProtoGen at date: 2017/2/2 19:46:55

void PtcK2M_PkMatchInWorldNtf::Process(UINT32 dwConnID)
{
	switch(m_Data.type())
	{
	case KKSG::MTWT_MATCH_INWORLD:
		{
			PkMgr::Instance()->OnMatchInWorld(m_Data);
		}
		break;
	default:
		break;
	}
}
