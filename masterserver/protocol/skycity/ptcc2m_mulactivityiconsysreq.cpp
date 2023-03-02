#include "pch.h"
#include "skycity/ptcc2m_mulactivityiconsysreq.h"
#include "role/rolemanager.h"
#include "define/systemiddef.h"
#include "skycity/skycitymgr.h"


// generate by ProtoGen at date: 2017/10/7 15:44:50

void PtcC2M_MulActivityIconSysReq::Process(UINT32 dwConnID)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		return;	
	}
	if (m_Data.id() == SYS_SKYARENAEND)
	{
		SkyCityMgr::Instance()->ClearRoleIcon(pRole);
	}
}
