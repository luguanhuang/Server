#include "pch.h"
#include "teamtranshandler.h"
#include "table/expeditionconfigmgr.h"
#include "role/role.h"
#include "team/teamcommon.h"
#include "team/rpcm2h_teamtranms2ts.h"
#include "define/teamdef.h"
#include "network/teamlink.h"
#include "team/rpcm2g_teamtranms2gs.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "teamsynextradata.h"
#include "team/ptcm2c_teamchangem2cntf.h"
#include "team/ptcm2c_leaveteamm2cntf.h"
#include "common/ptcm2c_mserrornotify.h"
#include "teaminvitemgr.h"
#include "platforminfo/platformfriendmgr.h"
#include "team/rpcm2s_teamrequestplatfreindm2s.h"
#include "network/cslink.h"
#include "team/ptcm2c_teamfulldatam2cntf.h"
#include "team/teamrequest.h"
#include "notice/notice.h"
#include "cross/crosszonestatus.h"
#include "other/matchhandler.h"
#include "chat/ptcm2t_chatforward.h"
#include "match/matchrequest.h"
#include "team/ptcm2h_teamtranms2ts.h"
#include "other/servercrossdatamgr.h"

TeamTransHandler::TeamTransHandler()
{

}

TeamTransHandler::~TeamTransHandler()
{

}

bool TeamTransHandler::NeedInTs(int expID)
{
	auto pMgr = ExpeditionConfigMgr::Instance();
	auto pConf = pMgr->GetExpeditionConf(expID);
	if(NULL == pConf)
	{
		return false;
	}
	//龙本没首通不能跨服
	if(DRAGON_TEAM_SCENE == pConf->Type && !ServerCrossDataSyn::IsDragonSceneFininshed(pMgr->GetExpRandomScene(expID)))
	{
		return false;
	}
	return ExpeditionConfigMgr::Instance()->NeedInTs(expID, MSConfig::Instance()->GetServerID());
}

KKSG::ErrorCode TeamTransHandler::HandleRoleOpCond(const KKSG::TeamOPArg& roArg, CRole* pRole)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	KKSG::ErrorCode ret = KKSG::ERR_SUCCESS;
	switch(roArg.request())
	{
		case KKSG::TEAM_CHANGE_EPXTEAMID:
			{
				if(pRole->HasTeamInTs() && !NeedInTs(roArg.expid()))
				{
					ret = KKSG::ERR_TEAM_CANNOT_FORNOW;
				}
			}
			break;
		default:
			break;
	}
	return ret;
}
//目前进行正向选择
bool TeamTransHandler::NeedHandleToTs(const KKSG::TeamOPArg& roArg, CRole* pRole)
{
	if(NULL == pRole)
	{
		return false;
	}

	auto pExpMgr = ExpeditionConfigMgr::Instance();

	switch(roArg.request())
	{
	case KKSG::TEAM_CREATE:
	case KKSG::TEAM_START_MATCH:
	case KKSG::TEAM_DOWN_MATCH:
	case KKSG::TEAM_STOP_MATCH:
		{
			if(NeedInTs(roArg.expid()))
			{
				return true;
			}
		}
		break;
	case KKSG::TEAM_JOIN:
		{
			if(HTeam::IsTsTeam(roArg.teamid()))
			{
				return true;
			}
		}
		break;
	case KKSG::TEAM_LEAVE:
	case KKSG::TEAM_KICK:
	case KKSG::TEAM_INVITE:
	case KKSG::TEAM_CHANGE_PASSWORD:
	case KKSG::TEAM_TRAHS_LEADER:
	case KKSG::TEAM_GET_FULL_DATA:
	case KKSG::TEAM_PPTLIMIT:
	case KKSG::TEAM_COSTTYPE:
	case KKSG::TEAM_START_BATTLE:
	case KKSG::TEAM_START_BATTLE_AGREE:
	case KKSG::TEAM_START_BATTLE_DISAGREE:
	case KKSG::TEAM_CHANGE_EPXTEAMID:
	case KKSG::TEAM_MEMBER_TYPE:
		{
			if(pRole->HasTeamInTs())
			{
				return true;
			}
		}
		break;
	default:
		return false;
	}

	return false;
}

