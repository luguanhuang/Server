#include "pch.h"
#include "pb/project.pb.h"
#include "logutil.h"
#include "worldbossline.h"
#include "worldbossmgr.h"
#include "network/gslink.h"
#include "scene/scene.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "table/globalconfig.h"
#include "scene/sceneswitch.h"

#define WOLRDBOSS_LINE_MAX ( GetGlobalConfig().WorldBossSceneLimit)

INSTANCE_SINGLETON(WorldBossLineMgr);

WorldBossLineMgr::WorldBossLineMgr()
{

}

WorldBossLineMgr::~WorldBossLineMgr()
{

}

bool WorldBossLineMgr::Init()
{
	return true;
}

void WorldBossLineMgr::Uninit()
{

}

void WorldBossLineMgr::Clear()
{
	for (auto i = m_sceneid2line.begin(); i != m_sceneid2line.end(); ++i)
	{
		delete i->second;
		i->second = NULL;
	}
	m_sceneid2line.clear();
	m_roleid2line.clear();
	for (auto i = m_prelines.begin(); i != m_prelines.end(); ++i)
	{
		delete *i;
		*i = NULL;
	}
	m_prelines.clear();
	m_waitlist.clear();

}

const WorldBossLine* WorldBossLineMgr::GetAvaliableLine(UINT64 roleid)
{
	auto i = m_roleid2line.find(roleid);
	if (i != m_roleid2line.end())
	{
		return i->second;
	}
	for (auto i = m_sceneid2line.begin(); i != m_sceneid2line.end(); ++i)	
	{
		WorldBossLine* line = i->second;
		if ((int)(line->m_roleids.size()) < WOLRDBOSS_LINE_MAX)
		{
			line->m_roleids.push_back(roleid);
			m_roleid2line[roleid] = line;
			return line;
		}
	}
	return NULL;
}

void WorldBossLineMgr::RoleEnterDelay(UINT64 roleid)
{
	auto i = m_waitlist.find(roleid);
	if (i != m_waitlist.end())
	{
		SSWarn<<"in preline "<<roleid<<END;
		return;
	}
	// find a pre line
	WorldBossLine* line = NULL;
	for (auto i = m_prelines.begin(); i != m_prelines.end(); ++i)
	{
		WorldBossLine* templine = *i;
		if ((int)templine->m_roleids.size() < WOLRDBOSS_LINE_MAX)
		{
			line = templine;
			break;
		}
	}
	// create scene
	if (NULL == line)
	{
		line = CreateScene();
		m_prelines.push_back(line);
	}
	// put in pre line
	m_waitlist.insert(roleid);
	line->m_roleids.push_back(roleid);
}

void WorldBossLineMgr::OnLineReady(UINT32 sceneid, UINT32 gsline)
{
	if (m_prelines.empty())
	{
		SSWarn<<"prelines empty"<<END;
	}
	WorldBossLine* line = m_prelines.front();
	line->gsline = gsline;
	line->sceneid = sceneid;

	m_sceneid2line[sceneid] = line;

	for (auto i = line->m_roleids.begin(); i != line->m_roleids.end(); ++i)
	{
		UINT64 roleid = *i;
		// enter scene
		CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
		if (NULL != role)
		{
			EnterScene(role, sceneid, gsline);
		}

		m_roleid2line[roleid] = line;

		auto j = m_waitlist.find(roleid);
		if (j != m_waitlist.end())
		{
			m_waitlist.erase(j);
		}
	}
	m_prelines.pop_front();

	SSInfo<<"create succeed worldboss scene:"<<sceneid<<" line:"<<gsline<<END;
}

WorldBossLine* WorldBossLineMgr::CreateScene()
{
	WorldBossLine* line = new WorldBossLine();
	// create scene 
	BossInfo* info = WorldBossMgr::Instance()->GetBossInfo(); 
	KKSG::CreateBattleParam param;
	KKSG::WorldBossBornInfo* borninfo = param.mutable_worldboss();
	borninfo->set_enemyid(info->dwEnemyId);
	borninfo->set_attackpercent(info->fAttackPercent);
	borninfo->set_maxhp(info->maxhp);
	borninfo->set_currenthp(info->currenthp);
	CSceneCreator::Instance()->CreateBattleScene(WORLDBOSS_MAP_ID, this, param); 
	return line;
}

void WorldBossLineMgr::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT32 nErrCode = res.result();
	if (nErrCode != KKSG::ERR_SUCCESS || NULL == scene)
	{
		SSWarn<<"create scene failed"<<END;
		return;
	}
	OnLineReady(scene->GetID(), scene->GetLine());
}

void WorldBossLineMgr::EnterScene(CRole* role, UINT32 sceneid, UINT32 gsline)
{
	KKSG::SceneSwitchData data;
	CSceneSwitch::EnterScene(role, sceneid, data);
	LogInfo("Role [%llu] begin to change gs_%u to gs_%u", role->GetID(), role->GetGsLine(), gsline);
}
