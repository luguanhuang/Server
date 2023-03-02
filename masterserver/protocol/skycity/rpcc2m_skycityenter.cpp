#include "pch.h"
#include "skycity/rpcc2m_skycityenter.h"
#include "role/rolemanager.h"
#include "table/MultActivityMgr.h"
#include "../share/util/XCommon.h"
#include "skycity/skycitymgr.h"
// generate by ProtoGen at date: 2016/10/1 10:59:29

RPC_SERVER_IMPLEMETION(RpcC2M_SkyCityEnter, SkyCityEnterArg, SkyCityEnterRes)

void RpcC2M_SkyCityEnter::OnCall(const SkyCityEnterArg &roArg, SkyCityEnterRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;	
	}

	bool bState	= MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_SKYCITY);
	if (!bState)
	{
		roRes.set_error(ERR_SKYCITY_NOT_OPEN);
		return;	
	}
	if (pRole->GetTeamID()!=0)
	{
		roRes.set_error(ERR_SKYCITY_IN_TEAM);
		return;	
	}
	
	if (!SkyCityMgr::Instance()->CheckLevel(pRole))
	{
		roRes.set_error(ERR_SKYCITY_LV);
		return;	
	}

	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return ;
	}

	SkyCityMgr::Instance()->EnterWaitRoom(pRole);
	roRes.set_error(ERR_SUCCESS);

}

void RpcC2M_SkyCityEnter::OnDelayReplyRpc(const SkyCityEnterArg &roArg, SkyCityEnterRes &roRes, const CUserData &roUserData)
{
}
