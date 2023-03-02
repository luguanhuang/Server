#include "pch.h"
#include "sceneteam.h"
#include "foreach.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/towerconfig.h"
#include "scene/sceneswitch.h"
#include "table/ExpeditionTable.h"
#include "table/expeditionconfigmgr.h"
#include "gamelogic/team.h"
#include "live/livemanager.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/rolesummarymgr.h"
#include "unit/dummyrolemanager.h"
#include "entity/XObject.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "table/DragonNestTable.h"
#include "gamelogic/dragonconfig.h"
#include "attributepk.h"
#include "scenemanager.h"
#include "common/ptcg2c_gserrornotify.h"
#include "scene/scenedoodad.h"
#include "define/itemdef.h"
#include "gamelogic/teamrecord.h"
#include "enterscenecondition.h"
#include "gamelogic/teamrequest.h"
#include "scene/scenemorphinfo.h"
#include "team/ptcg2m_sceneteamresultntf.h"
#include "mslink.h"
#include "team/ptcg2m_teamcostleaderresetntf.h"
#include "gamelogic/teamsynextradata.h"
#include "team/teamcommon.h"
#include "team/ptcg2m_teamtrangs2ms.h"
#include "util/gametime.h"
#include "gamelogic/dragongroupmgr.h"
#include "table/DragonNestTable.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/bagtransition.h"
#include "network/worldlink.h"
#include "team/rpcg2k_teamgotonextscenereq.h"
#include "servermgr/servercrossdatamgr.h"
#include "servermgr/servermgr.h"

SceneTeam::SceneTeam(Scene* scene)
{
	m_pScene = scene;
	m_pConf = NULL;
	m_addRobotFlag = false;
	m_createTime = GameTime::GetTime();
}

SceneTeam::~SceneTeam()
{
	for(auto it = m_robotSummarys.begin(); it != m_robotSummarys.end(); ++it)
	{
		delete (*it);
	}
	m_robotSummarys.clear();
}

void SceneTeam::Init(const KKSG::SceneSwitchTeamData& data)
{
	m_KKSGData.CopyFrom(data);

	m_pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(GetExpID());
	if(NULL == m_pConf)
	{
		SSError << " find not conf expid = " << GetExpID() << END;
	}

	const auto& synData = m_KKSGData.teamsyndata();
	for(int i = 0; i < synData.members_size(); ++i)
	{
		UINT64 roleID = synData.members(i).roleid();
		if(Unit::GetUnitType(roleID) != KKSG::Category_Role)//机器人
		{
			continue;
		}
		m_allMemberIDs.push_back(roleID);
	}

	for(int i = 0; i < m_KKSGData.robotsummarystored_size() && i < m_KKSGData.robotid_size(); ++i)
	{
		RoleSummary* pRoleSummary = new RoleSummary();
		RoleSummaryMgr::Instance()->DBStoredToSummary(&(m_KKSGData.robotsummarystored(i)), pRoleSummary);
		pRoleSummary->qwRobotID = m_KKSGData.robotid(i);
		if(pRoleSummary)
		{
			SSDebug << "role summary roleid = " << pRoleSummary->GetID()<< " robotid = " << pRoleSummary->qwRobotID 
				<<" name = " << pRoleSummary->GetName()<< " level = " << pRoleSummary->GetLevel()<< END;
			m_robotSummarys.push_back(pRoleSummary);
		}

	}

	m_pScene->GetSceneMorphInfo()->Init(data);

	CreateLiveInfo();

//	InitRoleBuffs();

	InitDroppableInfo();

	InfoPrint();
}

void SceneTeam::InitRoleData(const KKSG::SceneSwitchData& roData, Role* pRole)
{
	if(!GSConfig::Instance()->IsCrossGS())
	{
		return ;//以防万一
	}

	const auto& teamData = roData.teamdata();

	for(int i = 0; i < teamData.rolebufflist_size(); ++i)
	{
		bool have = false;
		const auto& buffI = teamData.rolebufflist(i);
		for(int j = 0; j < m_KKSGData.rolebufflist_size(); ++j)
		{
			const auto& buffJ = m_KKSGData.rolebufflist(j);

			if(buffI.roleid() == buffJ.roleid() &&
				buffI.buff().buffid() == buffJ.buff().buffid() &&
				buffI.buff().bufflevel() == buffJ.buff().bufflevel())
			{
				have = true;
				break;
			}
		}
		if(!have)
		{
			*m_KKSGData.add_rolebufflist() = buffI;
		}
		else
		{
			SSDebug << " add buff repeat role = " << pRole->GetID() << END;
		}
	}
}

