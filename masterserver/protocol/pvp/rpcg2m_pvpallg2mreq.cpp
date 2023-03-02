#include "pch.h"
#include "pvp/rpcg2m_pvpallg2mreq.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "pvp/pvpmgr.h"
#include "config.h"
#include "role/rolesummarymgr.h"
#include "network/worldlink.h"

// generate by ProtoGen at date: 2016/8/19 19:50:14

RPC_SERVER_IMPLEMETION(RpcG2M_PvpAllG2MReq, PvpG2MArg, PvpG2MRes)

void RpcG2M_PvpAllG2MReq::OnCall(const PvpG2MArg &roArg, PvpG2MRes &roRes)
{
	const PvpArg& c2gArg = roArg.c2garg();
	PvpRes& c2gRes = *roRes.mutable_c2gres();

	c2gRes.set_reqtype(c2gArg.type());
	c2gRes.set_err(ERR_SUCCESS);

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		c2gRes.set_err(KKSG::ERR_INVALID_REQUEST);
		return ;
	}

	switch(c2gArg.type())
	{
	case KKSG::PVP_REQ_BASE_DATA:
		{
			KKSG::PvpBaseData* data = c2gRes.mutable_basedata();
			data->set_matchingcount(PvpMgr::Instance()->GetInMatchCount(pRole));	
		}
		break;
	default:
		break;
	}
}

void RpcG2M_PvpAllG2MReq::OnDelayReplyRpc(const PvpG2MArg &roArg, PvpG2MRes &roRes, const CUserData &roUserData)
{
}
