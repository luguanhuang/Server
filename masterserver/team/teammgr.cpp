#include "pch.h"
#include "teammgr.h"
#include "team.h"
#include "foreach.h"
#include <time.h>
#include "team/ptcm2c_leaveteamm2cntf.h"
#include "teamrequest.h"
#include "role/rolemanager.h"
#include "team/rpcm2g_teamsynm2c.h"
#include "network/gslink.h"
#include "teaminvitemgr.h"
#include "teammatchmgr.h"
#include "teammember.h"
#include "scene/gsmanager.h"
#include "teamforceop.h"
#include "notice/noticemgr.h"
#include "table/globalconfig.h"
#include "other/matchhandler.h"
#include "scene/scenemanager.h"
#include "team/teamcommon.h"
#include "team/teamtranshandler.h"

static int TeamID = 1;

INSTANCE_SINGLETON(TeamMgr)

TeamMgr::TeamMgr()
{
	m_Handler = INVALID_HTIMER;
}

TeamMgr::~TeamMgr()
{

}

static bool TeamCmdHandler(const CmdLineArgs &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(size >= 2)
	{
		const std::string& op = args[1];
		if(op == "inv")
		{
			if(size >= 3 && args[2] == "cl")
			{
				TeamInviteMgr::Instance()->ClearAll();
			}
			else
			{
				TeamInviteMgr::Instance()->TestPrint();
			}
		}
	}
	else
	{
		TeamMgr::Instance()->TestPrint();
	}
	return true;
}

bool TeamMgr::Init()
{
	CmdLine::RegistCmd("team", &TeamCmdHandler);

	m_Handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	return true;
}

void TeamMgr::Uninit()
{
	foreach (i in m_Teams)
	{
		delete i->second;
	}

	m_Teams.clear();

	CTimerMgr::Instance()->KillTimer(m_Handler);
}


Team * TeamMgr::CreateTeam(UINT32 expeditionID)
{
	Team *pTeam = new Team();
	pTeam->SetID(TeamID++);
	LogInfo("set team %x id to %d", pTeam, pTeam->GetID());
	m_Teams[pTeam->GetID()] = pTeam;
	return pTeam;
}

static void BeforeDestoryCB(CTeamMember *pMember, CRole *pRole, void *arg) 
{
	SSError << " should not run here teamid = " << pRole->GetTeamID() << " roleid = " << pRole->GetID() << END;
	PtcM2C_LeaveTeamM2CNtf *pPtc = (PtcM2C_LeaveTeamM2CNtf*)arg;
	pRole->Send(*pPtc);
	pRole->SetTeamID(0);
}

void TeamMgr::DestoryTeam(Team *pTeam)
{
	if(NULL == pTeam)
	{
		return ;
	}

	BeforeDestory(pTeam);

	PtcM2C_LeaveTeamM2CNtf oLeavePtc;
	oLeavePtc.m_Data.set_errorno(0);
	pTeam->Foreach(BeforeDestoryCB, &oLeavePtc);

	int teamID = pTeam->GetID();
	delete pTeam;
	m_Teams.erase(teamID);
}

Team * TeamMgr::FindTeam(int teamID)
{
	auto it = m_Teams.find(teamID);
	return (it == m_Teams.end()) ? NULL : it->second;
}

std::list<int> TeamMgr::GetTeams(const std::vector<UINT32> &Category)
{
	std::list<int> result;

	foreach (i in m_Teams)
	{
		Team *pTeam = i->second;
		if (pTeam->GetConf())
		{
			if (std::find(Category.begin(), Category.end(), pTeam->GetConf()->Category) != Category.end())
			{
				result.push_back(i->first);
			}
		}
	}

	return result;
}

std::list<int> TeamMgr::GetTeams(UINT32 expeditionID)
{
	std::list<int> result;

	foreach (i in m_Teams)
	{
		Team *pTeam = i->second;
		if (pTeam->GetConf() && pTeam->GetConf()->DNExpeditionID == (int)expeditionID)
		{
			result.push_back(i->first);
		}
	}

	return result;
}

void TeamMgr::BeforeDestory(Team *pTeam)
{
	if(NULL == pTeam)
	{
		return ;
	}

	HMatchHandler handler;
	handler.Stop(pTeam, KKSG::KMT_EXP);
	
	SynDel2GS(pTeam->GetID());
	TeamInviteMgr::Instance()->DelInvByTeam(pTeam->GetID());
}

void TeamMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UpdateAllTeam();
}


/*
void TeamMgr::OnRoleEnterScene(Role *pRole, Scene *pScene)
{
	if (pRole->GetTeamID() != INVALID_TEAM_ID)
	{
		Team *pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if(NULL == pTeam)
		{
			return;
		}
		pTeam->UpdateMemberInfo(pRole);
		pTeam->OnRoleEnterScene(pRole);

		if(!pRole->IsInBattle() && pTeam->HasRobotInTeam())
		{
			TeamRequest req(pRole);
			req.LeaveTeam(TEAM_DEL_ROBOT);
		}

		if(!pRole->IsInBattle())
		{
			TeamRequest req(pRole);
			req.QueryFullData();
		}
	}
}
*/

