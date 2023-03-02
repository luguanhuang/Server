#include "pch.h"
#include "team/rpcs2m_teamrequestplatfreinds2m.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teamtranshandler.h"
#include "table/expeditionconfigmgr.h"

// generate by ProtoGen at date: 2017/5/3 15:27:44

RPC_SERVER_IMPLEMETION(RpcS2M_TeamRequestPlatFreindS2M, TeamRequestPlatFreindS2MArg, TeamRequestPlatFreindS2MRes)

void RpcS2M_TeamRequestPlatFreindS2M::OnCall(const TeamRequestPlatFreindS2MArg &roArg, TeamRequestPlatFreindS2MRes &roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.destinfo().roleid());
	if (!role)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	//服务器配置不相同兼容
	if(!TeamTransHandler::NeedInTs(roArg.teaminviteinfo().teambrief().expid()))
	{
		roRes.set_error(KKSG::ERR_TEAM_CANNOT_FORNOW);
		return;
	}
	
	//模拟成请求建队
	KKSG::TeamTransData para;
	para.set_roleid(roArg.destinfo().roleid());
	KKSG::TeamOPArg& c2mArg = *para.mutable_c2moparg();
	c2mArg.set_request(KKSG::TEAM_CREATE);
	c2mArg.set_expid(roArg.teaminviteinfo().teambrief().expid());
	TeamTransHandler trans;	
	trans.HandleRoleOpReqFromTs(DelayRpc(), role, para);
}

void RpcS2M_TeamRequestPlatFreindS2M::OnDelayReplyRpc(const TeamRequestPlatFreindS2MArg &roArg, TeamRequestPlatFreindS2MRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		return;
	}
	KKSG::TeamTransRes* tmpRes = (KKSG::TeamTransRes*)(roUserData.m_pUserPtr);
	KKSG::ErrorCode ret = tmpRes->c2mopres().result();//结果 ERR_SUCCUSS表示成功
	roRes.set_error(ret);
}