void SceneTeam::CreateLiveInfo()
{
	if (!GetExpID())
	{
		return;
	}
	if (!GetLeaderID())
	{
		return;
	}
	if (m_pScene->GetSceneType() == KKSG::SCENE_NEST || m_pScene->GetSceneType() == KKSG::SCENE_DRAGON || m_pScene->GetSceneType() == KKSG::SCENE_WEEK_NEST)
	{
		std::vector<UINT64> roles;
		roles.push_back(GetLeaderID());
		for (size_t i = 0; i < m_allMemberIDs.size(); ++i)
		{
			//Role* role = RoleManager::Instance()->FindByRoleID(m_allMemberIDs[i]);
			//if (role)
			//{
			//	roles.push_back(role->GetID());
			//}

			roles.push_back(m_allMemberIDs[i]);
		}
		if (roles.size() < 2)
		{
			return;
		}
		std::vector<int> expIDs;
		expIDs.push_back(GetExpID());
		LiveInfo* info = NULL;
		if (m_pScene->GetSceneType() == KKSG::SCENE_NEST || m_pScene->GetSceneType() == KKSG::SCENE_WEEK_NEST)
		{
			info = LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_NEST, m_pScene->GetSceneID(), &expIDs, 0, &roles);
		}
		else if (m_pScene->GetSceneType() == KKSG::SCENE_DRAGON)
		{
			info = LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_DRAGON, m_pScene->GetSceneID(), &expIDs, 0, &roles);
		}
		/*if (info)
		{
			m_pScene->SetLiveID(info->m_liveID);
		}*/
	}
}

void SceneTeam::RemoveLiveInfo()
{
	if (m_pScene->GetSceneType() == KKSG::SCENE_NEST || m_pScene->GetSceneType() == KKSG::SCENE_WEEK_NEST)
	{
		LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_NEST, m_pScene->GetSceneID(), 0, 0, 0);
	}
	if (m_pScene->GetSceneType() == KKSG::SCENE_DRAGON)
	{
		LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_DRAGON, m_pScene->GetSceneID(), 0, 0, 0);
	}
}

void SceneTeam::ChangeScene()
{
	CtCsInfo info;
	info.leaderID = GetLeaderID();//假装不变
	GotoNextScene(info);
}

int SceneTeam::GotoNextScene(CtCsInfo& info)
{
	const std::list<Role *>& tmp = m_pScene->GetAllRoles();
	const std::map<UINT64, Role*>& loadingRole = m_pScene->GetWaitingRoles();
	std::vector<Role*> roles(tmp.begin(), tmp.end());
	foreach(i in loadingRole)
	{
		if (i->second->IsWatcher())
		{
			continue;
		}
		roles.push_back(i->second);
	}
	if (roles.empty())//场景中没有人
	{
		return KKSG::ERR_FAILED;
	}

	KKSG::SceneSwitchData oData;

	if(IsCostTeam() && !InAirShip())//赏金团(非飞艇)
	{
		HandleChangeScene(roles, GetGlobalConfig().InitialReturnSceneID, oData);
		return KKSG::ERR_SUCCESS;
	}

	UINT32 nextExpID = GetNextExpID();
	if(0 == nextExpID)
	{
		HandleChangeScene(roles, GetGlobalConfig().InitialReturnSceneID, oData);
		return KKSG::ERR_SUCCESS;
	}

	UINT32 nextSceneID = GetNextSceneID(nextExpID);//ExpeditionConfigMgr::Instance()->GetExpRandomScene(nextExpID);
	if(0 == nextSceneID)
	{
		SSError << " get scene failed exp = " << nextExpID << END;
		return KKSG::ERR_FAILED;
	}

	KKSG::SceneSwitchTeamData& data = *oData.mutable_teamdata();
	data.CopyFrom(m_KKSGData);	
	data.clear_csextra();//跨服切换场景数据清除

	if(!InAirShip())
	{
		std::map<Role*, int> roleRet;
		ChangeSceneRoleFilter(nextExpID, roles, roleRet, info);
		ChangeSceneFailNtf(roleRet);

		int ret = ChangeSceneCheck(roles, roleRet, info);
		if(KKSG::ERR_SUCCESS != ret)
		{
			return ret;
		}

		ChangeSceneDataCh(nextExpID, data, roleRet, info);
	}

	//这里只支持本服 改跨服要处理
	ChagneSceneDeathRole(roles, oData);
	ChangeSceneTotalCostTime(oData);

	bool flag = HandleChangeScene(roles, nextSceneID, oData);
	if(!flag)
	{
		return KKSG::ERR_FAILED;
	}
	return KKSG::ERR_SUCCESS;
}

bool SceneTeam::HandleChangeScene(std::vector<Role*>& roles, UINT32 nextSceneID, const KKSG::SceneSwitchData& oData)
{
	if(!GSConfig::Instance()->IsCrossGS())	
	{
		return CSceneSwitch::ChangeScene(roles, nextSceneID, oData);
	}
    else
	{
        if(GetGlobalConfig().InitialReturnSceneID == nextSceneID)
        {
            for(size_t i = 0; i < roles.size(); ++i)
            {
                std::vector<Role*> tmps;
                tmps.push_back(roles[i]);
                CSceneSwitch::ChangeScene(tmps, nextSceneID, oData);
            }
            return true;
        }
        else
        {
            if(!WorldLink::Instance()->IsConnected())
            {
                return false;
            }

            RpcG2K_TeamGoToNextSceneReq* rpc = RpcG2K_TeamGoToNextSceneReq::CreateRpc();
            auto& data = rpc->m_oArg;
            *data.mutable_teamdata() = oData.teamdata();
            data.set_mapid(nextSceneID);
            auto& extra = *data.mutable_teamdata()->mutable_csextra();
            if(GetGlobalConfig().InitialReturnSceneID == nextSceneID)
            {
                extra.set_type(TGOTONST_RETURN);
            }
            else
            {
                extra.set_type(TGOTONST_NEXT);
            }
            for(size_t i = 0; i < roles.size(); ++i)
            {
                const auto* pMember = GetSynMember(roles[i]->GetID());
                if(pMember)
                {
                    *extra.add_members() = *pMember;
                }
            }
            WorldLink::Instance()->SendTo(*rpc);

            return true;
        }
	}
}