void TeamTransHandler::HandleRoleOpReqFromC(UINT32 delayID, CRole* pRole, const KKSG::TeamOPArg& roArg, int specialType/* = THSpeType_None*/)
{
	if(NULL == pRole)
	{
		return;
	}

	SSDebug << " delay = " << delayID << " role = " << pRole->GetID() << END;

	RpcM2H_TeamTranMs2Ts* rpc = RpcM2H_TeamTranMs2Ts::CreateRpc();
	auto& data = rpc->m_oArg;
	data.set_type(TEAM_TRANS_OP_REQ);
	data.set_roleid(pRole->GetID());
	data.set_serverid(MSConfig::Instance()->GetServerID());
	data.set_delayid(delayID);
	*data.mutable_c2moparg() = roArg;

	data.set_specialtype(specialType);

	switch(roArg.request())
	{
	case KKSG::TEAM_INVITE:
		{
			pRole->FillTeamInviteData(*data.mutable_teaminvdata());
			if(!roArg.account().empty())//平台
			{
				const auto pInfo = GetPlatUserInfo(pRole->GetID(), roArg.account());
				if(pInfo)
				{
					data.set_para32(pInfo->serverID);
				}
				else
				{
					SSError << " find plat user info failed roleid = " << pRole->GetID() << " account = " << roArg.account() << END;
				}
			}
		}
		break;
	default:
		break;
	}

	TeamLink::Instance()->SendTo(*rpc);
}

void TeamTransHandler::HandleRoleOpResFromTs(UINT32 delayID, UINT64 roleID, const KKSG::TeamOPArg& roArg, const KKSG::TeamOPRes& roRes)
{
	SSDebug << " delay = " << delayID << " role = " << roleID << END;
	
	KKSG::TeamOPM2GRes tmpRes;
	*tmpRes.mutable_c2mres() = roRes;

	CUserData userData;
	userData.m_dwUserData = 1;//用作跨服组队的标志
	userData.m_pUserPtr = (void*)(&tmpRes);

	CRpc::ReplyDelayRpc(delayID, userData);		

	//处理匹配
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		CRole *pRole = CRoleManager::Instance()->GetByRoleID(roleID);
		if(pRole)
		{
			HMatchHandler handler; 
			if(roArg.request() == KKSG::TEAM_START_MATCH)
			{
				handler.Data().expid = roArg.expid();
				int ret = handler.Start(pRole, KKSG::KMT_EXP);
				if(KKSG::ERR_SUCCESS == ret)
				{
					auto pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(roArg.expid());
					if(pConf && HTeam::IsTsFastMatch(pConf->Type))
					{
						pRole->ErrorCodeNtf(KKSG::ERR_TEAM_FASTMATCH_BUFF, true);
					}
				}
				else
				{
					pRole->ErrorCodeNtf(UINT32(ret), true);
				}
			}
			else if(roArg.request() == KKSG::TEAM_STOP_MATCH)
			{
				handler.Stop(pRole, KKSG::KMT_EXP);
			}
		}
		else
		{
			SSError << " find role failed roleid = " << roleID << END;
		}
	}
}

void TeamTransHandler::HandleRoleOpReqFromTs(UINT32 delayID, CRole* pRole, const KKSG::TeamTransData& roArg)
{
	if(NULL == pRole)
	{
		return;
	}

	SSDebug << " delay = " << delayID << " role = " << pRole->GetID() << END;

	RpcM2G_TeamTranMs2Gs* rpc = RpcM2G_TeamTranMs2Gs::CreateRpc();
	auto& data = rpc->m_oArg;

	data = roArg;
	data.set_type(TEAM_TRANS_OP_REQ);
	data.set_delayid(delayID);
	data.set_roleid(pRole->GetID());
	pRole->SendMsgToGS(*rpc);	
}

