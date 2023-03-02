#include "pch.h"
#include "match/rpcm2k_kmatchreqm2k.h"
#include "team/teammgr.h"
#include "other/matchhandler.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "team/team.h"

// generate by ProtoGen at date: 2017/5/5 21:17:38

RPC_CLIENT_IMPLEMETION(RpcM2K_KMatchReqm2k, KMatchTransArg, KMatchCommonRes)

void RpcM2K_KMatchReqm2k::OnReply(const KMatchTransArg &roArg, const KMatchCommonRes &roRes, const CUserData &roUserData)
{
	const auto& c2mArg = roArg.matcharg();
	const auto& kunit = roArg.kunit();
	auto ret = roRes.errorcode();

	SSInfo << "matchm2k ret = " << ret << " type = " << c2mArg.type() << " op = " << c2mArg.op() << " role = " << kunit.roleid() << " team = " << kunit.teamid() << END;

	if(KKSG::ERR_SUCCESS == ret)
	{
		return;
	}
	if(KKSG::KMATCH_OP_STOP == c2mArg.op())
	{
		return;
	}

	switch(c2mArg.type())
	{
	case KKSG::KMT_CUSTOM_PKTWO:
		{
			HMatchHandler handler;
			if(c2mArg.isteam())
			{
				Team* pTeam = TeamMgr::Instance()->FindTeam(kunit.teamid());
				if(pTeam && pTeam->GetMatchUniIndex() == roArg.uniqueindex())
				{
					handler.Done(pTeam, c2mArg.type());		
					CRole* pRole = CRoleManager::Instance()->GetByRoleID(pTeam->GetLeader());
					if(pRole)
					{
						pRole->ErrorCodeNtf(ret, true);
					}
				}
			}
			else
			{
				CRole* pRole = CRoleManager::Instance()->GetByRoleID(kunit.roleid());
				if(pRole && pRole->GetMatchUniIndex() == roArg.uniqueindex())
				{
					handler.Done(pRole, c2mArg.type());
					pRole->ErrorCodeNtf(ret, true);
				}
			}
		}
		break;
	default:
		{
			SSWarn << " m2k match err " << END;
		}
		break;
	}
}

void RpcM2K_KMatchReqm2k::OnTimeout(const KMatchTransArg &roArg, const CUserData &roUserData)
{
}