void SceneTeam::WinAddTeamDayCount()
{
	ExpeditionTable::RowData* pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(GetExpID());
	if(NULL == pConf)
	{
		SSError << " find not conf expid = " << GetExpID() << END;
		return ;
	}
	
	std::vector<Role*> countRoles;
	TeamCostCtrl pTeamCostCtrl;
	for(size_t i = 0; i < m_allMemberIDs.size(); ++i)
	{
		UINT64 roleID = m_allMemberIDs[i];
		Role* pRole = m_pScene->FindSceneRoleNotWatcher(roleID);
		if(NULL == pRole)
		{
			continue;
		}
		if(!IsHelper(roleID))
		{
			if(FESTIVAL_TEAM_SCENE == pConf->Type)//节日副本 次数另计
			{
				pRole->Get<StageMgr>()->DoAfterHolidayEnd(m_pScene->GetSceneTemplateID());
			}
			else
			{
				if(UseTicket(roleID))
				{
					pTeamCostCtrl.TakeUseTicket(pRole, pConf);	
				}
				else
				{
					pTeamCostCtrl.CheckAddTeamDayCount(pRole, pConf, TEAM_CCT_WIN);
					countRoles.push_back(pRole);
				}
			}
		}
		else
		{
			pRole->Get<CTeamRecord>()->AddHelpCount(pConf->Type);
		}
	}

	if(!countRoles.empty())
	{
		HTeamSynExtraData syn;
		syn.TrySynExtraDataToTs(countRoles, pConf->Type);
	}
}

void SceneTeam::AddRobot(const Vector3& pos, float face)
{
	if(m_addRobotFlag)//只调用一次
	{
		//SSWarn << " try add robot repeat expid = " << GetExpID() << END;
		return;
	}
	m_addRobotFlag = true;
	ExpeditionTable::RowData *pRow = ExpeditionConfigMgr::Instance()->GetExpeditionConf(GetExpID());
	if (pRow == NULL)
	{
		return;
	}
	for(auto it = m_robotSummarys.begin(); it != m_robotSummarys.end(); ++it)
	{
		EnterSceneInfo entersceneinfo = SceneConfig::Instance()->GetEnterSceneInfo(KKSG::ENTER_SCENE_SWITCH, m_pScene->GetSceneTemplateID());
		DummyRole* pDummyRole = DummyRoleManager::Instance()->Create((*it), pos, face, (UINT32)(KKSG::FightRole), entersceneinfo, SCENE_ALLY);
		if(pDummyRole)
		{
			if(ENDLESSABYSS_SCENE == pRow->Type)
			{
				AttributePkMgr::Instance()->SetPkAttribute(pDummyRole, m_pScene->GetSceneType());
			}
			m_pScene->AddUnitAndNotify(pDummyRole);

			SetRobotAttrPrecent(pDummyRole, pRow, (*it)->GetID() != 0);
		}
		else//主城宕机重启后 有概率出现
		{
			SSWarn << " create dummy failed id = " << (*it)->GetRobotID() << END;
		}
	}
}

void SceneTeam::SetRobotAttrPrecent(DummyRole* pDummy, ExpeditionTable::RowData *pRow, bool rolecopy)
{
	if(NULL == pDummy || NULL == pRow)
	{
		return ;
	}
	double lifescale = pRow->RobotLifePercent;
	double atkscale = 1.0;
	if(!rolecopy)
	{
		atkscale = pRow->RobotAtkPercent;
	}

	pDummy->ResetBaseAttr(lifescale, atkscale);
}

//void SceneTeam::InitRoleBuffs()
//{
//	for(int i = 0; i < m_KKSGData.rolebufflist_size(); ++i)
//	{
//		const KKSG::RoleBuff& rb = m_KKSGData.rolebufflist(i);
//		UINT64 roleid = rb.roleid();
//		for(int j = 0; j < rb.bufflist_size(); ++j)
//		{
//			BuffDesc bd;
//			bd.buffID = rb.bufflist(j).buffid();
//			bd.BuffLevel = rb.bufflist(j).bufflevel();
//			m_roleBuffs[roleid].push_back(bd);
//			SSDebug << "roldid = " << roleid << " buffID = " << bd.buffID << " bufflevel = " << bd.BuffLevel << END;
//		}
//	}
//}

