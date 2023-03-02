#include "pch.h"
#include "activity/rpcc2g_mulactivityreq.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/MultActivityMgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guilddef.h"
#include "gamelogic/QAMgr.h"
#include "guild/guilddef.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/QARecord.h"
#include "network/mslink.h"
#include "activity/rpcg2m_getmulactopeninfo.h"

// generate by ProtoGen at date: 2016/7/9 14:32:04

RPC_SERVER_IMPLEMETION(RpcC2G_MulActivityReq, MulActivityArg, MulActivityRes)

void RpcC2G_MulActivityReq::OnCall(const MulActivityArg &roArg, MulActivityRes &roRes)
{
	roRes.set_errcode(KKSG::ERR_SUCCESS);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_myguildlevel(0);
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(pRole->GetID());

	// 没有公会的直接返回了
	if(!guild)
	{
		MultActivityMgr::Instance()->FillAllMulActivityInfo(pRole, roArg.acid(), roRes);
		return;
	}

	// 有公会的去查询一下公会红包的发放状态
	roRes.set_myguildlevel(guild->GetGuildLvl());

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_errcode(KKSG::ERR_FAILED);
		return;
	}
	RpcG2M_GetMulActOpenInfo* rpc = RpcG2M_GetMulActOpenInfo::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendToMS(*rpc);
}

void RpcC2G_MulActivityReq::OnDelayReplyRpc(const MulActivityArg &roArg, MulActivityRes &roRes, const CUserData &roUserData)
{
	roRes.set_errcode(KKSG::ERR_FAILED);
	if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS || roUserData.m_pUserPtr == NULL) return;
	KKSG::GetMulActOpenInfoRes* res = (KKSG::GetMulActOpenInfoRes*)(roUserData.m_pUserPtr);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is out, session id:"<<m_sessionID<<END;
		return;
	}
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if(guild)
	{
		roRes.set_myguildlevel(guild->GetGuildLvl());
	}
	roRes.set_errcode(KKSG::ERR_SUCCESS);
	MultActivityMgr::Instance()->FillAllMulActivityInfo(pRole, roArg.acid(), roRes, res);
}
