#include "pch.h"
#include "matchhandler.h"
#include "role/role.h"
#include "team/teamrequest.h"
#include "pb/enum.pb.h"
#include "team/team.h"
#include "team/teammgr.h"
#include "team/teammatchmgr.h"
#include "pvp/pvpmgr.h"
#include "pk/pkmgr.h"
#include "leaguebattle/leaguebattlematchmgr.h"
#include "herobattle/herobattlemgr.h"
#include "role/rolemanager.h"
#include "table/globalconfig.h"
#include "skycraft/skycraftmatchmgr.h"
#include "team/teamcommon.h"
#include "match/rpcm2k_kmatchreqm2k.h"
#include "network/worldlink.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "team/teamforceop.h"
#include "network/teamlink.h"
#include "cross/crosszonestatus.h"
#include "match/matchmgr.h"
#include "mobabattle/mobabattlemgr.h"
#include "custombattle/custombattlemgr.h"
#include "custombattle/custombattlerole.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattleconfig.h"
#include "mobabattle/mobarecordmgr.h"
#include "team/teammember.h"
#include "table/expeditionconfigmgr.h"
#include "team/teamtranshandler.h"

std::unordered_map<int, HMatchBase*> HMatchHandler::m_allMatch;

HMatchExp HMatchExp::GlobalHMatchExp;
HMatchPvp HMatchPvp::GlobalHMatchPvp;
HMatchHero HMatchHero::GlobalHMatchHero;
HMatchMoba HMatchMoba::GlobalHMatchMoba;
HMatchPk HMatchPk::globalHMatchPk;
HMatchPkTwo HMatchPkTwo::GlobalHMatchPkTwo;
HMatchCustomPkTwo HMatchCustomPkTwo::GlobalHMatchCustomPkTwo;
HMatchLeague HMatchLeague::GlobalHMatchLeague;
HMatchSkyCraft HMatchSkyCraft::GlobalHMatchSkyCraft;
HMatchWeekendAct HMatchWeekendAct::GlobalHMatchWeekendAct;
HMatchSurvive HMatchSurvive::GlobalHMatchSurvive;

HMatchBase::HMatchBase(int type)
{
	HMatchHandler::m_allMatch[type] = this;
}

int HMatchBase::MatchSendToWorld(CRole* pRole, KKSG::KMatchCommonArg& roArg, KKSG::KMatchRole* krole/* = NULL*/)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	if(!CrossZoneStatus::Instance()->IsCrossZoneUsable())
	{
		SSWarn << " cross server unusable role = " <<  pRole->GetID() << END;
		return KKSG::ERR_SERVER_UNNORMAL;
	}
	auto pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	if(NULL == pSummary)
	{
		SSError << " find summary failed role = " << pRole->GetID() << END;
		return KKSG::ERR_FAILED;
	}
	if(roArg.type() == KKSG::KMT_NONE)
	{
		SSError << " req type err type = " << roArg.type() << " op = " << roArg.op() << END;
		return KKSG::ERR_FAILED;
	}

	RpcM2K_KMatchReqm2k* rpc = RpcM2K_KMatchReqm2k::CreateRpc();	
	auto& data = rpc->m_oArg;

	*data.mutable_matcharg() = roArg;//玩家请求信息

	//必要信息
	auto& kunit = *data.mutable_kunit();
	kunit.set_serverid(MSConfig::Instance()->GetServerID());
	kunit.set_roleid(pRole->GetID());

	if(krole)//额外信息已有
	{
		//必要信息
		krole->set_serverid(MSConfig::Instance()->GetServerID());
		krole->set_roleid(pRole->GetID());
		//可能用到的信息
		pSummary->FillRolePvpInfo(*krole->mutable_pvpinfo());

		*kunit.add_roles() = *krole;
	}
	else if(roArg.op() == KKSG::KMATCH_OP_START)
	{
		SSError << " start match role data null roleid = " << pRole->GetID() << END;
		return KKSG::ERR_FAILED;
	}

	WorldLink::Instance()->SendTo(*rpc);

	return KKSG::ERR_SUCCESS;

}

