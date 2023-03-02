#include "pch.h"
#include "spactivity/rpcc2g_getspactivitybigprize.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "spactivity/spactivityrecord.h"
#include "spactivity/spactivitydef.h"
#include "spactivity/spactivitydatasmall.h"

// generate by ProtoGen at date: 2016/10/11 20:14:43

RPC_SERVER_IMPLEMETION(RpcC2G_GetSpActivityBigPrize, GetSpActivityBigPrizeArg, GetSpActivityBigPrizeRes)

void RpcC2G_GetSpActivityBigPrize::OnCall(const GetSpActivityBigPrizeArg &roArg, GetSpActivityBigPrizeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END; 
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return; 
	}	

	SpActivityDataBase* pDataBase = pRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_OpenServer);
	if (pDataBase == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	SpActOpenServerData* pOpenData = static_cast<SpActOpenServerData*>(pDataBase);
	KKSG::ErrorCode errorCode = pOpenData->GetBigPrize();
	roRes.set_errorcode(errorCode);
}

void RpcC2G_GetSpActivityBigPrize::OnDelayReplyRpc(const GetSpActivityBigPrizeArg &roArg, GetSpActivityBigPrizeRes &roRes, const CUserData &roUserData)
{
}
