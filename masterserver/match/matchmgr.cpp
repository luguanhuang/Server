#include "pch.h"
#include "matchmgr.h"
#include "scene/scene.h"
#include "scene/sceneswitch.h"
#include "role/rolemanager.h"
#include "other/matchhandler.h"
#include "team/teammgr.h"
#include "team/teamforceop.h"
#include "role/rolesummarymgr.h"
#include "table/expeditionconfigmgr.h"
#include "pk/pkmgr.h"
#include "loghelper/tlogr.h"
#include "cross/crosszonestatus.h"
#include "team/team.h"
#include "match/matchunit.h"
#include "match/matchunitmgr.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemgr.h"
#include "custombattle/custombattledef.h"

INSTANCE_SINGLETON(MatchMgr)

MatchMgr::MatchMgr()
{
	AddUnitMgr(KKSG::KMT_CUSTOM_PKTWO, new MatchUnitMgrCustomPkTwo(KKSG::KMT_CUSTOM_PKTWO, this));
}

MatchMgr::~MatchMgr()
{

}

static bool MatchCmdHandler(const CmdLineArgs &args, std::string &outputMessage)
{
	auto pMgr = MatchMgr::Instance();
	int size = (int)args.size();
	if(size >= 2)
	{
		const std::string& op = args[1];
		if(op == "cl")
		{
			pMgr->Clear();
		}
		else if(op == "match")
		{
			pMgr->Update();
		}
	}
	else
	{
		pMgr->InfoPrint();
	}
	return true;
}

bool MatchMgr::Init()
{
	CmdLine::RegistCmd("match", &MatchCmdHandler);
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);		
	return true;
}

void MatchMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void MatchMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Update();
}

void MatchMgr::CreateSceneBegin(const KKSG::KMatchFightData& data, std::vector<MatchUnit*>& matched)
{
	if(data.units_size() <= 0)
	{
		SSError << " match units = 0 type = " << data.type() << END;
		return;
	}
	UINT32 mapID = GetMapID(data.type(), data.units(0));

	if(0 == mapID)
	{
		SSError << " mapid = 0 type = " << data.type() << END;
		return;
	}

	KKSG::CreateBattleParam para;
	*para.mutable_kmatchfightdata() = data;
	FillDataExtra(*para.mutable_kmatchfightdata());
	bool flag = CSceneCreator::Instance()->CreateBattleScene(mapID, this, para);
	if(!flag)
	{
		SSError << "create scene failed mapid = " << mapID << END;
	}
}

void MatchMgr::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	if(NULL == scene)
	{
		SSError << " create scene failed result = " << res.result() << END;
		return;
	}
	UINT32 sceneID = scene->GetID();

	KKSG::SceneSwitchData tmpData;
	const auto& data = roParam.kmatchfightdata();
	for(int i = 0; i < data.roles_size(); ++i)
	{
		UINT64 roleID = data.roles(i).roleid();
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
		if(pRole)
		{
			CSceneSwitch::EnterScene(pRole, sceneID, tmpData);
		}
		else
		{
			SSWarn << " find role failed roleid = " << roleID << " sceneid = " << sceneID << " type = " << data.type() << END;
		}
	}
}

void MatchMgr::MatchTimeOut(MatchUnit* pMatchUnit)
{
	
}

void MatchMgr::MatchDone(int type, const KKSG::KMatchUnit& kunit)
{
	if(kunit.serverid() != MSConfig::Instance()->GetServerID())
	{
		SSInfo << " match server = " << kunit.serverid() << " ms server = " << MSConfig::Instance()->GetServerID() << END;
		return;
	}
	HMatchHandler handler;
	TeamForceOp forceOp;
	UINT64 roleID = kunit.roleid();
	int teamID = kunit.teamid();
	if(roleID)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);			
		if(pRole)
		{
			handler.Done(pRole, type);
		}
		else
		{
			SSError << " find role failed role = " << roleID << END;
		}
	}
	else if(teamID)
	{
		Team* pTeam = TeamMgr::Instance()->FindTeam(teamID);
		if(pTeam)
		{
			handler.Done(pTeam, type);
			forceOp.SetInBattleState(teamID);
		}
		else
		{
			SSError << " find team failed team = " << teamID << END;
		}
	}

	DoTxLog(type, teamID);
}