void TeamTransHandler::HandleRoleOpResFromGs(UINT32 delayID, UINT64 roleID, const KKSG::TeamTransData& roArg, const KKSG::TeamTransRes& roRes)
{
	SSDebug << " delay = " << delayID << " role = " << roleID << END;

	KKSG::TeamTransRes tmpRes;
	tmpRes = roRes;

	//极限情况处理
	if(NULL == CRoleManager::Instance()->GetByRoleID(roleID))
	{
		tmpRes.mutable_c2mopres()->set_result(KKSG::ERR_FAILED);
	}

	CUserData userData;
	userData.m_pUserPtr = (void*)(&tmpRes);

	//做本服配置和跨服配置不相同兼容和防止外挂
	HandleCompatibleConf(roleID, roArg, tmpRes);

	FillRoleDataFromGsBackToTs(roleID, roArg, tmpRes);

	CRpc::ReplyDelayRpc(delayID, userData);			
}

void TeamTransHandler::HandleMatchReqFromTs(UINT32 delayID, CRole* pRole, const KKSG::TeamTransData& roArg)
{
	if(NULL == pRole)
	{
		return;
	}

	SSDebug << " delay = " << delayID << " role = " << pRole->GetID() << END;

	RpcM2G_TeamTranMs2Gs* rpc = RpcM2G_TeamTranMs2Gs::CreateRpc();
	auto& data = rpc->m_oArg;

	data = roArg;
	data.set_type(TEAM_TRANS_TEAM_MATCH);
	data.set_delayid(delayID);
	data.set_roleid(pRole->GetID());
	pRole->SendMsgToGS(*rpc);	
}

void TeamTransHandler::HandleMatchResFromGs(UINT32 delayID, UINT64 roleID, const KKSG::TeamTransData& roArg, const KKSG::TeamTransRes& roRes)
{
	SSDebug << " delay = " << delayID << " role = " << roleID << END;

	KKSG::TeamTransRes tmpRes;
	tmpRes = roRes;

	CUserData userData;
	userData.m_pUserPtr = (void*)(&tmpRes);

	const auto& c2mArg = roArg.matcharg();
	auto& c2mRes = *tmpRes.mutable_matchres();
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);	
	if(pRole && c2mRes.errorcode() == KKSG::ERR_SUCCESS)
	{
		HMatchHandler handler;
		KKSG::ErrorCode ret = handler.RoleCondition(pRole, c2mArg.type(), tmpRes.mutable_matchroledata());
		c2mRes.set_errorcode(ret);

		if(KKSG::ERR_SUCCESS == ret)
		{
			//待整合 TODO
			MatchRequest req(pRole);
			auto tmpC2MArg = roArg.matcharg();
			tmpC2MArg.set_isteam(false);//单人
			req.CheckReportLimit(tmpC2MArg, c2mRes);
		}
	}
	else
	{
		;//不影响
	}

	CRpc::ReplyDelayRpc(delayID, userData);			

}

void TeamTransHandler::HandleTeamChangeFromTs(UINT64 roleID, const KKSG::TeamChanged& changeData)
{
	SSInfo << " role = " << roleID << " team = " << changeData.teambrief().teamid() << END;

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pRole)
	{
		SSError << " find role failed roleid = " << roleID << END;
		return;
	}

	PtcM2C_TeamChangeM2CNtf ptc;
	ptc.m_Data = changeData;	
	pRole->Send(ptc);
}

void TeamTransHandler::HandleLeaveTeamFromTs(UINT64 roleID, KKSG::LeaveTeamType reason)
{
	SSInfo << " role = " << roleID << " reason = " << reason << END;

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pRole)//有可能下线了
	{
		SSInfo << " find role failed roleid = " << roleID << END;
		return;
	}

	PtcM2C_LeaveTeamM2CNtf ptc;
	ptc.m_Data.set_errorno(reason);
	pRole->Send(ptc);
}

