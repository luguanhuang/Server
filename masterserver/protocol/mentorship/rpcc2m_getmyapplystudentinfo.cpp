#include "pch.h"
#include "mentorship/rpcc2m_getmyapplystudentinfo.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentorrelationmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "mentorship/mentoroneside.h"
#include "mentorship/mentormsg.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/12/9 21:08:40

RPC_SERVER_IMPLEMETION(RpcC2M_GetMyApplyStudentInfo, GetMyApplyStudentInfoArg, GetMyApplyStudentInfoRes)

void RpcC2M_GetMyApplyStudentInfo::OnCall(const GetMyApplyStudentInfoArg &roArg, GetMyApplyStudentInfoRes &roRes)
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
	UINT32 error = psrc->GetMentorMsg()->ClientGetStudentRecommend(roRes, roArg.has_isrefresh() ? roArg.isrefresh() : false);
	roRes.set_error((KKSG::ErrorCode)error);
}

void RpcC2M_GetMyApplyStudentInfo::OnDelayReplyRpc(const GetMyApplyStudentInfoArg &roArg, GetMyApplyStudentInfoRes &roRes, const CUserData &roUserData)
{
}
