#include "pch.h"
#include "team/rpcm2h_teamtranms2ts.h"
#include "team/teamtranshandler.h"
#include "define/teamdef.h"

// generate by ProtoGen at date: 2017/4/25 21:35:52

RPC_CLIENT_IMPLEMETION(RpcM2H_TeamTranMs2Ts, TeamTransData, TeamTransRes)

void RpcM2H_TeamTranMs2Ts::OnReply(const TeamTransData &roArg, const TeamTransRes &roRes, const CUserData &roUserData)
{
	TeamTransHandler trans;		

	switch(roArg.type())
	{
	case TEAM_TRANS_OP_REQ:
		{
			if(roArg.delayid())
			{
				trans.HandleRoleOpResFromTs(roArg.delayid(), roArg.roleid(), roArg.c2moparg(), roRes.c2mopres());
			}
			else//特殊
			{
				if(roArg.specialtype() == THSpeType_Ac2Join)//接受邀请
				{
					trans.HandleErrorCodeFromTs(roArg.roleid(), roRes.c2mopres().result());
				}
			}
		}
		break;
	case TEAM_TRANS_FETCH_TEAM:
		{
			trans.HandleFetchTeamResFromTs(roArg.delayid(), roArg.roleid(), roRes.teamlistres());
		}
		break;
	case TEAM_TRANS_TEAM_MATCH:
		{
			trans.HandleMatchResFromTs(roArg.delayid(), roRes.matchres());
		}
		break;
	default:
		break;
	}
}

void RpcM2H_TeamTranMs2Ts::OnTimeout(const TeamTransData &roArg, const CUserData &roUserData)
{
}
