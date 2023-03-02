#include "pch.h"
#include "mentorship/rpcc2m_mentorrelationop.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentorrelationmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "mentorship/mentoroneside.h"
#include "mentorship/mentormsg.h"
#include "util/gametime.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/12/9 22:57:42

RPC_SERVER_IMPLEMETION(RpcC2M_MentorRelationOp, MentorRelationOpArg, MentorRelationOpRes)

void RpcC2M_MentorRelationOp::OnCall(const MentorRelationOpArg &roArg, MentorRelationOpRes &roRes)
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
	if (!psrc)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	UINT32 error = psrc->ClientReqestRelationOp(roArg);
	roRes.set_error((ErrorCode)error);
	roRes.set_curtime(GameTime::GetTime());
}

void RpcC2M_MentorRelationOp::OnDelayReplyRpc(const MentorRelationOpArg &roArg, MentorRelationOpRes &roRes, const CUserData &roUserData)
{
}
