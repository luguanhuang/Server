#include "pch.h"
#include "lineselector.h"
#include "scene.h"
#include "scenemanager.h"
#include "role/role.h"
#include "worldboss/worldbossmgr.h"
#include "worldboss/worldbossline.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"

LineSelectorHall LineSelectorHall::GlobalSelectorHall;
UINT32 LineSelectorHall::SelectLine(UINT32 destid, LineDest& dest)
{
	dest.line = MAIN_HALL_GS_LINE;
	return KKSG::ERR_SUCCESS;
}

LineSelectorDynamic LineSelectorDynamic::GlobalSelectorDynamic;
UINT32 LineSelectorDynamic::SelectLine(UINT32 destid, LineDest& dest)
{
	return KKSG::ERR_FAILED;
}

LineSelectorWorldBoss LineSelectorWorldBoss::GlobalSelectorWorldBoss;
UINT32 LineSelectorWorldBoss::SelectLine(UINT32 destid, LineDest& dest)
{
	UINT32 ret = WorldBossMgr::Instance()->CanEnter(1 == m_roles.size() ? m_roles[0]->GetID() : 0);
	if (ret != KKSG::ERR_SUCCESS)
	{
		dest.line = INVALID_LINE_ID;
	}
	else
	{
		if (1 != m_roles.size())
		{
			SSWarn<<"enter world boss role count error"<<END;
			return KKSG::ERR_FAILED;
		}
		const WorldBossLine* line = WorldBossLineMgr::Instance()->GetAvaliableLine(m_roles[0]->GetID());
		if (NULL == line)
		{
			///> 等GS创建好场景后在把玩家拉到场景中
			dest.isdelay = true;
			for(auto i = m_roles.begin(); i != m_roles.end(); ++i)
			{
				CRole* poRole = *i;
				WorldBossLineMgr::Instance()->RoleEnterDelay(poRole->GetID());
				SSInfo<<"Role:"<<poRole<<" enter worldboss, delay begin to change gs_"<<poRole->GetGsLine()<<" to gs_"<<dest.line<<END;
			}
		}
		else
		{
			dest.line = line->gsline;
			dest.destid = line->sceneid;
		}
	}
	return ret;
}

LineSelectorSpecific LineSelectorSpecific::GlobalSelectorSpecific;
UINT32 LineSelectorSpecific::SelectLine(UINT32 destid, LineDest& dest)
{
	dest.destid = destid;
	CScene* poScene = CSceneManager::Instance()->GetScene(destid);
	if(poScene == NULL)
	{
		LogError("Can't find dest scene %u", destid);
		dest.line = INVALID_LINE_ID;
		return KKSG::ERR_FAILED;
	}
	else
	{
		dest.line = poScene->GetLine();
		dest.mapid = poScene->GetMapID();
		return KKSG::ERR_SUCCESS;
	}
}

LineSelectorGuildBoss LineSelectorGuildBoss::GlobalSelectorGuildBoss;
UINT32 LineSelectorGuildBoss::SelectLine(UINT32 destid, LineDest& dest)
{
	if (m_roles.size() == 0)
	{
		return KKSG::ERR_FAILED;
	}
	if (m_roles.size() > 1)
	{
		SSWarn << "enter guild boss role list more than one!"<<END;
	}
	if (m_roles[0]->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(m_roles[0]->GetID());
	if (!CGuildBossMgr::Instance()->CanEnterScene(guildId))
	{
		return KKSG::ERR_FAILED;
	}

	if (CGuildBossMgr::Instance()->IsJoinOtherBefoer(m_roles[0]->GetID()))
	{
		return KKSG::ERR_FAILED;
	}

	UINT32 sceneId = CGuildBossMgr::Instance()->GetSceneId(guildId, m_roles[0]->GetID()); 
	CScene* pCScene = CSceneManager::Instance()->GetScene(sceneId);
	if (pCScene == NULL)
	{
		dest.isdelay = true;
		for(auto i = m_roles.begin(); i != m_roles.end(); ++i)
		{
			CRole* poRole = *i;
			CGuildBossMgr::Instance()->RoleEnterDelay(poRole->GetID());
			SSInfo<<"Role:"<<poRole<<" enter guildboss, delay begin to change gs_:"<<poRole->GetGsLine()<<" to gs_:"<<dest.line<<" and guildid_:"<<guildId<<END;
		}
	}
	else
	{
		dest.line = pCScene->GetLine();
		CGuildBossMgr::Instance()->AddGuildBossSceneNum(sceneId, m_roles[0]->GetID());
	}

	dest.destid = sceneId;
	return KKSG::ERR_SUCCESS;
}

LineSelectorOwner LineSelectorOwner::GlobalSelectorOwner;
UINT32 LineSelectorOwner::SelectLine(UINT32 destid, LineDest& dest)
{
	if (1 != m_roles.size())
	{
		return KKSG::ERR_FAILED;
	}
	///> 合法性检测
	if (m_mapid == GUILD_MAP_ID)
	{
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(m_roles[0]->GetID());
		m_uid = NULL == guild ? 0 : guild->GetID();
	}
	else if (m_mapid == FAMILYGARDEN_MAP_ID)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roles[0]->GetID());
		if (NULL == summary)
		{
			m_uid = 0;
		}
		else
		{
			m_roles[0]->OnEnterOwnScene(m_uid);
		}
	}
	else if (dest.type == KKSG::SCENE_LEISURE)
	{
		///> 休闲场景 以mapid为uid
		m_uid = m_mapid;
	}
	if (0 == m_uid)
	{
		return KKSG::ERR_FAILED;
	}
	OneOwnerScene* onescene = CSceneManager::Instance()->FindOwnerScene(m_mapid, m_uid);
	if (NULL == onescene || INVALID_SCENE_ID == onescene->sceneid)
	{
		for (auto i = m_roles.begin(); i != m_roles.end(); ++i)
		{
			CSceneManager::Instance()->EnterOwnerScene(m_mapid, m_uid, (*i)->GetID());
		}
		dest.isdelay = true;
	}
	else
	{
		onescene->ResetState();
		CScene* scene = CSceneManager::Instance()->GetScene(onescene->sceneid);
		if (NULL != scene)
		{
			dest.destid = scene->GetID();
			dest.line = scene->GetLine();
		}
		else
		{
			SSWarn<<"scene is NULL, sceneuid:"<<onescene->sceneid<<END;
		}
	}
	return KKSG::ERR_SUCCESS;
}