void TeamMgr::OnRoleLogout(CRole* pRole)
{
	if(NULL == pRole || 0 == pRole->GetTeamID())
	{
		return ;
	}
	int teamID = pRole->GetTeamID();//需暂存
	if(0 == teamID)//没有队伍
	{
		return;
	}
	if(HTeam::IsTsTeam(teamID))//在TeamServer
	{
		TeamTransHandler trans;		
		trans.HandleRoleLogout(pRole->GetID(), teamID);
	}
	else
	{
		TeamRequest req(pRole);
		req.LeaveTeam(KKSG::LTT_BY_SELF);

		Team* pTeam = FindTeam(teamID);
		if(pTeam && pRole->IsInBattle())
		{
			pTeam->CheckFinishBattle();
		}
	}
}

void TeamMgr::TestPrint()
{
	for(auto it = m_Teams.begin(); it != m_Teams.end(); ++it)
	{
		(it->second)->InfoPrint();
	}
}

void TeamMgr::SynAllData2GS(Team* pTeam)
{
	if(NULL == pTeam)
	{
		return ;
	}

	KKSG::TeamSynPara para;
	para.set_type(KKSG::STM2C_ALL_DATA);
	pTeam->SaveSynData(*para.mutable_dataall());
	SynToGSCommon(para);
}

void TeamMgr::SynDel2GS(int teamID)
{
	KKSG::TeamSynPara para;
	para.mutable_dataall()->set_teamid(teamID);
	para.set_type(KKSG::STM2C_DESTROY);
	SynToGSCommon(para);
}

void TeamMgr::SynGetExtraData(Team* pTeam, UINT64 roleid/* = 0*/)
{
	if(NULL == pTeam || NULL == pTeam->GetConf())
	{
		return ;
	}
	//SSDebug << __FUNCTION__ << " teamid = " << pTeam->GetID() << " roleid = " << roleid << END;
	RpcM2G_TeamSynM2C* rpc = RpcM2G_TeamSynM2C::CreateRpc();
	rpc->m_oArg.set_type(KKSG::STM2C_GETEXTRADATA);
	rpc->m_oArg.set_teamid(pTeam->GetID());
	rpc->m_oArg.set_expid(pTeam->GetConf()->DNExpeditionID);
	if(0 != roleid)
	{
		rpc->m_oArg.add_roleids(roleid);
	}
	else
	{
		const auto& members = pTeam->GetMember();
		for(auto it = members.begin(); it != members.end(); ++it)
		{
			if((*it)->IsRobot())
			{
				continue;
			}
			rpc->m_oArg.add_roleids((*it)->m_qwRoleID);
		}
	}

	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
}

void TeamMgr::SynGetExtraDataBack(int teamid, const KKSG::TeamSynRes& roRes)
{
	//SSDebug << __FUNCTION__ << " teamid = " << teamid << END;
	Team* pTeam = FindTeam(teamid);
	if(NULL == pTeam || NULL == pTeam->GetConf())
	{
		return ;
	}
	for(int i = 0; i < roRes.memberextradata_size(); ++i)
	{
		const KKSG::TeamSynExtraData& extradata = roRes.memberextradata(i);
		//SSDebug << __FUNCTION__ << " roleid = " << extradata.roleid() << " leftcount " << extradata.leftcount() << END;
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(extradata.roleid());
		if(pRole)
		{
			pTeam->UpdateMemberExtraInfo(pRole, extradata);
		}
	}
}

void TeamMgr::OnRoleLeaveBattle(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	if(pRole->HasTeamInMs())
	{
		Team* pTeam = FindTeam(pRole->GetTeamID());
		if(pTeam)
		{
			pTeam->UpdateMemberInfo(pRole);
			pTeam->OnRoleLeaveBattle(pRole);

			if(pTeam->HasRobotInTeam())
			{
				TeamRequest req(pRole);
				req.LeaveTeam(KKSG::LTT_DEL_ROBOT);
			}

			TeamRequest req(pRole);
			req.QueryFullData();
		}
	}
}

void TeamMgr::OnRoleEnterBattle(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	if(pRole->HasTeamInMs())
	{
		Team* pTeam = FindTeam(pRole->GetTeamID());
		if(pTeam)
		{
			pTeam->UpdateMemberInfo(pRole);

		}
	}
}