int HMatchBase::Start(CRole* pRole, HMatchHandler* pHandler)
{
	//默认调用
	if(pHandler->Data().matchData.roledata_size() <= 0)//默认会有一个玩家数据(即便是空的)
	{
		SSError << "  match role data size = 0 roleid = " << pRole->GetID() << END;
		return KKSG::ERR_FAILED;
	}
	auto data = pHandler->Data().matchData.roledata(0);
	return MatchSendToWorld(pRole, pHandler->Data().matchArg, &data);
}

int HMatchBase::Start(Team* pTeam, HMatchHandler* pHandler)
{
	SSWarn << " should not run here team = " << pTeam->GetID() << END;
	return KKSG::ERR_FAILED;
}

void HMatchBase::Stop(CRole* pRole, HMatchHandler* pHandler)
{
	//默认调用
	MatchSendToWorld(pRole, pHandler->Data().matchArg);
}

void HMatchBase::Stop(Team* pTeam, HMatchHandler* pHandler)
{
	SSWarn << " should not run here team = " << pTeam->GetID() << END;
}

KKSG::ErrorCode HMatchBase::MatchCondition(CRole* pRole)
{
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode HMatchBase::FillMatchData(CRole* pRole, KKSG::KMatchRole* krole)
{
	return KKSG::ERR_SUCCESS;
}

int HMatchHandler::Start(CRole* pRole, int type)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}

	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " type = " << type << END;

	if(KKSG::KMT_NONE == type)
	{
		return KKSG::ERR_FAILED;
	}

	if(pRole->GetMatchType() == type)
	{
		return KKSG::ERR_REQUEST_REPEAT;
	}

	int condret = RoleMatchCond(pRole, type);
	if(KKSG::ERR_SUCCESS != condret)
	{
		return condret;
	}

	//如果有组队 则退出队伍
	if(pRole->GetTeamID())
	{
		TeamForceOp op;
		op.LeaveTeam(pRole, KKSG::LTT_BY_SELF);
	}

	CheckRoleStop(pRole);//取消原有匹配(不管新的匹配是否成功)

	auto handler = GetMatchHandler(type);	
	if(NULL == handler)
	{
		return KKSG::ERR_FAILED;
	}

	int ret = handler->Start(pRole, this);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	pRole->SetMatchType(type);

	///> 全民pk取消匹配
	CustomBattleMsMatcher::Instance()->Cancel(pRole->GetID());

	return KKSG::ERR_SUCCESS;
}

int HMatchHandler::Start(Team* pTeam, int type)
{
	if(NULL == pTeam)
	{
		return KKSG::ERR_FAILED;
	}

	if(KKSG::KMT_NONE == type)
	{
		return KKSG::ERR_FAILED;
	}

	SSDebug << __FUNCTION__ << " team = " << pTeam->GetID() << " type = " << type << END;

	if(pTeam->GetMatchType() == type)
	{
		return KKSG::ERR_REQUEST_REPEAT;
	}

	std::vector<UINT64> roleIDs;
	pTeam->GetRoleIDs(roleIDs);
	for(size_t i = 0; i < roleIDs.size(); ++i)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleIDs[i]);
		if(NULL == pRole || pRole->IsInBattle())
		{
			return KKSG::ERR_PVP_ROLE_INBATTLE;
		}
	}

	CheckTeamStop(pTeam);

	auto handler = GetMatchHandler(type);
	if(NULL == handler)
	{
		return KKSG::ERR_FAILED;
	}

	int ret = handler->Start(pTeam, this);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	pTeam->SetMatchType(type);

	return KKSG::ERR_SUCCESS;
}

void HMatchHandler::Stop(CRole* pRole, int type)
{
	if(NULL == pRole)
	{
		return ;
	}

	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " type = " << type << END;

	if(KKSG::KMT_NONE == type)
	{
		return; 
	}

	if(KKSG::KMT_NONE == pRole->GetMatchType())
	{
		return;
	}

	if(pRole->GetMatchType() != type)
	{
		SSWarn << __FUNCTION__ << " role = " << pRole->GetID() << " rtype = " << pRole->GetMatchType() << " wtype = " << type << END;
		return ;
	}

	auto handler = GetMatchHandler(type);
	if(NULL == handler)
	{
		return; 
	}

	handler->Stop(pRole, this);

	pRole->SetMatchType(KKSG::KMT_NONE);
}