void SceneTeam::AddRoleBuffs(Role* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	for(int i = 0; i < m_KKSGData.rolebufflist_size(); ++i)
	{
		UINT64 roleID = m_KKSGData.rolebufflist(i).roleid();
		if(pRole->GetID() != roleID)
		{
			continue;
		}
		XAddBuffActionArgs args;
		args.singleBuff.buffID = m_KKSGData.rolebufflist(i).buff().buffid();
		args.singleBuff.BuffLevel = m_KKSGData.rolebufflist(i).buff().bufflevel();
		args.Caster = pRole->GetID();
		SSDebug << __FUNCTION__ << " name = " << pRole->GetName() << " buffid = " << args.singleBuff.buffID << " bufflevel = " << args.singleBuff.BuffLevel << END;
		pRole->GetXObject()->DispatchAction(&args);
	}
	//UINT64 roleid = pRole->GetID();
	//auto it = m_roleBuffs.find(roleid);
	//if(it == m_roleBuffs.end())
	//{
	//	return ;
	//}
	//auto buffs = it->second;
	//for(size_t i = 0; i < buffs.size(); ++i)
	//{
	//	XAddBuffActionArgs args;
	//	args.singleBuff.buffID = buffs[i].buffID;
	//	args.singleBuff.BuffLevel = buffs[i].BuffLevel;
	//	args.Caster = pRole->GetID();
	//	SSDebug << __FUNCTION__ << " name = " << pRole->GetName() << " buffid = " << args.singleBuff.buffID << " bufflevel = " << args.singleBuff.BuffLevel << END;
	//	pRole->GetXObject()->DispatchAction(&args);
	//}
}

void SceneTeam::OnClientLoaded(Role* pRole)
{
	CheckSetCanDestroy();

	if(NULL == pRole)
	{
		return ;
	}
	AddRoleBuffs(pRole);

	//只支持一个玩家 一个机器人 的情况
	AddRobot(pRole->GetXObject()->GetPosition_p(), pRole->GetXObject()->GetFaceDegree());
}

//void SceneTeam::TeamChangeScene(UINT32 sceneID)
//{
//	//跨场景不支持机器人， 条件不满足的玩家进行也会带到下一个场景
//	const std::list<Role *>& roles = m_pScene->GetAllRoles();
//	if (roles.empty())
//	{
//		return;
//	}
//
//	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(sceneID);
//	if (pSceneInfo == NULL)
//	{
//		LogWarn("team change scene failed!");
//		return;
//	}
//
//	std::vector<Role*> rightRoles;
//	std::map<Role*, int> errorRoles;
//	ChangeSceneRoleFilter(pSceneInfo, roles, rightRoles, errorRoles);
//
//	for (auto iter = errorRoles.begin(); iter != errorRoles.end(); ++iter)
//	{
//		PtcG2C_GSErrorNotify oErrorPtc;
//		oErrorPtc.m_Data.set_errorno(iter->second);
//		iter->first->Send(oErrorPtc);
//	}
//	if (rightRoles.empty())
//	{
//		return;
//	}
//
//	KKSG::SceneSwitchData oData;
//	KKSG::SceneSwitchTeamData& data = *oData.mutable_teamdata();
//	data.CopyFrom(m_KKSGData);	
//	data.set_cursceneindex(m_KKSGData.cursceneindex() + 1);
//	if (m_pScene->GetSceneType() == KKSG::SCENE_DRAGON)
//	{
//		const DragonNestTable::RowData* info = DragonConfig::Instance()->GetDragonTableData(GetExpID());
//		if (info)
//		{
//			const DragonNestTable::RowData* infoNew = DragonConfig::Instance()->GetDragonTableData(info->DragonNestType, info->DragonNestDifficulty, info->DragonNestWave + 1);
//			if (infoNew)
//			{
//				data.mutable_teamsyndata()->set_expid(infoNew->DragonNestID);//龙本exp改变
//			}
//		}
//	}
//	CSceneSwitch::ChangeScene(rightRoles, sceneID, oData);
//}

void SceneTeam::ChangeSceneDataCh(UINT32 expID, KKSG::SceneSwitchTeamData& data, const std::map<Role*, int>& roleRet, const CtCsInfo& info)
{
	auto& tsdata = *data.mutable_teamsyndata();
	tsdata.set_expid(expID);
	tsdata.set_leaderid(info.leaderID);

	for(int i = 0; i < tsdata.members_size(); ++i)
	{
		auto& mem = *tsdata.mutable_members(i);		
		KKSG::TeamMemberType oldMemType = mem.memtype();

		if(oldMemType == KKSG::TMT_HELPER && info.notHelper.find(mem.roleid()) != info.notHelper.end())
		{
			mem.set_memtype(KKSG::TMT_NORMAL);
		}
		if(oldMemType == KKSG::TMT_NORMAL && info.beHelper.find(mem.roleid()) != info.beHelper.end())
		{
			mem.set_memtype(KKSG::TMT_HELPER);
		}
	}
}

void SceneTeam::ChangeSceneFailNtf(const std::map<Role*, int>& roleRet)
{
	if(IsDragonExp())//失败原因通知
	{
		for(auto it = roleRet.begin(); it != roleRet.end(); ++it)
		{
			Role* pRole = it->first;
			int ret = it->second;	
			if(KKSG::ERR_SUCCESS != ret)
			{
				PtcG2C_GSErrorNotify oErrorPtc;
				oErrorPtc.m_Data.set_errorno(UINT32(ret));
				pRole->Send(oErrorPtc);
			}
		}
	}
}

void SceneTeam::ChagneSceneDeathRole(const std::vector<Role*>& roles, KKSG::SceneSwitchData& data)
{
	bool isHardDragon = false;
	bool isCompeteDragon = false;
    const DragonNestTable::RowData* table = DragonConfig::Instance()->GetDragonTableData(GetExpID());
	if (table && table->DragonNestDifficulty == 2)
	{
		isHardDragon = true;
	}

	if (IsCompeteDragon())
	{
		isCompeteDragon = true;
	}

	if (isHardDragon || isCompeteDragon)
	{
		for (auto iter = roles.begin(); iter != roles.end(); ++iter)
		{
			if ((*iter)->IsDead())
			{
				data.add_deadroles((*iter)->GetID());
			}
		}
	}
}

