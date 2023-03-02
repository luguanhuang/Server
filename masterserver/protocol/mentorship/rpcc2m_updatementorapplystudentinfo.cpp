#include "pch.h"
#include "mentorship/rpcc2m_updatementorapplystudentinfo.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentorrelationmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "mentorship/mentoroneside.h"
#include "mentorship/mentormsg.h"
#include "mentorship/mentorrelationlist.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/12/9 22:54:06

RPC_SERVER_IMPLEMETION(RpcC2M_UpdateMentorApplyStudentInfo, UpdateMentorApplyStudentInfoArg, UpdateMentorApplyStudentInfoRes)

void RpcC2M_UpdateMentorApplyStudentInfo::OnCall(const UpdateMentorApplyStudentInfoArg &roArg, UpdateMentorApplyStudentInfoRes &roRes)
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

	MentorShipOneSide * pOneSide = MentorMgr::Instance()->GetMentorShip(role->GetID());
	if (!pOneSide)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	/*int pos = pOneSide->GetRelationList()->GetSelfPos();
	if (pos == KKSG::EMentorPosStudent)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}*/
	UINT32 error = pOneSide->UpdateSelfInfo(roArg);
	roRes.set_error((ErrorCode)(error));
}

void RpcC2M_UpdateMentorApplyStudentInfo::OnDelayReplyRpc(const UpdateMentorApplyStudentInfoArg &roArg, UpdateMentorApplyStudentInfoRes &roRes, const CUserData &roUserData)
{
}
