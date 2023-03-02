#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "network/dblink.h"
#include "table/ProfessionMgr.h"
#include "gamelogic/changeprofession.h"
#include "role/rpcc2g_changeprofession.h"
#include "role/rpcg2d_readhiddenroles.h"

// generate by ProtoGen at date: 2017/1/11 16:33:03

RPC_SERVER_IMPLEMETION(RpcC2G_ChangeProfession, ChangeProfessionArg, ChangeProfessionRes)

void RpcC2G_ChangeProfession::OnCall(const ChangeProfessionArg &roArg, ChangeProfessionRes &roRes)
{
	if (!GSConfig::Instance()->IsHallGS() || GSConfig::Instance()->IsCrossGS())
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	///> system open
	if (!role->Get<CRoleSystem>()->IsSystemOpened(SYS_PROFESSION_CHANGE))
	{
		SSInfo<<"system close"<<END;
		roRes.set_errorcode(ERR_SYS_NOTOPEN);
		return;
	}

	if (role->IsBeginChangingPro())
	{
		SSWarn<<"request is send, roleid:"<<role->GetID()<<" pro:"<<roArg.pro()<<END;
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	KKSG::ErrorCode error = ChangeProfessionMgr::Instance()->Check(role, roArg.pro());
	if (error != KKSG::ERR_SUCCESS)
	{
		SSWarn<<"try change pro, roleid:"<<role->GetID()<<" pro:"<<roArg.pro()<<" error:"<<error<<END;
		roRes.set_errorcode(error);
		return;
	}
	RpcG2D_ReadHiddenRoles* rpc = RpcG2D_ReadHiddenRoles::CreateRpc();
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_account(role->GetAccount());
	rpc->m_oArg.set_serverid(role->GetServerId());
	rpc->m_oArg.set_slot(roArg.pro());
	DBLink::Instance()->Send(*rpc);

	role->SetBeginChangingPro(true);

	SSInfo<<"try change pro, roleid:"<<role->GetID()<<" pro:"<<roArg.pro()<<" send to db"<<END;
}

void RpcC2G_ChangeProfession::OnDelayReplyRpc(const ChangeProfessionArg &roArg, ChangeProfessionRes &roRes, const CUserData &roUserData)
{
	UINT32 errorcode = roUserData.m_dwUserData;
	const ReadHiddenRolesRes* res = (ReadHiddenRolesRes*)roUserData.m_pUserPtr;
	if (0 != errorcode || NULL == res)
	{
		SSInfo<<"delay reply, try change pro, sessionid:"<<m_sessionID<<" error:"<<errorcode<<END;
		roRes.set_errorcode(KKSG::ErrorCode(errorcode));
		return;
	}

	KKSG::RoleBriefInfo slotrole;
	if (!slotrole.ParseFromString(res->slotrole()))
	{
		SSWarn<<"parse slotrole error, roleid:"<<res->roleid()<<END;
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	KKSG::HiddenRoles hiddenroles;
	if (!hiddenroles.ParseFromString(res->hiddenroles()))
	{
		SSWarn<<"parse hiddenroles error, roleid:"<<res->roleid()<<END;
		roRes.set_errorcode(ERR_FAILED);
		return;
	}

	Role* role = RoleManager::Instance()->FindByRoleID(res->roleid());
	if (NULL != role)
	{
		if (ChangeProfessionMgr::Instance()->Fill(role->GetID(), !res->slotrole().empty(), slotrole, hiddenroles))
		{
			ChangeProfessionMgr::Instance()->BeginChange(role, res->slot());
		}

		SSInfo<<"delay reply, try change pro, roleid:"<<role->GetID()<<" pro:"<< res->slot()<<" begin"<<END;
	}
	else
	{
		SSWarn<<"not find roleid:"<<res->roleid()<<END;
	}
}