void HMatchHandler::Stop(Team* pTeam, int type)
{
	if(NULL == pTeam)
	{
		return ;
	}

	if(KKSG::KMT_NONE == type)
	{
		return ;
	}

	SSDebug << __FUNCTION__ << " team = " << pTeam->GetID() << " type = " << type << END;

	if(KKSG::KMT_NONE == pTeam->GetMatchType())
	{
		return;
	}

	if(pTeam->GetMatchType() != type)
	{
		SSWarn << __FUNCTION__ << " team = " << pTeam->GetID() << " leader = " << pTeam->GetLeader() << " rtype = " << pTeam->GetMatchType() << " wtype = " << type << END;
		return;
	}

	auto handler = GetMatchHandler(type);
	if(NULL == handler)
	{
		return ;
	}
	handler->Stop(pTeam, this);

	pTeam->SetMatchType(KKSG::KMT_NONE);
}

void HMatchHandler::Done(CRole* pRole, int type)
{
	if(NULL == pRole)
	{
		return; 
	}
	if(pRole->GetMatchType() != type)
	{
		SSWarn << __FUNCTION__ << " role = " << pRole->GetID() << " rtype = " << pRole->GetMatchType() << " wtype = " << type << END;
		return;
	}

	SSDebug << __FUNCTION__ << " role = " << pRole->GetAccount() << " type = " << type << END;

	pRole->SetMatchType(KKSG::KMT_NONE);
}

void HMatchHandler::Done(Team* pTeam, int type)
{
	if(NULL == pTeam)
	{
		return ;
	}
	if(pTeam->GetMatchType() != type)
	{
		SSWarn << __FUNCTION__ << " team = " << pTeam->GetID() << " leader = " << pTeam->GetLeader() << " rtype = " << pTeam->GetMatchType() << " wtype = " << type << END;
		return ;
	}

	SSDebug << __FUNCTION__ << " team = " << pTeam->GetID() << " type = " << type << END;

	pTeam->SetMatchType(KKSG::KMT_NONE);
}

void HMatchHandler::OnChangeScene(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}

	SSDebug << __FUNCTION__ << " role = " << pRole->GetAccount() << END;

	CheckRoleStop(pRole);

	///> 全民pk取消匹配
	CustomBattleMsMatcher::Instance()->Cancel(pRole->GetID());

	if(pRole->HasTeamInMs())
	{
		auto pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if(pTeam && pTeam->GetMatchType() != KKSG::KMT_NONE)
		{
			//Stop(pTeam, pTeam->GetMatchType());
			TeamRequest req(pRole);
			req.LeaveTeam(KKSG::LTT_BY_SELF);
		}
	}

}

void HMatchHandler::OnLogout(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}

	SSDebug << __FUNCTION__ << " role = " << pRole->GetAccount() << END;

	CheckRoleStop(pRole);

	///> 全民pk取消匹配
	CustomBattleMsMatcher::Instance()->Cancel(pRole->GetID());

	//Team 那边会处理离队
}

HMatchBase* HMatchHandler::GetMatchHandler(int type)
{
	auto it = m_allMatch.find(type);
	if(it != m_allMatch.end())
	{
		return (it->second);
	}
	return NULL;
}

void HMatchHandler::OnJoinTeam(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}

	SSDebug << __FUNCTION__ << " role = " << pRole->GetAccount() << END;

	//m_data.joinTeam = true;
	CheckRoleStop(pRole);

	///> 全民pk取消匹配
	CustomBattleMsMatcher::Instance()->Cancel(pRole->GetID());
}

int HMatchHandler::RoleMatchCond(CRole* pRole, int type)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	if(pRole->IsInBattle())
	{
		if(KKSG::KMT_PK == type)
		{
			if(pRole->GetMapID() != GetGlobalConfig().PkSceneId)
			{
				return KKSG::ERR_INVFIGHT_ME_SCENE;//errorcode wait replace
			}
		}
		else
		{
			return KKSG::ERR_INVFIGHT_ME_SCENE;//errorcode wait replace
		}
	}
	return KKSG::ERR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////
void HMatchHandler::CheckRoleStop(CRole* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	if(pRole->GetMatchType() != KKSG::KMT_NONE)
	{
		HMatchHandler handler;//用新的 防止数据覆盖
		//兼容通用流程
		handler.Data().matchArg.set_type((KKSG::KMatchType)(pRole->GetMatchType()));
		handler.Data().matchArg.set_op(KKSG::KMATCH_OP_STOP);
		handler.Data().matchArg.set_isteam(false);

		handler.Stop(pRole, pRole->GetMatchType());
	}
}

