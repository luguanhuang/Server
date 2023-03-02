#include "pch.h"
#include "role/rolemanager.h"
#include "friend/friendop.h"
#include "friend/rpcc2m_doaddfriendnew.h"

// generate by ProtoGen at date: 2016/7/28 23:00:49

RPC_SERVER_IMPLEMETION(RpcC2M_DoAddFriendNew, DoAddFriendArg, DoAddFriendRes)

void RpcC2M_DoAddFriendNew::OnCall(const DoAddFriendArg &roArg, DoAddFriendRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}

	UINT32 errorcode = ERR_SUCCESS;
	bool istrue = false;
	switch (roArg.op())
	{
	case KKSG::Friend_AgreeApply:
		{
			for (int i = 0; i < roArg.roleid_size(); ++i)
			{
				FriendOp op(role->GetID());
				if (op.DeleteApply(roArg.roleid(i)))
				{
					op.SetOther(roArg.roleid(i));
					UINT32 ret = op.AddFriend();
					if (ret == ERR_SUCCESS)
					{
						istrue = true;
					}
					else
					{
						errorcode = ret;
					}
				}
			}
		}
	case KKSG::Friend_IgnoreApply:
		{
			for (int i = 0; i < roArg.roleid_size(); ++i)
			{
				FriendOp op(role->GetID());
				op.DeleteApply(roArg.roleid(i));
			}
		}
	default:
		break;
	}

	roRes.set_errorcode(istrue ?  ERR_SUCCESS : KKSG::ErrorCode(errorcode));

}

void RpcC2M_DoAddFriendNew::OnDelayReplyRpc(const DoAddFriendArg &roArg, DoAddFriendRes &roRes, const CUserData &roUserData)
{
}
