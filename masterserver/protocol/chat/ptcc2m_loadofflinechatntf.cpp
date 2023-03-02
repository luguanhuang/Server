#include "pch.h"
#include "chat/ptcc2m_loadofflinechatntf.h"
#include "role/rolemanager.h"
#include "chat/chatmgr.h"

// generate by ProtoGen at date: 2016/8/3 2:06:32

void PtcC2M_LoadOfflineChatNtf::Process(UINT32 dwConnID)
{
	UINT64 roleid = m_Data.roleid();
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if(NULL == pRole)
	{
		SSError << "Cannot Find Role, RoleID:" << roleid << END;
		return; 
	}
	
	CChatMgr::Instance()->OffLineChatNotify(pRole);
}
