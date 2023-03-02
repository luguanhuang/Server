#include "pch.h"
#include "battle/ptcc2g_autofightntf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "entity/XActionDefine.h"
#include "entity/XObject.h"

// generate by ProtoGen at date: 2016/5/19 20:40:24

void PtcC2G_AutoFightNTF::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		return ;
	}
	
	pRole->SetClientAutoFight(m_Data.autof());

}
