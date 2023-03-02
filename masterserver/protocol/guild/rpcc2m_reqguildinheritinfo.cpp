#include "pch.h"
#include "guild/rpcc2m_reqguildinheritinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guildinherit.h"
#include "../share/util/gametime.h"

#include "foreach.h"

// generate by ProtoGen at date: 2016/11/18 16:23:02

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildInheritInfo, ReqGuildInheritInfoArg, ReqGuildInheritInfoRes)

void RpcC2M_ReqGuildInheritInfo::OnCall(const ReqGuildInheritInfoArg &roArg, ReqGuildInheritInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}

	UINT32 nowTime = GameTime::GetTime(); 
	std::map<UINT64, UINT32> roleData;
	CGuildInherit::Instance()->GetReqRole(pRole->GetID(), roleData);
	foreach (i in roleData)
	{
		CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(i->first);
		if (pRole != NULL)
		{
			KKSG::InheritData* pData = roRes.add_data();
			pData->set_name(pRoleSum->GetName());
			pData->set_lvl(pRoleSum->GetLevel());
			pData->set_time(nowTime - i->second);
			pData->set_roleid(pRoleSum->GetID());
		}
	}
}

void RpcC2M_ReqGuildInheritInfo::OnDelayReplyRpc(const ReqGuildInheritInfoArg &roArg, ReqGuildInheritInfoRes &roRes, const CUserData &roUserData)
{
}
