#include "pch.h"
#include "match/rpcm2g_kmatchcommonreqm2c.h"
#include "unit/rolemanager.h"
#include "gamelogic/teammgr.h"
#include "gamelogic/team.h"
#include "gamelogic/herobattlerecord.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/matchhandler.h"
#include "define/teamdef.h"

// generate by ProtoGen at date: 2017/5/14 19:48:10

RPC_SERVER_IMPLEMETION(RpcM2G_KMatchCommonReqM2C, KMatchCommonArgM2G, KMatchCommonResM2G)

void RpcM2G_KMatchCommonReqM2C::OnCall(const KMatchCommonArgM2G &roArg, KMatchCommonResM2G &roRes)
{
	const auto& c2mArg = roArg.c2marg();
	auto& c2mRes = *roRes.mutable_c2mres();

	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		c2mRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	KKSG::ErrorCode ret = KKSG::ERR_SUCCESS;
	if(c2mArg.op() == KKSG::KMATCH_OP_START)
	{
		//填充数据
		switch(c2mArg.type())
		{
		default:
			{
				std::vector<Role*> roles;
				if(c2mArg.isteam())
				{
					TeamCopy* pTeam = TeamCopyMgr::Instance()->GetTeamByRoleID(pRole->GetID());
					if(pTeam)
					{
						pTeam->GetOnlineRoles(roles);
					}
				}
				else
				{
					roles.push_back(pRole);
				}
				if(roles.empty())
				{
					SSWarn << " role size = 0 roleid = " << pRole->GetID() << " type = " << c2mArg.type() << " isteam = " << c2mArg.isteam() << END;
					ret = KKSG::ERR_FAILED;
					break;
				}
				MatchHandler handler;
				for(size_t i = 0; i < roles.size(); ++i)
				{
					if(roles[i])
					{
						ret = handler.RoleCondition(roles[i], c2mArg.type(), roRes.add_roledata());
						if(KKSG::ERR_SUCCESS != ret)
						{
							break;
						}
					}
					else
					{
						ret = KKSG::ERR_FAILED;
						break;
					}
				}
			}
			break;
		}
	}
	c2mRes.set_errorcode(ret);

}

void RpcM2G_KMatchCommonReqM2C::OnDelayReplyRpc(const KMatchCommonArgM2G &roArg, KMatchCommonResM2G &roRes, const CUserData &roUserData)
{
}
