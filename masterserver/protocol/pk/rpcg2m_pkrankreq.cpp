#include "pch.h"
#include "pk/rpcg2m_pkrankreq.h"
#include "pk/pkmgr.h"
#include "ranklist/ranklist.h"
#include "role/rolemanager.h"
#include "reportdata/reportdata_def.h"
#include "loghelper/reportdatamgr.h"

// generate by ProtoGen at date: 2016/8/25 15:22:15

RPC_SERVER_IMPLEMETION(RpcG2M_PkRankReq, QMHRPkArg, QMHRPkRes)

void RpcG2M_PkRankReq::OnCall(const QMHRPkArg &roArg, QMHRPkRes &roRes)
{
	PkMgr::Instance()->HandlerRankUpdateDelay(roArg, roRes);
	if (roArg.type() == KKSG::PK_1v1 && roRes.allrank() != INVALID_RANK)//1v1天梯赛
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.rankdata().roleid());
		if (pRole)
		{
			CReportDataMgr::Instance()->Common(pRole, Tx_Reportdata_PkRank, ToString(roRes.allrank()));
		}
	}

}

void RpcG2M_PkRankReq::OnDelayReplyRpc(const QMHRPkArg &roArg, QMHRPkRes &roRes, const CUserData &roUserData)
{
}
