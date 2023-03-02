#include "pch.h"
#include "roleinfoinit.h"
#include "unit/calcattribute.h"
#include "unit/combatattribute.h"
#include "unit/selectrolestatemgr.h"
#include "table/ProfessionMgr.h"
#include "table/PlayerLevelManager.h"
#include "table/OpenSystemMgr.h"
#include "table/globalconfig.h"
#include "gamelogic/bag.h"
#include "scene/sceneconfig.h"
#include "gamelogic/skilllevel.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "gamelogic/AchivementActivityManager.h"
#include "util/XCommon.h"
#include "utility/BitSet.h"
#include "gamelogic/rolefashion.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"
#include <time.h>
#include "table/PushTableConfig.h"
#include "tablemgr/XEntityInfoLibrary.h"

void CRoleInfoInit::FirstInitRoleInfo(KKSG::RoleAllInfo& roRoleInfo)
{
	/*
	UINT32 dwTimes = roRoleInfo.extrainfo().logintimes();
	++dwTimes;
	roRoleInfo.mutable_extrainfo()->set_logintimes(dwTimes);
	if(dwTimes > 1)
	{
		return;
	}

	// INIT Role Brief
	KKSG::RoleAllInfo *poRoleInfo = &roRoleInfo;

	poRoleInfo->mutable_brief()->set_level(1);
	poRoleInfo->mutable_brief()->set_exp(0);
	poRoleInfo->mutable_brief()->set_sceneid( GetGlobalConfig().InitialReturnSceneID);
	poRoleInfo->mutable_brief()->set_tutorialbits(0);

	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo( GetGlobalConfig().InitialReturnSceneID);
	poRoleInfo->mutable_brief()->mutable_position()->set_x(pSceneInfo->m_pConf->StartPos[0][0]);
	poRoleInfo->mutable_brief()->mutable_position()->set_y(pSceneInfo->m_pConf->StartPos[0][1]);
	poRoleInfo->mutable_brief()->mutable_position()->set_z(pSceneInfo->m_pConf->StartPos[0][2]);
	poRoleInfo->mutable_brief()->set_face(pSceneInfo->m_pConf->StartRot[1]);

	auto *levelExp = PlayerLevelManager::Instance()->GetLevelInfo(2);
	poRoleInfo->mutable_brief()->set_maxexp(levelExp->Exp);
	poRoleInfo->mutable_brief()->set_offlinetime(0);

	poRoleInfo->mutable_stages()->set_day(0);

	poRoleInfo->mutable_lottery()->set_lastdrawtime(0);
	poRoleInfo->mutable_lottery()->set_onedrawcount(0);
	poRoleInfo->mutable_lottery()->set_minimumrewardcount(XRandom::randInt( GetGlobalConfig().SpriteDrawGoodRange[0],  GetGlobalConfig().SpriteDrawGoodRange[1] + 1));
	poRoleInfo->mutable_lottery()->set_goldminimumrewardcount(XRandom::randInt( GetGlobalConfig().SpriteGoldDrawGoodRange[0],
																			    GetGlobalConfig().SpriteGoldDrawGoodRange[1] + 1));
	CDataFieldSet oSet;

	// INIT Attribute
	CalcRoleAttribute InitRoleAttribute;
	CombatAttribute combatAttribute;
	InitRoleAttribute.SetupBasicAttribute(1, roRoleInfo.brief().type(), &combatAttribute);
	InitRoleAttribute.CalcAttributeConvert(&combatAttribute, roRoleInfo.brief().type());
	InitRoleAttribute.CalcPowerPoint(&combatAttribute, 0, NULL, roRoleInfo.brief().type());
	combatAttribute.Save(poRoleInfo, oSet);

	// INIT Skill
	InitSkill *pInitSkill = XEntityInfoLibrary::Instance()->GetInitSkill(roRoleInfo.brief().type());
	if (pInitSkill)
	{
		RoleSkillSet skillMgr(NULL);
		skillMgr.LearnInitSkill(pInitSkill);
		skillMgr.Save(poRoleInfo, oSet);
	}

	// INIT System
	BitSet<MAX_SYSTEM_ID> openSystemID;
	const std::vector<int> &initSystems = OpenSystemMgr::Instance()->OpendSystemWhenCreateRole();
	for (UINT32 j = 0; j < initSystems.size(); ++j)
	{
		openSystemID.Set(initSystems[j], true);
	}
	poRoleInfo->mutable_system()->set_system(openSystemID.ToString());

	// INIT Bag
	Bag bag(NULL);
	bag.AddAnyItem(ItemDesc(FATIGUE, GetGlobalConfig().MaxRecoverFatigue, false), NULL);

	//被IDIP初始化的角色，保留了钻石;
	//if (roRoleInfo.virtualitems_size()>DIAMOND && roRoleInfo.virtualitems(DIAMOND))
	//{
	//	bag.AddAnyItem(ItemDesc(DIAMOND, (UINT32)roRoleInfo.virtualitems(DIAMOND), false), NULL);
	//}
	bag.Save(poRoleInfo, oSet);

	RoleFashion rf(NULL);
	rf.Changed();
	rf.Save(poRoleInfo, oSet);

	poRoleInfo->mutable_brief()->set_inittime((UINT32)time(NULL));
	poRoleInfo->mutable_extrainfo()->set_lastleveluptime(TimeUtil::GetTime());
	poRoleInfo->mutable_extrainfo()->set_lastfatiguerecovertime(TimeUtil::GetTime());


	// 推送配置
	const std::map<UINT32, std::string>& pushCfg = PushTableConfig::Instance()->GetTypeKey();
	for (auto it = pushCfg.begin(); it != pushCfg.end(); ++it)
	{
		poRoleInfo->mutable_config()->add_type(it->second);
		poRoleInfo->mutable_config()->add_value("1");
	}

	// 任务
	//TaskRecord::OnCreateRole(poRoleInfo);
	*/
}