void HMatchHandler::CheckRoleStop(CRole* pRole, int type)
{
	if(NULL == pRole)
	{
		return;
	}
	if(pRole->GetMatchType() != type)
	{
		return;
	}
	CheckRoleStop(pRole);
}

void HMatchHandler::CheckTeamStop(Team* pTeam)
{
	if(NULL == pTeam)
	{
		return;
	}
	if(pTeam->GetMatchType() != KKSG::KMT_NONE)
	{
		HMatchHandler handler;//用新的 防止数据覆盖
		//兼容通用流程
		handler.Data().matchArg.set_type((KKSG::KMatchType)(pTeam->GetMatchType()));
		handler.Data().matchArg.set_op(KKSG::KMATCH_OP_STOP);
		handler.Data().matchArg.set_isteam(true);

		handler.Stop(pTeam, pTeam->GetMatchType());
	}
}

void HMatchHandler::CheckTeamStop(Team* pTeam, int type)
{
	if(NULL == pTeam)
	{
		return;
	}
	if(pTeam->GetMatchType() != type)
	{
		return;
	}
	CheckTeamStop(pTeam);
}

KKSG::ErrorCode HMatchHandler::RoleCondition(CRole* pRole, int type, KKSG::KMatchRole* krole)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}

	KKSG::ErrorCode ret = KKSG::ERR_SUCCESS;

	auto pSub = GetMatchHandler(type);
	if(pSub)
	{
		ret = pSub->MatchCondition(pRole);
		TJUDGERET(ret);

		if(krole)
		{
			ret = pSub->FillMatchData(pRole, krole);	
			TJUDGERET(ret);
		}
	}
	else
	{
		;//Get不到表示不需要处理
	}
	return ret;
}

UINT64 HMatchHandler::GetMatchUniqueIndex(bool inc/* = false*/)
{
	static UINT64 UniqueIndex = 1;
	if(!inc)
	{
		return UniqueIndex;
	}
	return UniqueIndex ++;
}

int HMatchExp::Start(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " do " << END;
	int expid = pHandler->Data().expid;
	if(TeamTransHandler::NeedInTs(expid))
	{
		TeamTransHandler handler;
		if(handler.HandleRoleMatchToTs(pRole, expid, true))
		{
			return KKSG::ERR_SUCCESS;
		}
		else
		{
			return KKSG::ERR_FAILED;
		}
	}
	else
	{
		return TeamMatchMgr::Instance()->StartMatch(pRole, expid);
	}
}

int HMatchExp::Start(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team " << pTeam->GetID() << " do " << END;
	return TeamMatchMgr::Instance()->StartMatch(pTeam);
}

void HMatchExp::Stop(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " do " << END;

	//ms 和 ts 都尝试stop 

	TeamTransHandler handler;
	handler.HandleRoleMatchToTs(pRole, 0, false);

	TeamMatchMgr::Instance()->StopMatch(pRole);
}

void HMatchExp::Stop(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team " << pTeam->GetID() << " do " << END;
	TeamMatchMgr::Instance()->StopMatch(pTeam);
}

///////////////////////////////////////////////////////////////////////////////////////////

int HMatchHero::Start(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " do " << END;
	if (IsMatchInWorld())
	{
		return HMatchBase::Start(pRole, pHandler);
	}
	HeroMatchUnit unit(pRole->GetID(), 0);
	if (pHandler->Data().matchData.roledata_size() == 0)
	{
		return KKSG::ERR_UNKNOWN;
	}
	unit.eloPoint = pHandler->Data().matchData.roledata(0).elopoint();
	return HeroBattleMgr::Instance()->AddMatchUnit(unit);
}

int HMatchHero::Start(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team " << pTeam->GetID() << " do " << END;
	double elopoint = 0;
	for (int i = 0; i < pHandler->Data().matchData.roledata_size(); ++i)
	{
		elopoint += pHandler->Data().matchData.roledata(i).elopoint();
	}
	elopoint /= 1.0 * pHandler->Data().matchData.roledata_size();
	HeroMatchUnit unit(0, pTeam->GetID());
	unit.eloPoint = elopoint;
	return HeroBattleMgr::Instance()->AddMatchUnit(unit);
}