void SceneTeam::ChangeSceneTotalCostTime(KKSG::SceneSwitchData& data)
{
	if (IsCompeteDragon())
	{
		time_t cost = m_pScene->CostTime();
		data.set_alreadycosttime(cost > 0 ? cost : 0);
	}
}

int SceneTeam::ChangeSceneCheck(std::vector<Role*>& roles, const std::map<Role*, int>& roleRet, const CtCsInfo& info)
{
	if(roleRet.size() != roles.size())
	{
		return KKSG::ERR_FAILED;
	}
	roles.clear();	
	for(auto it = roleRet.begin(); it != roleRet.end(); ++it)
	{
		if(KKSG::ERR_SUCCESS == it->second)
		{
			roles.push_back(it->first);
		}
	}

	if(IsGoddessOrEndlessA())
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(info.leaderID);
		if(NULL == pRole)
		{
			return KKSG::ERR_FAILED;
		}
		auto it = roleRet.find(pRole);
		if(it == roleRet.end())
		{
			return KKSG::ERR_FAILED;
		}
		if(KKSG::ERR_SUCCESS != it->second)
		{
			return it->second;
		}
	}

	if(roles.empty())
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

void SceneTeam::ChangeSceneRoleFilter(UINT32 expID, const std::vector<Role*>& input, std::map<Role*, int>& roleRet, CtCsInfo& info)
{
	auto pMgr = ExpeditionConfigMgr::Instance();
	auto pConf = pMgr->GetExpeditionConf(expID);

	for (auto iter = input.begin(); iter != input.end(); ++iter)
	{
		Role* pRole = *iter;

		roleRet[pRole] = KKSG::ERR_SUCCESS;
		UINT64 roleID = pRole->GetID();

		if(NULL == pConf)
		{
			roleRet[pRole] = KKSG::ERR_FAILED;
			continue;
		}
		if(UseTicket(roleID))//不考虑巢穴门票 因为巢穴门票不参加再次挑战
		{
			roleRet[pRole] = KKSG::ERR_FAILED;
			continue;
		}

		auto synMem = GetSynMember(roleID);
		if(NULL == synMem)
		{
			roleRet[pRole] = KKSG::ERR_FAILED;
			continue;
		}

		if(GSConfig::Instance()->IsCrossGS() && DRAGON_TEAM_SCENE == pConf->Type && 
			!ServerCrossDataMgr::Instance()->IsServerDragonSceneFinished(synMem->serverid(), pMgr->GetExpRandomScene(expID)))
		{
			roleRet[pRole] = KKSG::ERR_TEAM_SERVER_NOT_MATCH;
			continue;
		}

		TeamEnterData beHelperData(true, KKSG::TMT_HELPER);
		TeamEnterData notHelperData(true, KKSG::TMT_NORMAL);

		TeamRequest req(pRole);
		int beHelpRet = req.CheckEnterCondtion(pRole, expID, &beHelperData);
		int notHelpRet = req.CheckEnterCondtion(pRole, expID, &notHelperData);

		if(IsHelper(roleID))
		{
			if(info.leaderID == roleID)//只有无尽深渊和女神试炼会出现这种情况
			{
				if(KKSG::ERR_SUCCESS == notHelpRet)
				{
					info.notHelper.insert(roleID);
				}
				roleRet[pRole] = notHelpRet;
			}
			else
			{
				roleRet[pRole] = beHelpRet;
			}
		}
		else
		{
			if(KKSG::ERR_SUCCESS == notHelpRet)
			{
				roleRet[pRole] = notHelpRet;
			}
			else 
			{
				if(IsGoddessOrEndlessA())
				{
					if(KKSG::ERR_SUCCESS == beHelpRet)
					{
						info.beHelper.insert(pRole->GetID());						
					}
					roleRet[pRole] = beHelpRet;
				}
				else
				{
					roleRet[pRole] = notHelpRet;
				}
			}
		}

	}
}

void SceneTeam::OnSceneCompleteForDragonStatistics(bool win)
{
    // 龙本队友
    if (m_pScene->GetSceneType() == KKSG::SCENE_DRAGON)
    {
        DragonGroupMgr::Instance()->RecordDragonGroup(m_allMemberIDs, m_pScene, win);
    }
}

