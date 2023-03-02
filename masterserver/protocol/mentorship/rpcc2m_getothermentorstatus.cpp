#include "pch.h"
#include "mentorship/rpcc2m_getothermentorstatus.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentorrelationmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/12/9 17:05:33

RPC_SERVER_IMPLEMETION(RpcC2M_GetOtherMentorStatus, GetOtherMentorStatusArg, GetOtherMentorStatusRes)

void RpcC2M_GetOtherMentorStatus::OnCall(const GetOtherMentorStatusArg &roArg, GetOtherMentorStatusRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	if (!role->IsSystemOpened(SYS_MENTOR))
	{
		roRes.set_error(KKSG::ERR_SYS_NOTOPEN);
		return;
	}

	MentorShipOneSide* psrc = MentorMgr::Instance()->GetMentorShip(role->GetID());
	MentorShipOneSide* pdest = MentorMgr::Instance()->GetMentorShip(roArg.roleid(), false);

	KKSG::MentorApplyStatus status = MentorRelationMgr::Instance()->GetRelationApplyStatus(psrc, pdest);
	roRes.set_error(KKSG::ERR_SUCCESS);
	roRes.set_status(status);
}

void RpcC2M_GetOtherMentorStatus::OnDelayReplyRpc(const GetOtherMentorStatusArg &roArg, GetOtherMentorStatusRes &roRes, const CUserData &roUserData)
{
}