void HMatchHero::Stop(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " do " << END;
	if (IsMatchInWorld())
	{
		HMatchBase::Stop(pRole, pHandler);
		return;
	}
	HeroBattleMgr::Instance()->RemoveMatchUnit(HeroMatchUnit(pRole->GetID(), 0));
}

void HMatchHero::Stop(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team " << pTeam->GetID() << " do " << END;
	HeroBattleMgr::Instance()->RemoveMatchUnit(HeroMatchUnit(0, pTeam->GetID()));
}

bool HMatchHero::IsMatchInWorld()
{
	ExpeditionTable::RowData* conf = ExpeditionConfigMgr::Instance()->GetTeamType2ExpConf(HEROBATTLE_TEAM_SCENE);
	if (conf == NULL)
	{
		return false;
	}
	return conf->isCrossServerInvite;
}

///////////////////////////////////////////////////////////////////////////////////////////

int HMatchMoba::Start(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " do " << END;
	if (IsMatchInWorld())
	{
		return HMatchBase::Start(pRole, pHandler);
	}
	MobaRecord* pRecord = MobaRecordMgr::Instance()->GetMobaRoleRecord(pRole->GetID());
	if (pRecord == NULL)
	{
		return KKSG::ERR_MOBA_CANTMATCH;
	}
	MobaMatchUnit unit(pRole->GetID(), 0);
	unit.eloPoint = pRecord->GetELOPoint();
	return MobaBattleMgr::Instance()->AddMatchUnit(unit);
}

int HMatchMoba::Start(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team " << pTeam->GetID() << " do " << END;
	double eloPoint = 0, count = 0;
	const std::list<CTeamMember*>& pMember = pTeam->GetMember();
	for (auto i = pMember.begin(); i != pMember.end(); ++i)
	{
		MobaRecord* pRecord = MobaRecordMgr::Instance()->GetMobaRoleRecord((*i)->m_qwRoleID);
		if (pRecord == NULL)
		{
			return KKSG::ERR_MOBA_CANTMATCH;
		}
		eloPoint += pRecord->GetELOPoint();
		++count;
	}
	if (count == 0)
	{
		LogError("what?!!! team [%d], role count is 0!!", pTeam->GetID());
		return KKSG::ERR_UNKNOWN;
	}
	MobaMatchUnit unit(0, pTeam->GetID());
	unit.eloPoint = eloPoint / (double)count;
	return MobaBattleMgr::Instance()->AddMatchUnit(unit);
}

void HMatchMoba::Stop(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " do " << END;
	if (IsMatchInWorld())
	{
		HMatchBase::Stop(pRole, pHandler);
		return;
	}	
	MobaBattleMgr::Instance()->RemoveMatchUnit(MobaMatchUnit(pRole->GetID(), 0));
}

void HMatchMoba::Stop(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team " << pTeam->GetID() << " do " << END;
	MobaBattleMgr::Instance()->RemoveMatchUnit(MobaMatchUnit(0, pTeam->GetID()));
}

