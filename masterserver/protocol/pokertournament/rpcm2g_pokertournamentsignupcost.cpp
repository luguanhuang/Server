#include "pch.h"
#include "pokertournament/rpcm2g_pokertournamentsignupcost.h"
#include "role/role.h"
#include "role/rolemanager.h"
//#include "guild/ptcc2m_guildcardmatchreq.h"
#include "guild/ptcm2c_guildcardmatchntf.h"
#include "pokertournament/cardmatchmgr.h"
// generate by ProtoGen at date: 2016/12/12 11:11:13

RPC_CLIENT_IMPLEMETION(RpcM2G_PokerTournamentSignUpCost, ConsumeItemsArg, PokerTournamentSignUpCostRes)

void RpcM2G_PokerTournamentSignUpCost::OnReply(const ConsumeItemsArg &roArg, const PokerTournamentSignUpCostRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.role_id());
	if (NULL == role)
	{
		SSWarn<<"role is NULL, sessionid:"<<roArg.role_id()<<END;
		return;
	}
	LogDebug("roRes.result() : %d",roRes.result()); 
	PtcM2C_GuildCardMatchNtf ntf;	
	ntf.m_Data.set_match_type(1);
	ntf.m_Data.set_op(KKSG::CardMatch_SignUp);
	
	///> GS条件没过
	if (KKSG::ERR_SUCCESS != roRes.result())
	{
		ntf.m_Data.set_errorcode(roRes.result());
		role->Send(ntf);
		return;
	}
	// 添加报名，等待比赛开始
	CardMatchMgr::Instance()->AddRoleTOCardMatch(role->GetID());
	bool exist = CardMatchMgr::Instance()->CheckRoleExist(role->GetID());
	ntf.m_Data.set_sign_up(exist);	
	ntf.m_Data.set_errorcode(roRes.result());
	UINT32 sign_up_num = CardMatchMgr::Instance()->GetSignUpNum(role->GetID());
	ntf.m_Data.set_sign_up_num(sign_up_num);

	role->Send(ntf);
}

void RpcM2G_PokerTournamentSignUpCost::OnTimeout(const ConsumeItemsArg &roArg, const CUserData &roUserData)
{
}
