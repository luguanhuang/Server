#include "pch.h"
#include "guild/rpcg2m_opguildreqdata.h"

// generate by ProtoGen at date: 2016/9/9 9:56:30

RPC_CLIENT_IMPLEMETION(RpcG2M_OpGuildReqData, OpGuildReqDataArg, OpGuildReqDataRes)

void RpcG2M_OpGuildReqData::OnReply(const OpGuildReqDataArg &roArg, const OpGuildReqDataRes &roRes, const CUserData &roUserData)
{
	switch(roArg.optype())
	{
	case (KKSG::STUDY_SKILL):  // 公会技能
		{
			CUserData oUser;
			oUser.m_dwUserData = (UINT32)roRes.errorcode();
			UINT32 skillLvl = roRes.guildskilllvl(); 
			oUser.m_pUserPtr = (void*)&skillLvl;
			CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
		}
		break;
	case (KKSG::GUILD_DARE_INFO): // 公会挑战
		{
			//CUserData oUser;
			//const KKSG::GuildDareData& data = roRes.guilddaredata();
			//oUser.m_pUserPtr = (void*)&roRes.guilddaredata();
			//CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
		}
		break;
	}
}

void RpcG2M_OpGuildReqData::OnTimeout(const OpGuildReqDataArg &roArg, const CUserData &roUserData)
{
}