void TeamTransHandler::HandleRole2TeamFromTs(UINT64 roleID, int teamID)
{
	SSInfo << " role = " << roleID << " new team = " << teamID << END;

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pRole)//有可能下线了
	{
		SSInfo << " find role failed roleid = " << roleID << END;
		return;
	}

	int oldTeamID = pRole->GetTeamID();
	SSInfo << " role = " << roleID << " old team = " << oldTeamID << END;

	if(0 != oldTeamID)
	{
		if(0 != teamID)
		{
			if(HTeam::IsTsTeam(oldTeamID))
			{
				SSError << " set teamid err repeat ignore" << END;
			}
			else
			{
				SSWarn << " set teamid err repeat replace " << END;
				//离开本服组队
				TeamRequest req(pRole);
				req.LeaveTeam(KKSG::LTT_BY_SELF);

				pRole->SetTeamID(teamID);
			}
		}
		else
		{
			pRole->SetTeamID(teamID);
		}
	}
	else
	{
		if(0 == teamID)
		{
			SSWarn << " set teamid err 0 -> 0" << END;
		}
		else
		{
			pRole->SetTeamID(teamID);
		}
	}
}

void TeamTransHandler::HandleFetchTeamReqFromC(UINT32 delayID, UINT64 roleID, const KKSG::FetchTeamListArg& roArg)
{
	//SSInfo << " delay = " << delayID << " role = " << roleID << END;

	RpcM2H_TeamTranMs2Ts* rpc = RpcM2H_TeamTranMs2Ts::CreateRpc();
	auto& data = rpc->m_oArg;

	*data.mutable_teamlistroarg() = roArg;
	data.set_type(TEAM_TRANS_FETCH_TEAM);
	data.set_delayid(delayID);
	data.set_roleid(roleID);
	data.set_serverid(MSConfig::Instance()->GetServerID());
	TeamLink::Instance()->SendTo(*rpc);	
}

void TeamTransHandler::HandleFetchTeamResFromTs(UINT32 delayID, UINT64 roleID, const KKSG::FetchTeamListRes& roRes)
{
	//SSInfo << " delay = " << delayID << " role = " << roleID << END;

	KKSG::FetchTeamListRes tmpRes;
	tmpRes = roRes;

	CUserData userData;
	userData.m_pUserPtr = (void*)(&tmpRes);

	CRpc::ReplyDelayRpc(delayID, userData);

}

void TeamTransHandler::HandleRoleLogout(UINT64 roleID, int teamID)
{
	RpcM2H_TeamTranMs2Ts* rpc = RpcM2H_TeamTranMs2Ts::CreateRpc();
	auto& data = rpc->m_oArg;

	data.set_type(TEAM_TRANS_ROLELOGOUT);
	data.set_roleid(roleID);

	TeamLink::Instance()->SendTo(*rpc);	
}

void TeamTransHandler::HandleRoleAckToTs(CRole* pRole, TeamInviteRec& rec, bool accept)
{
	if(NULL == pRole)
	{
		return;
	}
	if(!accept)
	{
		//拒绝(目前没有拒绝)
		
		return;
	}

	if(pRole->HasTeam())//已经有队伍(已经判断了)
	{
		SSWarn << " role has team role = " << pRole->GetID() << " inv = " << rec.GetInviteID() << END;
		return;
	}

	//伪装成请求加入
	KKSG::TeamOPArg c2mArg;
	c2mArg.set_request(KKSG::TEAM_JOIN);
	c2mArg.set_teamid(rec.GetTeamID());
	HandleRoleOpReqFromC(0, pRole, c2mArg, THSpeType_Ac2Join);
}

void TeamTransHandler::HandleErrorCodeFromTs(UINT64 roleID, KKSG::ErrorCode errcode)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pRole)
	{
		return;
	}

	if(KKSG::ERR_SUCCESS == errcode)
	{
		return;
	}

	pRole->ErrorCodeNtf(errcode, true);
}

void TeamTransHandler::HandleErrorCodeToTs(UINT64 roleID, KKSG::ErrorCode errcode)
{

}

