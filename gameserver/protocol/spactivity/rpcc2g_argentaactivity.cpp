#include "pch.h"
#include "spactivity/rpcc2g_argentaactivity.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "spactivity/spactivityrecord.h"
#include "spactivity/spactivitydatasmall.h"

// generate by ProtoGen at date: 2017/4/11 14:48:30

RPC_SERVER_IMPLEMETION(RpcC2G_ArgentaActivity, ArgentaActivityArg, ArgentaActivityRes)

void RpcC2G_ArgentaActivity::OnCall(const ArgentaActivityArg &roArg, ArgentaActivityRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	
	switch (roArg.type())
	{
	case 1:
		{
			SpActArgentaData* pArgentData = static_cast<SpActArgentaData*>(pRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_Argenta));
			if (pArgentData == NULL)
			{
				LogWarn("ArgentData is NULL, role [%llu]", pRole->GetID());
				roRes.set_errorcode(KKSG::ERR_SPACTIVITY_NOTOPEN);
				return;
			}
			pArgentData->FillInfo(roRes);
			return;
		}
	case 2:
		{
			SpActArgentaData* pArgentData = static_cast<SpActArgentaData*>(pRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_Argenta));
			if (pArgentData == NULL)
			{
				LogWarn("ArgentData is NULL, role [%llu]", pRole->GetID());
				roRes.set_errorcode(KKSG::ERR_SPACTIVITY_NOTOPEN);
				return;
			}
			roRes.set_errorcode(pArgentData->GetDailyReward(roArg.id()));
			return;
		}
	default:
		{
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			return;
		}
	}
}

void RpcC2G_ArgentaActivity::OnDelayReplyRpc(const ArgentaActivityArg &roArg, ArgentaActivityRes &roRes, const CUserData &roUserData)
{
}
