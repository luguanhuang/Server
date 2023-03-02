#include "pch.h"
#include "role/rpcm2d_readroledata.h"
//#include "scene/rpcm2g_enterscene.h"
#include "network/gslink.h"
#include "commondef.h"
#include "idip/idipproc.h"
#include "idip/idip.h"
#include "idip/idiphandler.h"
#include "util/jsonutil.h"
#include "chat/chatmgr.h"
#include "friend/friendop.h"

// generate by ProtoGen at date: 2016/7/10 14:06:11

RPC_CLIENT_IMPLEMETION(RpcM2D_ReadRoleData, ReadRoleDataArg, ReadRoleDataRes)

void RpcM2D_ReadRoleData::OnReply(const ReadRoleDataArg &roArg, const ReadRoleDataRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		if (roArg.readtype() == KKSG::ROLE_DATA_READ_IDIP)
		{
			CIdipProcess::Instance()->OnAsyncReply(roArg.index(), oUser);
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
		CUserData oData;
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
		}
	}
	else
	{
		/*RpcM2G_EnterScene* rpc = RpcM2G_EnterScene::CreateRpc();
		rpc->m_sessionID = m_sessionID;
		rpc->m_oArg.set_mapid(0);
		rpc->m_oArg.set_type(KKSG::ENTER_SCENE_SELECT_ROLE);
		rpc->m_oArg.set_rpcid(roArg.rpcid());
		rpc->m_oArg.add_rolesessionlist(m_sessionID);
		RoleAllInfo* poData = rpc->m_oArg.add_roledatalist();
		*poData = oInfo;

		KKSG::ClientInfo* poCliInfo = rpc->m_oArg.add_cliinfolist();
		CAccountSessionMgr::Instance()->PackSessionClientInfo(m_sessionID, *poCliInfo);

		FriendList* friends = rpc->m_oArg.add_friendlist();
		FriendOp op(poData->brief().roleid());
		op.FillFriendList(friends);

		UINT32 dwGsLine = MAIN_HALL_GS_LINE;
		GSLink::Instance()->SendToLine(dwGsLine, *rpc);

		CRole* poRole = CAccountSessionMgr::Instance()->LoginRole(m_sessionID, oInfo);
		if(poRole == NULL)
		{
			LogError("Role [%llu] login failed with session %llu", oInfo.brief().roleid(), m_sessionID);
			CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
			return;
		}

		poRole->SetGsLine(dwGsLine);
		poRole->SetState(ROLE_STATE_ENTER_SCENE);*/
	}
}

void RpcM2D_ReadRoleData::OnTimeout(const ReadRoleDataArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	if (roArg.readtype() == KKSG::ROLE_DATA_READ_IDIP)
	{
		CIdipProcess::Instance()->OnAsyncReply(roArg.index(), oUser);
	}
	else
	{
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
	}
}
