#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/rpcc2g_queryscenetime.h"

// generate by ProtoGen at date: 2016/9/16 10:07:52

RPC_SERVER_IMPLEMETION(RpcC2G_QuerySceneTime, QuerySceneTimeArg, QuerySceneTimeRes)

void RpcC2G_QuerySceneTime::OnCall(const QuerySceneTimeArg &roArg, QuerySceneTimeRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role || NULL == role->GetCurrScene())
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"role is null, session:"<<m_sessionID<<END;
		return;
	}
	time_t timespan = 0;
	if (role->GetCurrScene()->IsSceneReady()) 
	{
		timespan = role->GetCurrScene()->CostTime();
	}
	roRes.set_time(timespan);
}

void RpcC2G_QuerySceneTime::OnDelayReplyRpc(const QuerySceneTimeArg &roArg, QuerySceneTimeRes &roRes, const CUserData &roUserData)
{
}