//void SceneTeam::ChangeSceneRoleFilter(UINT32 sceneID, const std::vector<Role*>& input, std::vector<Role*>& rightRoles, std::map<Role*, int>& errorRoles)
//{
//	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(sceneID);
//	if (NULL == pSceneInfo)
//	{
//		SSError << " find sceneinfo failed sceneid = " << sceneID << END;
//		return;
//	}
//	if(NULL == m_pConf)
//	{
//		return;
//	}
//	for (auto iter = input.begin(); iter != input.end(); ++iter)
//	{
//		Role* pRole = *iter;
//
//        int ret = KKSG::ERR_SUCCESS;
//        if(InAirShip())
//        {
//			rightRoles.push_back(pRole);
//			continue;
//        }
//
//        if(IsDragonExp())
//        {
//            EnterSceneNormalCondition normalCond;
//            ret = normalCond.CanEnterScene(pRole, pSceneInfo);
//            if(ret != KKSG::ERR_SUCCESS)
//            {
//                errorRoles[pRole] = ret;
//                continue;
//            }
//        }
//		//助战
//		if (IsHelper(pRole->GetID()) || UseTicket(pRole->GetID()))
//		{
//			rightRoles.push_back(pRole);
//			continue;
//		}
//
//		//体力判断
//		EnterSceneFatigueCondition cond;
//		ret = cond.CanEnterScene(pRole, pSceneInfo);
//		if (KKSG::ERR_SUCCESS != ret)
//		{
//			errorRoles[pRole] = ret;
//			continue;
//		}
//		//次数检测
//		if (TEAM_CCT_NONE != m_pConf->CostCountType && pRole->Get<CTeamRecord>()->GetTeamLeftCountToday(m_pConf->Type) <= 0)
//		{
//			errorRoles[pRole] = KKSG::ERR_SCENE_TODYCOUNTLIMIT;
//			continue;
//		}		
//
//		rightRoles.push_back(pRole);
//	}
//}

void SceneTeam::OnSceneComplete(bool win)
{
	ResultNtf(win);

	ExpeditionTable::RowData *pRow = ExpeditionConfigMgr::Instance()->GetExpeditionConf(GetExpID());
	if (pRow == NULL)
	{
		return;
	}

	if (win)
	{
		WinAddTeamDayCount();
		CheckKingBackReward();
	}

	if (m_pScene->GetSceneType() == KKSG::SCENE_NEST || m_pScene->GetSceneType() == KKSG::SCENE_WEEK_NEST)
	{
		LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_NEST, m_pScene->GetSceneID(), 0, 0, 0);
	}
	if (m_pScene->GetSceneType() == KKSG::SCENE_DRAGON)
	{
		LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_DRAGON, m_pScene->GetSceneID(), 0, 0, 0);
	}
}

void SceneTeam::UpdateTowerInfo(int time)
{
	++m_towerInfo.reachTopFloor;
	m_towerInfo.bestTime += time;
}

void SceneTeam::AddSyncSceneFetchReward(Role* role, int itemID, int itemCount)
{
	if (!role)
	{
		return;
	}
	bool find = FindMember(role->GetID());
	if (!find)
	{
		return;
	}
	m_memberInfos[role->GetID()].m_mapTowerFetchReward[itemID] += itemCount;
}

std::map<int, int>* SceneTeam::GetSyncSceneFetchReward(Role* role)
{
	if (!role)
	{
		return NULL;
	}

	bool find = FindMember(role->GetID());
	if (!find)
	{
		return NULL;
	}

	return &m_memberInfos[role->GetID()].m_mapTowerFetchReward;

}

bool SceneTeam::FindMember(UINT64 roleID)
{
	foreach(i in m_allMemberIDs)
	{
		if ((*i) == roleID)
		{
			return true;
		}
	}

	return false;
}

int SceneTeam::GetMemberCountWithoutRobot()
{
	return m_allMemberIDs.size();
}

bool SceneTeam::IsHelper(UINT64 roleID)
{
	auto pSynMem = GetSynMember(roleID);
	if(pSynMem)
	{
		return (pSynMem->memtype() == KKSG::TMT_HELPER);
	}
	return false;
}

int SceneTeam::UseTicket(UINT64 roleID)
{
	auto pSynMem = GetSynMember(roleID);
	if(pSynMem)
	{
		return (pSynMem->memtype() == KKSG::TMT_USETICKET);
	}
	return 0;
}

void SceneTeam::GetTeammates(UINT64 unitID, std::vector<UINT64>& IDS)
{
	IDS.clear();
	bool flag = false;
	auto& synTeam = m_KKSGData.teamsyndata();
	for(int i = 0; i < synTeam.members_size(); ++i)
	{
		if(synTeam.members(i).roleid() == unitID)
		{
			flag = true;
			continue;
		}
		IDS.push_back(synTeam.members(i).roleid());
	}
	if(!flag)
	{
		IDS.clear();
	}
}

