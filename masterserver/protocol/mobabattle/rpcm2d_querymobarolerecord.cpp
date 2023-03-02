#include "pch.h"
#include "mobabattle/rpcm2d_querymobarolerecord.h"
#include "mobabattle/mobarecordmgr.h"

// generate by ProtoGen at date: 2017/6/17 19:25:04

RPC_CLIENT_IMPLEMETION(RpcM2D_QueryMobaRoleRecord, QueryMobaRoleRecordArg, QueryMobaRoleRecordRes)

void RpcM2D_QueryMobaRoleRecord::OnReply(const QueryMobaRoleRecordArg &roArg, const QueryMobaRoleRecordRes &roRes, const CUserData &roUserData)
{
	MobaRecordMgr::Instance()->OnGetMobaRecordFromDB(roArg.roleid(), m_oRes);
}

void RpcM2D_QueryMobaRoleRecord::OnTimeout(const QueryMobaRoleRecordArg &roArg, const CUserData &roUserData)
{
	m_oRes.set_errorcode(KKSG::ERR_TIMEOUT);
	MobaRecordMgr::Instance()->OnGetMobaRecordFromDB(roArg.roleid(), m_oRes);
}