KKSG::ErrorCode HMatchMoba::MatchCondition(CRole* pRole)
{
	MobaRecord* pRecord = MobaRecordMgr::Instance()->GetMobaRoleRecord(pRole->GetID());
	if (pRecord == NULL)
	{
		return KKSG::ERR_MOBA_CANTMATCH;
	}
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode HMatchMoba::FillMatchData(CRole* pRole, KKSG::KMatchRole* krole)
{
	MobaRecord* pRecord = MobaRecordMgr::Instance()->GetMobaRoleRecord(pRole->GetID());
	if (pRecord == NULL)
	{
		return KKSG::ERR_MOBA_CANTMATCH;
	}
	krole->set_elopoint(pRecord->GetELOPoint());
	return KKSG::ERR_SUCCESS;
}

bool HMatchMoba::IsMatchInWorld()
{
	ExpeditionTable::RowData* conf = ExpeditionConfigMgr::Instance()->GetTeamType2ExpConf(MOBABATTLE_TEAM_SCENE);
	if (conf == NULL)
	{
		return false;
	}
	return conf->isCrossServerInvite;
}

///////////////////////////////////////////////////////////////////////////////////////////
int HMatchPk::Start(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " do " << END;
	if(PkMgr::Instance()->AddRole(pRole, pHandler->Data().pkAddRobot, pHandler->Data().pkMatchInfo))
	{
		return KKSG::ERR_SUCCESS;
	}
	return KKSG::ERR_FAILED;
}

void HMatchPk::Stop(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role " << pRole->GetAccount() << " do " << END;
	PkMgr::Instance()->RemoveRole(pRole->GetID(), PK_RR_STOP);
}

///////////////////////////////////////////////////////////////////////////////////////////

int HMatchLeague::Start(CRole* pRole, HMatchHandler* pHandler)
{
	SSWarn << " should not run here role = " << pRole->GetID() << END;
	return KKSG::ERR_FAILED;
}

void HMatchLeague::Stop(CRole* pRole, HMatchHandler* pHandler)
{
	SSWarn << " should not run here role = " << pRole->GetID() << END;
}

int HMatchLeague::Start(Team* pTeam, HMatchHandler* pHandler)
{
	return LeagueBattleMatchMgr::Instance()->StartMatch(pHandler->Data().leagueTeamId, pTeam);
}

void HMatchLeague::Stop(Team* pTeam, HMatchHandler* pHandler)
{
	LeagueBattleMatchMgr::Instance()->CancelMatch(pTeam);
}

///////////////////////////////////////////////////////////////////////////////////////////
int HMatchSkyCraft::Start(Team* pTeam, HMatchHandler* pHandler)
{
	return SkyCraftMatchMgr::Instance()->StartMatch(pHandler->Data().stId, pTeam);
}

void HMatchSkyCraft::Stop(Team* pTeam, HMatchHandler* pHandler)
{
	SkyCraftMatchMgr::Instance()->CancelMatch(pTeam);
}

bool TIsCrossTMRun()
{
	return (TeamLink::Instance()->IsConnected() && CrossZoneStatus::Instance()->IsCrossZoneUsable());
}

///////////////////////////////////////////////////////////////////////////////////////////
int HMatchPkTwo::Start(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role = " << pRole->GetID() << " do " << END;
	std::vector<UINT64> roleIDs;
	roleIDs.push_back(pRole->GetID());
	return Start(pRole->GetID(), 0, roleIDs, pHandler);
}

int HMatchPkTwo::Start(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team = " << pTeam->GetID() << " do " << END;
	std::vector<UINT64> roleIDs;
	pTeam->GetRoleIDs(roleIDs);
	return Start(0, pTeam->GetID(), roleIDs, pHandler);
}

void HMatchPkTwo::Stop(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role = " << pRole->GetID() << " do " << END;
	Stop(pRole->GetID(), 0, pHandler);
}

void HMatchPkTwo::Stop(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team = " << pTeam->GetID() << " do " << END;
	Stop(0, pTeam->GetID(), pHandler);
}

void HMatchPkTwo::Stop(UINT64 roleID, int teamID, HMatchHandler* pHandler)
{
	if(!CrossZoneStatus::Instance()->IsCrossZoneUsable())
	{
		SSWarn << " cross zone err " << END;
		return;
	}
	RpcM2K_KMatchReqm2k* rpc = RpcM2K_KMatchReqm2k::CreateRpc();	
	auto& data = rpc->m_oArg;
	*data.mutable_matcharg() = pHandler->Data().matchArg;

	auto& kunit = *data.mutable_kunit();
	kunit.set_serverid(MSConfig::Instance()->GetServerID());
	kunit.set_roleid(roleID);
	kunit.set_teamid(teamID);

	WorldLink::Instance()->SendTo(*rpc);
}

int HMatchPkTwo::Start(UINT64 roleID, int teamID, std::vector<UINT64>& roleIDs, HMatchHandler* pHandler)
{
	if(NULL == pHandler)
	{
		return KKSG::ERR_FAILED;
	}
	KKSG::KMatchUnit kunit;
	kunit.set_serverid(MSConfig::Instance()->GetServerID());
	kunit.set_roleid(roleID);
	kunit.set_teamid(teamID);
	if(roleIDs.empty())
	{
		return KKSG::ERR_FAILED;
	}
	UINT32 allPoint = 0;
	for(size_t i = 0; i < roleIDs.size(); ++i)
	{
		UINT64 tmpRoleID = roleIDs[i];
		auto& data = *kunit.add_roles();
		data.set_roleid(tmpRoleID);
		data.set_serverid(MSConfig::Instance()->GetServerID());
		auto pSummry = CRoleSummaryMgr::Instance()->GetRoleSummary(tmpRoleID);
		if(pSummry)
		{
			pSummry->FillSmallInfo(*data.mutable_rolebrief());
		}
		else
		{
			SSWarn << " find role summary failed roleid = " << tmpRoleID << END;
			return KKSG::ERR_FAILED;
		}

		const KKSG::PkRoleRec* pkrec = GetRoleRec(tmpRoleID, pHandler);
		if(NULL == pkrec)
		{
			SSWarn << " get role rec failed roleid = " << tmpRoleID << END;
			return KKSG::ERR_FAILED;
		}
		*data.mutable_pkrec() = *pkrec;

		allPoint += data.pkrec().point();
	}
		
	KKSG::PkMatchInfo tmpInfo;
	PkMgr::Instance()->GetMatchStage(allPoint/roleIDs.size(), tmpInfo, KKSG::PK_2v2);
	if(tmpInfo.stage_size() <= 0)
	{
		SSWarn << " match stage = 0 allpoint = " << allPoint << " role count = " << roleIDs.size() << END;
		return KKSG::ERR_FAILED;
	}
	//如果是组队则使用相同的Stage
	for(int i = 0; i < kunit.roles_size(); ++i)
	{
		kunit.mutable_roles(i)->mutable_pkmatchstage()->CopyFrom(tmpInfo.stage());
	}

	if(!CrossZoneStatus::Instance()->IsCrossZoneUsable())
	{
		SSWarn << " cross zone err " << END;
		return KKSG::ERR_FAILED;
	}
	
	RpcM2K_KMatchReqm2k* rpc = RpcM2K_KMatchReqm2k::CreateRpc();	
	auto& data = rpc->m_oArg;

	*data.mutable_matcharg() = pHandler->Data().matchArg;

	*data.mutable_kunit() = kunit;

	WorldLink::Instance()->SendTo(*rpc);

	return KKSG::ERR_SUCCESS;
}

const KKSG::PkRoleRec* HMatchPkTwo::GetRoleRec(UINT64 roleID, HMatchHandler* pHandler)
{
	if(NULL == pHandler)
	{
		return NULL;
	}
	auto& data = pHandler->Data();
	for(int i = 0; i < data.matchData.roledata_size(); ++i)
	{
		if(roleID == data.matchData.roledata(i).roleid())		
		{
			return &data.matchData.roledata(i).pkrec();
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////
int HMatchCustomPkTwo::Start(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role = " << pRole->GetID() << " do " << END;
	std::vector<UINT64> roleIDs;
	roleIDs.push_back(pRole->GetID());
	return Start(pRole->GetID(), 0, roleIDs, pHandler);
}

int HMatchCustomPkTwo::Start(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team = " << pTeam->GetID() << " do " << END;
	std::vector<UINT64> roleIDs;
	pTeam->GetRoleIDs(roleIDs);
	return Start(pTeam->GetLeader(), pTeam->GetID(), roleIDs, pHandler);
}

void HMatchCustomPkTwo::Stop(CRole* pRole, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " role = " << pRole->GetID() << " do " << END;

	MatchMgr::Instance()->DelMatchUnitRole(MSConfig::Instance()->GetServerID(), pRole->GetID());

	Stop(pRole->GetID(), 0, pHandler);
}

void HMatchCustomPkTwo::Stop(Team* pTeam, HMatchHandler* pHandler)
{
	SSDebug << __FUNCTION__ << " team = " << pTeam->GetID() << " do " << END;

	MatchMgr::Instance()->DelMatchUnitTeam(MSConfig::Instance()->GetServerID(), pTeam->GetID());

	Stop(0, pTeam->GetID(), pHandler);
}

void HMatchCustomPkTwo::Stop(UINT64 roleID, int teamID, HMatchHandler* pHandler)
{
	if(!CrossZoneStatus::Instance()->IsCrossZoneUsable())
	{
		SSWarn << " cross zone err " << END;
		return;
	}
	RpcM2K_KMatchReqm2k* rpc = RpcM2K_KMatchReqm2k::CreateRpc();	
	auto& data = rpc->m_oArg;
	*data.mutable_matcharg() = pHandler->Data().matchArg;

	auto& kunit = *data.mutable_kunit();
	kunit.set_serverid(MSConfig::Instance()->GetServerID());
	kunit.set_roleid(roleID);
	kunit.set_teamid(teamID);

	WorldLink::Instance()->SendTo(*rpc);
}

int HMatchCustomPkTwo::Start(UINT64 roleID, int teamID, std::vector<UINT64>& roleIDs, HMatchHandler* pHandler)
{
	if(NULL == pHandler)
	{
		return KKSG::ERR_FAILED;
	}
	KKSG::KMatchUnit kunit;
	kunit.set_serverid(MSConfig::Instance()->GetServerID());
	if (0 == teamID)
	{
		kunit.set_roleid(roleID);
	}
	kunit.set_teamid(teamID);
	if(roleIDs.empty())
	{
		return KKSG::ERR_FAILED;
	}
	///> 需要区分是本服(分数信息在ms)，跨服(分数信息在world)
	bool iscross = false;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->TryGetInfo(roleID);
	if (NULL == roleinfo || 0 == roleinfo->GetJoinId())
	{
		return KKSG::ERR_CUSTOMBATTLE_NOTJOIN;
	}
	CustomBattle* battle = CustomBattleMgr::Instance()->GetBattle(roleinfo->GetJoinId());
	if (NULL == battle)
	{
		iscross = true;
	}
	else
	{
		if (!battle->IsPvpTwo())
		{
			return KKSG::ERR_CUSTOMBATTLE_NOTJOIN;
		}
		KKSG::ErrorCode ret = battle->CanMatch();
		if (KKSG::ERR_SUCCESS != ret)
		{
			return ret;
		}
	}
	kunit.set_param(roleinfo->GetJoinId());

	UINT32 allPoint = 0;
	for(size_t i = 0; i < roleIDs.size(); ++i)
	{
		UINT64 tmpRoleID = roleIDs[i];
		auto& data = *kunit.add_roles();
		data.set_roleid(tmpRoleID);
		data.set_serverid(MSConfig::Instance()->GetServerID());
		auto pSummry = CRoleSummaryMgr::Instance()->GetRoleSummary(tmpRoleID);
		if(pSummry)
		{
			pSummry->FillSmallInfo(*data.mutable_rolebrief());
		}
		else
		{
			SSWarn << " find role summary failed roleid = " << tmpRoleID << END;
			return KKSG::ERR_FAILED;
		}

		if (NULL != battle)
		{
			///> get point from custombattle
			allPoint += battle->GetPoint(tmpRoleID);
		}
	}
		
	if (!iscross)
	{
		KKSG::CustomBattleMatchRoleInfo tmpInfo;
		CCustomBattleConfig::Instance()->FillPkStage(allPoint/roleIDs.size(), tmpInfo, KKSG::PK_2v2);
		if(tmpInfo.stages_size() <= 0)
		{
			SSWarn << " match stage = 0 allpoint = " << allPoint << " role count = " << roleIDs.size() << END;
			return KKSG::ERR_FAILED;
		}
		//如果是组队则使用相同的Stage
		for(int i = 0; i < kunit.roles_size(); ++i)
		{
			kunit.mutable_roles(i)->mutable_pkmatchstage()->CopyFrom(tmpInfo.stages());
		}
		MatchMgr::Instance()->AddMatchUnit(KKSG::KMT_CUSTOM_PKTWO, kunit);
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		if(!CrossZoneStatus::Instance()->IsCrossZoneUsable())
		{
			SSWarn << " cross zone err " << END;
			return KKSG::ERR_FAILED;
		}
	
		RpcM2K_KMatchReqm2k* rpc = RpcM2K_KMatchReqm2k::CreateRpc();	
		auto& data = rpc->m_oArg;
		data.set_uniqueindex(HMatchHandler::GetMatchUniqueIndex());
		*data.mutable_matcharg() = pHandler->Data().matchArg;

		*data.mutable_kunit() = kunit;

		WorldLink::Instance()->SendTo(*rpc);

		return KKSG::ERR_SUCCESS;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