void TeamMgr::OnGsRegister(UINT32 gsLine)
{
	SSInfo << __FUNCTION__ << " line = " << gsLine << END;
	int SendCountOneTime = 250;
	KKSG::TeamSynPara para;
	para.set_type(KKSG::STM2C_TEAM_LIST);

	for(auto it = m_Teams.begin(); it != m_Teams.end(); ++it)
	{
		Team* pTeam = (it->second);
		pTeam->SaveSynData(*para.add_teamlist());
		if(para.teamlist_size() >= SendCountOneTime)
		{
			SynToGSCommon(para, gsLine);
			para.clear_teamlist();
		}
	}

	if(para.teamlist_size() > 0)
	{
		SynToGSCommon(para, gsLine);
		para.clear_teamlist();
	}

	TeamTransHandler trans;
	trans.HandlerGsRegister(gsLine);
}

void TeamMgr::OnMsCrashRecover(UINT32 gsLine, const KKSG::CrashRecoverM2GRes &roRes)
{
	SSInfo << __FUNCTION__ << " gsline = " << gsLine << " session size = " << roRes.sessionlist_size() << END;
	if(roRes.sessionlist_size() <= 0)
	{
		return ;
	}
	//要处理本服组队和跨服组队 TODO
	for(int i = 0; i < roRes.sessionlist_size(); ++i)
	{
		CRole* pRole = CRoleManager::Instance()->GetBySession(roRes.sessionlist(i));
		if(pRole)
		{
			PtcM2C_LeaveTeamM2CNtf oLeavePtc;
			oLeavePtc.m_Data.set_errorno(KKSG::LTT_MS_CRASH);
			pRole->Send(oLeavePtc);
		}
	}
}

void TeamMgr::UpdateAllTeam()
{
	time_t now = time(NULL);
	std::vector<CRole*> WaitKickLeader;	
	for(auto it = m_Teams.begin(); it != m_Teams.end(); ++it)
	{
		Team* pTeam = (it->second);
		pTeam->Update();

		if(pTeam->NeedKickLeaderBeforeNotice(now, GetGlobalConfig().TeamKickLeaderNoticeTime))
		{
			NoticeMgr::Instance()->NoticeTeamKickLeaderBefore(pTeam->GetLeader(), GetGlobalConfig().TeamKickLeaderNoticeTime);
		}
		if(pTeam->NeedKickLeader(now))
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(pTeam->GetLeader());
			if(pRole)
			{
				WaitKickLeader.push_back(pRole);
				NoticeMgr::Instance()->NoticeTeamKickLeaderAfter(pTeam->GetLeader());
			}
		}

		if(pTeam->NeedStartMatch())
		{
			HMatchHandler handler;
			handler.Start(pTeam, KKSG::KMT_EXP);
		}

	}

	TeamForceOp forceOp;
	forceOp.LeaveTeam(WaitKickLeader, KKSG::LLT_LEADER_TIMEOVER);
}

void TeamMgr::OnRoleEnterScene(CRole *pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	if(pRole->GetTeamID())
	{
		CScene* pScene = CSceneManager::Instance()->GetScene(pRole->GetSceneID());
		if(pScene && KKSG::SCENE_RISK == pScene->GetSceneType())//大冒险特殊处理
		{
			TeamForceOp op;
			op.LeaveTeam(pRole);
		}
	}
}

void TeamMgr::OnTsDisconnected()
{
	SSInfo << " teamserver disconnected " << END;

	//处理玩家
	auto itRoleEnd = CRoleManager::Instance()->End();
	for(auto it = CRoleManager::Instance()->Begin(); it != itRoleEnd; ++it)
	{
		CRole* pRole = (*it);
		if(pRole->HasTeamInTs())
		{
			//直接通知玩家离队了
			PtcM2C_LeaveTeamM2CNtf oLeavePtc;
			oLeavePtc.m_Data.set_errorno(KKSG::LTT_MS_CRASH);
			pRole->Send(oLeavePtc);

			pRole->SetTeamID(0);//标志
		}
	}

	//处理GS
	auto itLineEnd = CGsManager::Instance()->End();
	for(auto it = CGsManager::Instance()->Begin(); it != itLineEnd; ++it)
	{
		RpcM2G_TeamSynM2C *rpc = RpcM2G_TeamSynM2C::CreateRpc();
		rpc->m_oArg.set_type(KKSG::STM2C_TS_DISCONNECTED);
		GSLink::Instance()->SendToLine(it->first, *rpc);
	}
}

void TeamMgr::SynToGSCommon(const KKSG::TeamSynPara& para, UINT32 gsLine /*= 0*/)
{
	auto itend = CGsManager::Instance()->End();
	for(auto it = CGsManager::Instance()->Begin(); it != itend; ++it)
	{
		if(0 == gsLine || it->first == gsLine)
		{
			RpcM2G_TeamSynM2C* rpc = RpcM2G_TeamSynM2C::CreateRpc();
			rpc->m_oArg.CopyFrom(para);
			GSLink::Instance()->SendToLine(it->first, *rpc);
		}
	}
}

