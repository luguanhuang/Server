#include "pch.h"
#include "role/rolemanager.h"
#include "friend/friendop.h"
#include "friend/ptcc2m_friendqueryreportnew.h"

// generate by ProtoGen at date: 2016/7/28 22:58:55

void PtcC2M_FriendQueryReportNew::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	FriendOp op(role->GetID());
	if (m_Data.op() == KKSG::Friend_FriendAll)
	{
		op.NotifyFriend();
	}
	else if (m_Data.op() == KKSG::Friend_ApplyAll)
	{
		op.NotifyApply();
	}
	else if (m_Data.op() == KKSG::Friend_GiftInfo)
	{
		op.NotifyGiftCount();
	}
}
