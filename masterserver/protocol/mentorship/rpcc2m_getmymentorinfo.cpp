#include "pch.h"
#include "mentorship/rpcc2m_getmymentorinfo.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentorrelationmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "mentorship/mentoroneside.h"
#include <time.h>
#include "define/systemiddef.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/12/9 20:55:38

RPC_SERVER_IMPLEMETION(RpcC2M_GetMyMentorInfo, GetMyMentorInfoArg, GetMyMentorInfoRes)

void RpcC2M_GetMyMentorInfo::OnCall(const GetMyMentorInfoArg &roArg, GetMyMentorInfoRes &roRes)
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

	MentorShipOneSide* pOneSide = MentorMgr::Instance()->GetMentorShip(role->GetID());
	if (!pOneSide)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	UINT32 error = pOneSide->ClientGetMentorInfo(roRes);
	roRes.set_curtime(GameTime::GetTime());
	roRes.set_error((KKSG::ErrorCode)error);
}

void RpcC2M_GetMyMentorInfo::OnDelayReplyRpc(const GetMyMentorInfoArg &roArg, GetMyMentorInfoRes &roRes, const CUserData &roUserData)
{
}