void MatchMgr::FillDataExtra(KKSG::KMatchFightData& data)
{
	if (data.type() == KKSG::KMT_CUSTOM_PKTWO)
	{
		CustomBattle* battle = CustomBattleMgr::Instance()->GetBattle(data.uid());
		if (NULL != battle)
		{
			data.set_param(battle->IsFair() ? 1 : 0);
		}
	}
}

UINT32 MatchMgr::GetMapID(int type, const KKSG::KMatchUnit& kunit)
{
	UINT32 mapID = 0;
	switch(type)
	{
	case KKSG::KMT_PVP:
		{
			mapID = ExpeditionConfigMgr::Instance()->GetPvpScene();
		}
		break;
	case KKSG::KMT_HERO:
		{
			mapID = ExpeditionConfigMgr::Instance()->GetHeroBattleScene();
		}
		break;
	case KKSG::KMT_PKTWO:
		{
			mapID = ExpeditionConfigMgr::Instance()->GetPkTwoScene();
		}
		break;
	case KKSG::KMT_CUSTOM_PKTWO:
		{
			mapID = CustomBattlePkTwo_SceneId;
		}
		break;
	case KKSG::KMT_MOBA:
		{
			mapID = ExpeditionConfigMgr::Instance()->GetMobaBattleScene();
		}
		break;
	case KKSG::KMT_WEEKEND_ACT:
		{
			if(kunit.roles_size() > 0)
			{
				mapID = kunit.roles(0).mapid();
			}
		}
		break;
	default:
		break;
	}
	return mapID;
}

void MatchMgr::OnDelReason(MatchUnit* pUnit, MatchDelReason reason)
{
	if(NULL == pUnit)
	{
		return;
	}
	switch(reason)
	{
	case MDR_MATCHED:
		{
			MatchDone(pUnit->GetType(), pUnit->KKSGData());	
		}
		break;
	case MDR_TIMEOUT:
		{
			;//
		}
		break;
	default:
		break;
	}
}

void  MatchMgr::DoTxLog(int type, int teamID)
{
	TMatchFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_matchID = 0;//未记
	oLog.m_teamID = teamID;
	oLog.m_Type = type;
	oLog.m_Time = 0;//未记
	oLog.Do();
}

void MatchMgr::OnCrossZoneUnusable()
{
	//ws 会删除对应数据
	HMatchHandler handler;
	auto itBegin = CRoleManager::Instance()->Begin();
	auto itEnd = CRoleManager::Instance()->End();
	for(auto it = itBegin; it != itEnd; ++it)
	{
		CRole* pRole = *it;
		const int type = pRole->GetMatchType();
		//一定是在worldserver匹配
		if(type == KKSG::KMT_PKTWO ||
			type == KKSG::KMT_PVP ||
			type == KKSG::KMT_WEEKEND_ACT ||
			type == KKSG::KMT_SURVIVE ||
			type == KKSG::KMT_MOBA ||
			type == KKSG::KMT_HERO)
		{
			handler.Done(pRole, type);
		}
	}
	auto& allTeams = TeamMgr::Instance()->AllTeams();
	for(auto it = allTeams.begin(); it != allTeams.end(); ++it)
	{
		Team* pTeam = it->second;
		const int type = pTeam->GetMatchType();
		//一定是在worldserver匹配
		if(type == KKSG::KMT_PKTWO)
		{
			handler.Done(pTeam, type);
		}
	}
}

void MatchMgr::OnMsCrashRecover(UINT32 gsLine, const KKSG::CrashRecoverM2GRes &roRes)
{
	SSInfo << __FUNCTION__ << " gsline = " << gsLine << " session size = " << roRes.sessionlist_size() << END;
	if(roRes.sessionlist_size() <= 0)
	{
		return ;
	}
	for(int i = 0; i < roRes.sessionlist_size(); ++i)
	{
		CRole* pRole = CRoleManager::Instance()->GetBySession(roRes.sessionlist(i));
		if(pRole)
		{
			pRole->MatchTypeNtf();
		}
	}
}
