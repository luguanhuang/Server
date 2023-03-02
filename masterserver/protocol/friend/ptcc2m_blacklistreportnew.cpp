#include "pch.h"
#include "role/rolemanager.h"
#include "friend/friendop.h"
#include "friend/ptcc2m_blacklistreportnew.h"

// generate by ProtoGen at date: 2016/7/29 14:26:00

void PtcC2M_BlackListReportNew::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	FriendOp op(role->GetID());
	op.SendBlackList(role);
}