void SceneTeam::InitCostTeam(bool iswin, const std::vector<Role*>& roles, std::map<UINT64, ItemDesc>& reward)
{
	if(!IsCostTeam())
	{   
		SSInfo<<"team is not cost team"<<END;
		return;
	}   

	Role* leader = NULL;
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		if ((*i)->GetID() == GetLeaderID())
		{
			leader = *i;
			break;
		}
	}
	if (NULL == leader)
	{
		SSWarn<<"not find leader, sceneid:"<<m_pScene->GetSceneTemplateID()<<END;
		return;
	}

	//通知一下MS
	PtcG2M_TeamCostLeaderResetNtf ptc;
	ptc.m_Data.set_roleid(leader->GetID());
	leader->SendToMS(ptc);

	if (!iswin)
	{
		leader->Get<CTeamRecord>()->RecoverTeamCostInfo();
		SSInfo<<"not win give cost back to leader, leaderid:"<<leader->GetID()<<END;
	}
	else
	{
		UINT32 expid = 0;
		UINT32 costindex = TeamCostInvalidID;
		if (!leader->Get<CTeamRecord>()->GetTeamCostInfo(expid, costindex))
		{
			SSInfo<<"team is not cost team"<<END;
			return;
		}
		leader->Get<CTeamRecord>()->ResetTeamCostInfo(TCRR_WIN);
		const Sequence<UINT32, 3>* ret = ExpeditionConfigMgr::Instance()->GetTeamCost(expid, costindex);
		if (NULL == ret)
		{
			SSWarn<<"not find conf expid:"<<expid<<" index:"<<costindex<<END;
			return;
		}
		// calculate reward
		///> 钻石团
		if (1 == ret->seq[0])
		{
			ItemDesc item;
			if (NULL != ret && m_allMemberIDs.size() > 1)
			{
				item.itemID = ret->seq[1];
				item.itemCount = ret->seq[2]/(m_allMemberIDs.size() - 1);	
			}
			if (0 != item.itemCount)
			{
				for (auto i = roles.begin(); i != roles.end(); ++i)
				{
					Role* role = *i;
					if (role->GetID() != GetLeaderID())
					{
						reward[(*i)->GetID()] = item;
					}
				}
			}
		}
		///> 龙币团
		else if (2 == ret->seq[0])
		{
			for (auto i = roles.begin(); i != roles.end(); ++i)
			{
				Role* role = *i;
				if (role->GetID() == GetLeaderID())
				{
					continue;
				}
				role->Get<CTeamRecord>()->AddTeamCostCount();
				ItemDesc item = ExpeditionConfigMgr::Instance()->GetTeamCostReward(role->Get<CTeamRecord>()->GetTeamCostCount(), expid);
				reward[role->GetID()] = item;
			}
		}
		SSInfo<<"win dispatch cost to team members, leaderid:"<<leader->GetID()<<END;
	}
}

void SceneTeam::InitDroppableInfo()
{
	DroppableInfo info;
	info.nonHelperCount = GetRoleCountWithoutHelper();
	info.totalCount = m_allMemberIDs.size();

	m_pScene->GetSceneDoodad()->InitDroppableInfo(info);
}

bool SceneTeam::IsCostTeam()
{
	return (m_KKSGData.teamsyndata().teamcost() != TeamCostInvalidID);
}

UINT32 SceneTeam::GetNextExpID()
{
	if(InAirShip())
	{
		return GetExpID();
	}
	else if(IsGoddessOrEndlessA())
	{
		return GetExpID();
	}
	else if(IsDragonExp())
	{
		const DragonNestTable::RowData* info = DragonConfig::Instance()->GetDragonTableData(GetExpID());
		if (info)
		{
			if (info->DragonNestDifficulty == 2)
			{
				if (GetNextSceneID(GetExpID()))
				{
					return GetExpID();
				}

				return 0;
			}

			const DragonNestTable::RowData* infoNew = DragonConfig::Instance()->GetDragonTableData(info->DragonNestType, info->DragonNestDifficulty, info->DragonNestWave + 1);
			if (infoNew)
			{
				return infoNew->DragonNestID;
			}
		}
	}
	else if (IsCompeteDragon())
	{
		if (GetNextSceneID(GetExpID()))
		{
			return GetExpID();
		}

		return 0;
	}
	else
	{
		SSWarn << " unknow get next expid curexp = " << GetExpID() <<" curscene = " << m_pScene->GetSceneID() << END;
		return 0;
	}
	return 0;
}

UINT32 SceneTeam::GetNextSceneID(UINT32 nextExpID)
{
	bool hardDragon = false;
	const DragonNestTable::RowData* info = DragonConfig::Instance()->GetDragonTableData(nextExpID);
	if (info)
	{
		if (info->DragonNestDifficulty == 2)
		{
			hardDragon = true;
		}
	}
	if((hardDragon || IsCompeteDragon()) && !InAirShip())//特殊龙本
	{
		auto pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(nextExpID);
		if(NULL == pConf)
		{
			return 0;
		}
		//必须满足 RandomSceneID和SceneID 一一对应
		for(size_t i = 0; i < pConf->RandomSceneIDs.size(); ++i)
		{
			UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetRandomScene(pConf->RandomSceneIDs[i]);
			if(m_pScene->GetSceneTemplateID() == sceneID)
			{
				if(i + 1 < pConf->RandomSceneIDs.size())
				{
					return ExpeditionConfigMgr::Instance()->GetRandomScene(pConf->RandomSceneIDs[i+1]);
				}
				break;
			}
		}
	}
	else
	{
		return ExpeditionConfigMgr::Instance()->GetExpRandomScene(nextExpID);
	}
	return 0;
}

UINT32 SceneTeam::GetRoleCountWithoutHelper()
{
	UINT32 count = 0;
	auto& synTeam = m_KKSGData.teamsyndata();
	for(int i = 0; i < synTeam.members_size(); ++i)
	{
		auto& synmem = synTeam.members(i);
		if(IsHelper(synmem.roleid()))
		{
			continue;
		}
		if(Unit::GetUnitType(synmem.roleid()) != KKSG::Category_Role)
		{
			continue;
		}
		++count;
	}
	return count;
}

bool SceneTeam::IsCostCount()
{
	if(NULL == m_pConf)
	{
		return false;
	}
	return (m_pConf->CostCountType != TEAM_CCT_NONE);
}