void TeamTransHandler::HandleInviteDataFromTs(const KKSG::TeamTransData& data)
{
	const std::string& account = data.account();
	auto invData = data.teaminvdata();//浪费

	if(account.empty())
	{
		TeamInviteMgr::Instance()->MakeInvite(invData);
		return;
	}

	//平台 
	if(!GetPlatUserInfo(invData.invfromroleid(), account))
	{
		return;
	}

	RpcM2S_TeamRequestPlatFreindM2S* rpc = RpcM2S_TeamRequestPlatFreindM2S::CreateRpc();
	rpc->m_oArg.mutable_teaminviteinfo()->CopyFrom(invData);
	rpc->m_oArg.mutable_destinfo()->set_openid(account);
	CSLink::Instance()->SendTo(*rpc);
}

void TeamTransHandler::HandleCompatibleConf(UINT64 roleID, const KKSG::TeamTransData& roArg, KKSG::TeamTransRes& tmpRes)
{
	if(roArg.type() != TEAM_TRANS_OP_REQ)
	{
		return;
	}

	if(tmpRes.c2mopres().result() != KKSG::ERR_SUCCESS)
	{
		return;
	}

	const auto& c2mArg = roArg.c2moparg();
	if(c2mArg.request() == KKSG::TEAM_JOIN)
	{
		if(!NeedInTs(roArg.teamalldata().expid()))
		{
			SSWarn << " role try join cross team roleid = " << roleID << " teamid = " << roArg.teamalldata().teamid() << END;
			tmpRes.mutable_c2mopres()->set_result(KKSG::ERR_FAILED);
			return;
		}
	}
}

bool TeamTransHandler::FillRoleMemberData(UINT64 roleID, KKSG::TeamTransRes& tmpRes)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(pRole)
	{
		auto pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID);
		if(pSummary)
		{
			pSummary->SaveToTeamMember(tmpRes.mutable_memberbrief());
		}
		else
		{
			return false;
		}
		HTeamSynExtraData extraData;
		extraData.FillExtraData(pRole, *tmpRes.mutable_teamsynextradata());
	}
	else
	{
		return false;
	}
	return true;
}

void TeamTransHandler::FillRoleDataFromGsBackToTs(UINT64 roleID, const KKSG::TeamTransData& roArg, KKSG::TeamTransRes& tmpRes)
{
	if(roArg.type() != TEAM_TRANS_OP_REQ)
	{
		return;
	}

	if(tmpRes.c2mopres().result() != KKSG::ERR_SUCCESS)
	{
		return;
	}

	const auto& c2mArg = roArg.c2moparg();
	switch(c2mArg.request())
	{
	case KKSG::TEAM_CREATE:
	case KKSG::TEAM_JOIN:
		{
			if(!FillRoleMemberData(roleID, tmpRes))
			{
				tmpRes.mutable_c2mopres()->set_result(KKSG::ERR_FAILED);
			}
		}
		break;
	//case KKSG::TEAM_INVITE:
	//	{
	//		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	//		if(pRole)
	//		{
	//			pRole->FillTeamInviteData(*tmpRes.mutable_invdata());	
	//		}
	//		else
	//		{
	//			tmpRes.mutable_c2mopres()->set_result(KKSG::ERR_FAILED);
	//		}
	//	}
	//	break;
	default:
		break;
	}

}

void TeamTransHandler::HandleTeamFullData(CRole* pRole, const KKSG::TeamFullDataNtf& fullData)
{
	if(NULL == pRole)
	{
		return;
	}

	PtcM2C_TeamFullDataM2CNtf ptc;	
	ptc.m_Data = fullData;
	pRole->Send(ptc);

	pRole->MatchTypeNtf();//匹配
}

void TeamTransHandler::HandlerGsRegister(UINT32 gsLine)
{
	RpcM2H_TeamTranMs2Ts* rpc = RpcM2H_TeamTranMs2Ts::CreateRpc();
	auto& data = rpc->m_oArg;

	data.set_type(TEAM_TRANS_GS_REGISTER);
	data.set_serverid(MSConfig::Instance()->GetServerID());
	data.set_gsline(gsLine);

	if(TeamLink::Instance()->IsConnected())
	{
		TeamLink::Instance()->SendTo(*rpc);	
	}
}

