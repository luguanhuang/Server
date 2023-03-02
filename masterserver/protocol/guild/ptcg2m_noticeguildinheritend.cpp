#include "pch.h"
#include "guild/ptcg2m_noticeguildinheritend.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "friend/friendop.h"
#include "guild/guildinherit.h"

// generate by ProtoGen at date: 2016/11/23 17:03:20

void PtcG2M_NoticeGuildInheritEnd::Process(UINT32 dwConnID)
{
	CRole* pRoleOne = CRoleManager::Instance()->GetByRoleID(m_Data.roleone());
	CRole* pRoleTwo = CRoleManager::Instance()->GetByRoleID(m_Data.roletwo());

	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		return ;
	}
		
	CGuildInherit::Instance()->NoticeReqNum(pRoleOne->GetID());
	CGuildInherit::Instance()->NoticeReqNum(pRoleTwo->GetID());

	if (m_Data.isend())
	{
		FriendOp op(m_Data.roleone());
		if (op.IsFriend(m_Data.roletwo()))
		{
			op.SetOther(m_Data.roletwo());
			op.AddDegree(0, GetGlobalConfig().GuildInheritFirend);
		}
	}
}