bool SceneTeam::IsGoddessOrEndlessA()
{
	if(NULL == m_pConf)
	{
		return false;
	}
	return (m_pConf->Type == ENDLESSABYSS_SCENE || m_pConf->Type == GODDESS_TEAM_SCENE);
}

bool SceneTeam::IsDragonExp()
{
    if(NULL == m_pConf)
    {
        return false;
    }
    return (m_pConf->Type == DRAGON_TEAM_SCENE);	
}

bool SceneTeam::IsCompeteDragon()
{
    if(NULL == m_pConf)
    {
        return false;
    }
    return (m_pConf->Type == TEAM_TYPE_COMPETE_DRAGON);	
}

bool SceneTeam::InAirShip()
{
	if(m_pScene->GetSceneType() == KKSG::SCENE_AIRSHIP)
    {
        return true;
    }
    return false;
}

const KKSG::TeamSynMember* SceneTeam::GetSynMember(UINT64 roleID)
{
	for(int i = 0; i < m_KKSGData.teamsyndata().members_size(); ++i)
	{
		auto& synmem = m_KKSGData.teamsyndata().members(i);
		if(synmem.roleid() == roleID)
		{
			return &synmem;
		}
	}
	return NULL;
}

void SceneTeam::InfoPrint()
{
	SSInfo << " exp = " << GetExpID() << " leader = " << GetLeaderID() << END;
	const auto& data = m_KKSGData.teamsyndata();	
	for(int i = 0; i < data.members_size(); ++i)
	{
		const auto& mem = data.members(i);
		SSInfo << " role = " << mem.roleid() << " type = " << mem.memtype() << END;
	}
}

void SceneTeam::ResultNtf(bool iswin)
{
	//只用于跨服通知	
	if(!IsTsTeam()) 
	{
		return;
	}
	PtcG2M_TeamTranGs2Ms ptc;
	ptc.m_Data.set_type(TEAM_TRANS_BATTLE_RESULT);
	*ptc.m_Data.mutable_teamalldata() = m_KKSGData.teamsyndata();
	const auto& roles = m_pScene->GetAllRoles();
	for(auto it = roles.begin(); it != roles.end(); ++it)
	{
		if((*it)->SendToMS(ptc))//利用其中一个玩家转发给TS
		{
			break;
		}
	}
}

bool SceneTeam::IsTsTeam()
{
	return HTeam::IsTsTeam(GetTeamID());
}

void SceneTeam::Update()
{
	time_t now = GameTime::GetTime();
	if(now - m_createTime >= 60)//一分钟之后
	{
		CheckSetCanDestroy();
	}
}

bool SceneTeam::IsAllCross()
{
	bool bFromOneServer = true;
	if (GSConfig::Instance()->IsCrossGS())
	{
		UINT32 serverID = 0;
		for (auto iter = m_allMemberIDs.begin(); iter!= m_allMemberIDs.end(); iter++)
		{
			auto member = GetSynMember(*iter);
			if (!member)
			{
				bFromOneServer = false;
				break;
			}

			if (!serverID)
			{
				serverID = member->serverid();
			}
			else if(serverID != member->serverid())
			{
				bFromOneServer = false;
				break;
			}
		}
	}
	return bFromOneServer;

}

void SceneTeam::CheckSetCanDestroy()
{
	if(!m_pScene->GetDontDestory())
	{
		return;
	}
	m_pScene->SetDontDestory(false);
}

void SceneTeam::CheckKingBackReward()
{
	SSDebug << __FUNCTION__ << END;

	bool haveKingBack = false;
	std::vector<Role*> roles;
	for(size_t i = 0; i < m_allMemberIDs.size(); ++i)
	{
		UINT64 roleID = m_allMemberIDs[i];
		Role* pRole = m_pScene->FindSceneRoleNotWatcher(roleID);
		if(NULL == pRole)
		{
			continue;
		}
		if(pRole->IsKingBack())
		{
			if(!IsHelper(roleID))
			{
				haveKingBack = true;
			}
		}
		else
		{
			roles.push_back(pRole);
		}
	}

	if(!haveKingBack)
	{
		SSDebug << " have not kingback role " << END;
		return ;
	}

	for(size_t i = 0; i < roles.size(); ++i)
	{
		Role* pRole = roles[i];	
		if(pRole->Get<RoleMisc>()->GetKingBackRewardCount() < GetGlobalConfig().TeamKingBackRewardCountLimit)
		{
			pRole->Get<RoleMisc>()->AddKingBackRewardCount();

			BagGiveItemTransition give(pRole);
			give.SetReason(ItemFlow_Stage, ItemFlow_KingBackGive);
			const auto& rewards = GetGlobalConfig().TeamKingBackRewards;
			for(size_t j = 0; j < rewards.size(); ++j)
			{
				give.GiveItem(rewards[j][0], rewards[j][1], true);
			}
			give.NotifyClient();
			SSDebug << " kingback reward give name = " << pRole->GetName() << END;
		}
		else
		{
			pRole->ErrorCodeNtf(KKSG::ERR_KINGBACK_REWARD_COUNT_LIMIT, true);
			SSDebug << " kingback reward limit name = " << pRole->GetName() << END;
		}
	}
}