void TeamTransHandler::HandleInvRefuseToTs(CRole* pRole, UINT64 roleID)
{
	if(NULL == pRole)
	{
		return;
	}

	auto* pInfo = CPlatformFriendMgr::Instance()->GetPlatSelfInfo(pRole->GetAccount());
	if(NULL == pInfo)
	{
		return;
	}

	RpcM2H_TeamTranMs2Ts* rpc = RpcM2H_TeamTranMs2Ts::CreateRpc();
	rpc->m_oArg.set_type(TEAM_TRANS_INV_REFUSE);
	rpc->m_oArg.set_roleid(roleID);
	rpc->m_oArg.set_account(pInfo->nickname);
	TeamLink::Instance()->SendTo(*rpc);
}

void TeamTransHandler::HandleInvRefuseFromTs(const std::string& name, UINT64 roleID)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pRole)
	{
		return;
	}
	
	TeamInviteMgr::Instance()->RefuseNtf(roleID, name);

	////notice
	//Notice notice(NoticeTeamInvitePlatFriendRefuse);
	//notice.Replace("R", name);
	//notice.SendRole(roleID);
}

void TeamTransHandler::HandleMatchReqFromC(UINT32 delayID, CRole* pRole, const KKSG::KMatchCommonArg& roArg)
{
	if(NULL == pRole)
	{
		return;
	}
		
	RpcM2H_TeamTranMs2Ts* rpc = RpcM2H_TeamTranMs2Ts::CreateRpc();
	rpc->m_oArg.set_type(TEAM_TRANS_TEAM_MATCH);
	rpc->m_oArg.set_roleid(pRole->GetID());
	rpc->m_oArg.set_delayid(delayID);
	*rpc->m_oArg.mutable_matcharg() = roArg;
	TeamLink::Instance()->SendTo(*rpc);
}

void TeamTransHandler::HandleMatchResFromTs(UINT32 delayID, const KKSG::KMatchCommonRes& roRes)
{
	KKSG::KMatchCommonRes tmpRes;
	tmpRes = roRes;

	CUserData userData;
	userData.m_pUserPtr = (void*)(&tmpRes);

	CRpc::ReplyDelayRpc(delayID, userData);
}

void TeamTransHandler::SynTeamExtraInfoToTs(UINT64 roleID)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pRole)
	{
		return;
	}
	if(!pRole->HasTeamInTs())
	{
		return;
	}

	PtcM2H_TeamTranMs2Ts ptc;
	ptc.m_Data.set_type(TEAM_TRANS_SYN_EXTRAINFO);

	HTeamSynExtraData extraData;
	auto& data = *ptc.m_Data.add_tsynextradata();
	extraData.FillExtraData(pRole, data);	
	
	TeamLink::Instance()->SendTo(ptc);
}

void TeamTransHandler::HandleChatToTs(UINT64 roleID, const KKSG::ChatInfo& chatInfo)
{
	RpcM2H_TeamTranMs2Ts* rpc = RpcM2H_TeamTranMs2Ts::CreateRpc();
	rpc->m_oArg.set_type(TEAM_TRANS_CHAT);
	rpc->m_oArg.set_roleid(roleID);
	*rpc->m_oArg.mutable_chatinfo() = chatInfo;
	TeamLink::Instance()->SendTo(*rpc);
}

void TeamTransHandler::HandleChatFromTs(UINT64 roleID, const KKSG::ChatInfo& chatInfo)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(pRole)
	{
		PtcM2T_chatforward ptc;
		*ptc.m_Data.mutable_chatinfo() = chatInfo;
		pRole->Send(ptc);
	}
}

