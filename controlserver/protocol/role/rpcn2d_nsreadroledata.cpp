#include "pch.h"
#include "role/rpcn2d_nsreadroledata.h"
#include "scene/rpcn2g_enterscene.h"
#include "network/gslink.h"
#include "account/accountsessionmgr.h"
#include "commondef.h"
#include "scene/gsmanager.h"

// generate by ProtoGen at date: 2016/11/3 1:34:54

RPC_CLIENT_IMPLEMETION(RpcN2D_NsReadRoleData, ReadRoleDataArg, ReadRoleDataRes)

void RpcN2D_NsReadRoleData::OnReply(const ReadRoleDataArg &roArg, const ReadRoleDataRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		if (roArg.readtype() == KKSG::ROLE_DATA_READ_IDIP)
		{
			//CIdipProcess::Instance()->OnAsyncReply(roArg.index(), oUser);
		}
		else
		{
			CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
		}
		return;
	}

	const RoleAllInfo& oInfo = roRes.roledata();
	if (roArg.readtype() == KKSG::ROLE_DATA_READ_IDIP)
	{
		///>读取到数据后然后修改数据
		/*CUserData oData;
		oData.m_pUserPtr = (void*)&oInfo;
		//如果param存在且大于0，则是读取数据,否则是读取数据后修改数据;
		if (roArg.has_param() && roArg.param() > 0)
		{
			CIdipProcess::Instance()->OnAsyncReply(roArg.index(), oData);
		}
		else
		{
			if (!CIdipProcess::Instance()->ModifyData(roArg.index(), oData))
			{
				oData.m_dwUserData = IDIP_ERR_UNKNOWN;
				CIdipProcess::Instance()->OnAsyncReply(roArg.index(), oData);
			}
		}*/
	}
	else
	{
		//此时要检测主城GS是否还在
		if(CGsManager::Instance()->GetGsInfo(MAIN_HALL_GS_LINE) == NULL)
		{
			LogError("No main hall game server, can not login on after read role data, account [%s], role [%llu]", oInfo.brief().accountid().c_str(), oInfo.brief().roleid());
			oUser.m_dwUserData = (UINT32)KKSG::ERR_GS_UNREADY;
			CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
			return;
		}

		CRole* poRole = CAccountSessionMgr::Instance()->LoginRole(m_sessionID, oInfo);
		if(poRole == NULL)
		{
			LogError("Role [%llu] login failed with session %llu, session maybe closed earlier", oInfo.brief().roleid(), m_sessionID);
			oUser.m_dwUserData = (UINT32)KKSG::ERR_FAILED;
			CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
			return;
		}

		RpcN2G_EnterScene* rpc = RpcN2G_EnterScene::CreateRpc();
		rpc->m_sessionID = m_sessionID;
		rpc->m_oArg.set_mapid(0);
		rpc->m_oArg.set_type(KKSG::ENTER_SCENE_SELECT_ROLE);
		rpc->m_oArg.set_rpcid(roArg.rpcid());
		rpc->m_oArg.add_rolesessionlist(m_sessionID);
		RoleAllInfo* poData = rpc->m_oArg.add_roledatalist();
		*poData = oInfo;
		CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
		if(poAccount)
		{
			KKSG::ClientInfo* poCliInfo = rpc->m_oArg.add_cliinfolist();
			poCliInfo->CopyFrom(poAccount->GetCliConf());
			rpc->m_oArg.add_rolenum(poAccount->GetProfessionCount());
			rpc->m_oArg.add_account_backflow(poAccount->IsAccountBackFlow());
		}

		UINT32 dwGsLine = MAIN_HALL_GS_LINE;
		if(!GSLink::Instance()->SendToLine(dwGsLine, *rpc))
		{
			LogError("Send rpc to game server_1 failed, can not login on after read role data, account [%s], role [%llu]", oInfo.brief().accountid().c_str(), oInfo.brief().roleid());
			CAccountSessionMgr::Instance()->LogoutRole(m_sessionID);
			oUser.m_dwUserData = (UINT32)KKSG::ERR_FAILED;
			CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
			return;
		}

		poRole->SetGsLine(dwGsLine);
		poRole->SetState(ROLE_STATE_ENTER_SCENE);
	}
}

void RpcN2D_NsReadRoleData::OnTimeout(const ReadRoleDataArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	if (roArg.readtype() == KKSG::ROLE_DATA_READ_IDIP)
	{
		//CIdipProcess::Instance()->OnAsyncReply(roArg.index(), oUser);
	}
	else
	{
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
	}
}
