#include "pch.h"
#include "mentorship/rpcc2m_mentormybeappliedmsg.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentorrelationmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "mentorship/mentoroneside.h"
#include "mentorship/mentormsg.h"
#include "define/systemiddef.h"
#include "guild/rpcm2g_tryaddguildinherit.h"
#include "guild/rpcm2g_tryteacherinheritnew.h"
#include "guild/guildinherit.h"
// generate by ProtoGen at date: 2016/12/9 22:40:44

RPC_SERVER_IMPLEMETION(RpcC2M_MentorMyBeAppliedMsg, MentorMyBeAppliedMsgArg, MentorMyBeAppliedMsgRes)

void RpcC2M_MentorMyBeAppliedMsg::OnCall(const MentorMyBeAppliedMsgArg &roArg, MentorMyBeAppliedMsgRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role || !roArg.has_operation())
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

	/*UINT32 error = pOneSide->GetMentorMsg()->OperateMsg(roArg, roRes);
	roRes.set_error((KKSG::ErrorCode)error);
	if (error != KKSG::ERR_SUCCESS)
	{
		return;
	}*/
	
	if (roArg.msgtype() == KKSG::MentorMsgApplyInherit && roArg.operation() == KKSG::EMentorMsgOpType_Agree)
	{
		auto success_result = CGuildInherit::Instance()->StartTeacherInherit(role->GetID(), roArg.roleid());
		if (success_result != KKSG::ERR_SUCCESS)
		{
			roRes.set_error(success_result);
			return;
		}
		
		RpcM2G_TryTeacherInheritNew  *msg = RpcM2G_TryTeacherInheritNew::CreateRpc();
		msg->m_oArg.set_role_one(role->GetID());
		msg->m_oArg.set_role_two(roArg.roleid());
		msg->m_oArg.set_rpc_id(DelayRpc());	
		role->SendMsgToGS(*msg);
	}
	else
	{
		UINT32 error = pOneSide->GetMentorMsg()->OperateMsg(roArg, roRes);
		roRes.set_error((KKSG::ErrorCode)error);		
	}
}

void RpcC2M_MentorMyBeAppliedMsg::OnDelayReplyRpc(const MentorMyBeAppliedMsgArg &roArg, MentorMyBeAppliedMsgRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_error((KKSG::ErrorCode)roUserData.m_dwUserData);
		return ;
	}

	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if ( NULL == role)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	
	auto success_result = CGuildInherit::Instance()->StartTeacherInherit(role->GetID(), roArg.roleid());
	if (success_result != KKSG::ERR_SUCCESS)
	{
		roRes.set_error(success_result);
		return;
	}

	MentorShipOneSide* pOneSide = MentorMgr::Instance()->GetMentorShip(role->GetID());
	if (!pOneSide)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	UINT32 error = pOneSide->GetMentorMsg()->OperateMsg(roArg, roRes);
	roRes.set_error((KKSG::ErrorCode)error);
	if (error != KKSG::ERR_SUCCESS)
	{
		return;
	}

	/*PtcM2G_TryTeacherInherit msg;
	msg.m_Data.set_guildidone(role->GetID());
	msg.m_Data.set_guildidtwo(roArg.roleid());
	role->SendMsgToGS(msg);*/
}