bool TeamTransHandler::HandleRoleMatchToTs(CRole* pRole, int expID, bool start)
{
	if(NULL == pRole)
	{
		return false;
	}
	if(!TeamLink::Instance()->IsConnected())
	{
		return false;
	}
	RpcM2H_TeamTranMs2Ts* rpc = RpcM2H_TeamTranMs2Ts::CreateRpc();
	rpc->m_oArg.set_type(TEAM_TRANS_ROLE_MATCH);
	rpc->m_oArg.set_roleid(pRole->GetID());

	auto& matchData = *rpc->m_oArg.mutable_matcharg();
	matchData.set_type(KKSG::KMT_EXP);
	if(start)
	{
		matchData.set_op(KKSG::KMATCH_OP_START);
	}
	else
	{
		matchData.set_op(KKSG::KMATCH_OP_STOP);
	}
	matchData.set_isteam(false);

	auto& kunit = *rpc->m_oArg.mutable_kmatchunit();
	kunit.set_serverid(MSConfig::Instance()->GetServerID());	
	kunit.set_roleid(pRole->GetID());

	auto& krole = *kunit.add_roles();
	krole.set_serverid(MSConfig::Instance()->GetServerID());
	krole.set_roleid(pRole->GetID());

	if(start)
	{
		auto pExpConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
		if(NULL == pExpConf)
		{
			return false;
		}

		auto& expData = *krole.mutable_expdata();
		expData.set_expid(expID);
		expData.set_rolecount(pExpConf->PlayerNumber);
		expData.set_name(pRole->GetName());
	}

	TeamLink::Instance()->SendTo(*rpc);

    return true;
}

void TeamTransHandler::HandleRoleMatchFromTs(UINT32 delayID, CRole* pRole, const KKSG::TeamTransData& roArg)
{
	if(NULL == pRole)
	{
		return;
	}

	SSDebug << " delay = " << delayID << " role = " << pRole->GetID() << END;

	RpcM2G_TeamTranMs2Gs* rpc = RpcM2G_TeamTranMs2Gs::CreateRpc();
	auto& data = rpc->m_oArg;

	data = roArg;
	data.set_type(TEAM_TRANS_ROLE_MATCH);
	data.set_delayid(delayID);
	data.set_roleid(pRole->GetID());
	pRole->SendMsgToGS(*rpc);	

}

void TeamTransHandler::HandleRoleMatchFromGs(UINT32 delayID, UINT64 roleID, const KKSG::TeamTransRes& roRes)
{
	SSDebug << " delay = " << delayID << " role = " << roleID << END;

	KKSG::TeamTransRes tmpRes;
	tmpRes = roRes;

	//极限情况处理
	if(NULL == CRoleManager::Instance()->GetByRoleID(roleID))
	{
		tmpRes.mutable_c2mopres()->set_result(KKSG::ERR_FAILED);
	}

	if(tmpRes.c2mopres().result() == KKSG::ERR_SUCCESS)
	{
		if(!FillRoleMemberData(roleID, tmpRes))
		{
			tmpRes.mutable_c2mopres()->set_result(KKSG::ERR_FAILED);
		}
	}

	CUserData userData;
	userData.m_pUserPtr = (void*)(&tmpRes);

	CRpc::ReplyDelayRpc(delayID, userData);			
}

const PlatUserInfo* TeamTransHandler::GetPlatUserInfo(UINT64 roleID, const std::string& account)
{
	CRole* inviteFrom = CRoleManager::Instance()->GetByRoleID(roleID);
	if (!inviteFrom)
	{
		return NULL;
	}
	PlatFriendList* list = CPlatformFriendMgr::Instance()->GetPlatFriendListByOpenID(inviteFrom->GetAccount());
	if (!list)
	{
		return NULL;
	}

	return list->GetPlatFriendUserInfo(account);
}

void TeamTransHandler::HandleTransArgFromTsToGs(UINT32 delayID, CRole* pRole, const KKSG::TeamTransData& roArg)
{
	if(NULL == pRole)
	{
		return;
	}
	RpcM2G_TeamTranMs2Gs* rpc = RpcM2G_TeamTranMs2Gs::CreateRpc();
	auto& data = rpc->m_oArg;

	data = roArg;
	data.set_delayid(delayID);
	pRole->SendMsgToGS(*rpc);	
}

void TeamTransHandler::HandleTransResFromGsToTs(UINT32 delayID, const KKSG::TeamTransRes& roRes)
{
	CUserData userData;
	userData.m_pUserPtr = (void*)(&roRes);

	CRpc::ReplyDelayRpc(delayID, userData);			
}
