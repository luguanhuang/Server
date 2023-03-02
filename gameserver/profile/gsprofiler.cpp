#include "pch.h"
#include "gsprofiler.h"
#include "config.h"
#include "scene/scenemanager.h"
#include "unit/rolemanager.h"
#include "unit/enemymanager.h"
#include "unit/dummyrolemanager.h"
#include "gamelogic/rolesummarymgr.h"

INSTANCE_SINGLETON(CGsProfiler)


CGsProfiler::CGsProfiler()
{
}

CGsProfiler::~CGsProfiler()
{
}

bool CGsProfiler::Init()
{
	StartTimer();

	std::stringstream ss;
	ss << "gameserver_";
	if(GSConfig::Instance()->IsCrossGS())
	{
		ss << "cross_";
	}
	ss << GSConfig::Instance()->GetLine();
	SetFileName(ss.str());
	return true;
}

void CGsProfiler::Uninit()
{
	StopTimer();
}

void CGsProfiler::DoProfile(FILE* fp)
{
	fprintf(fp, "Scene count: %u\n", SceneManager::Instance()->GetSceneCount());

	fprintf(fp, "Hall Role count: %u\n", SceneManager::Instance()->GetHallRoleCount());

	fprintf(fp, "Role count: %u\n", RoleManager::Instance()->GetRoleCount());
	fprintf(fp, "Enemy count: %u\n", EnemyManager::Instance()->GetCount());
	fprintf(fp, "Dummy Role count: %u\n", DummyRoleManager::Instance()->GetCount());

	fprintf(fp, "Role Summary count: %u\n", RoleSummaryMgr::Instance()->GetCount());
	fprintf(fp, "Role Summary queue count: %u\n", RoleSummaryMgr::Instance()->GetQueueCount());
}