#include "pch.h"
#include "role/rpcc2g_chooseprofession.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"

// generate by ProtoGen at date: 2015/4/14 13:48:01

RPC_SERVER_IMPLEMETION(RpcC2G_ChooseProfession, ChooseProfArg, ChooseProfRes)

void RpcC2G_ChooseProfession::OnCall(const ChooseProfArg &roArg, ChooseProfRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		INT32 oldPro = pRole->GetProfession();
		roRes.set_errorcode((KKSG::ErrorCode)pRole->ChooseProfession(roArg.prof()));
		TProfessionFlow oLog(pRole);
		oLog.m_PreProfession = oldPro;
		oLog.m_Op = TX_Transfer_Profession;
		oLog.Do();
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
	}
}

void RpcC2G_ChooseProfession::OnDelayReplyRpc(const ChooseProfArg &roArg, ChooseProfRes &roRes, const CUserData &roUserData)
{
}
