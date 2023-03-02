#include "pch.h"
#include "ctrlprofiler.h"
#include "scene/scenemanager.h"
#include "role/rolemanager.h"
#include "queuing/queuingmgr.h"


INSTANCE_SINGLETON(CControlProfiler)


CControlProfiler::CControlProfiler()
{
}

CControlProfiler::~CControlProfiler()
{
}

bool CControlProfiler::Init()
{
	StartTimer();
	SetFileName("controlserver");
	return true;
}

void CControlProfiler::Uninit()
{
	StopTimer();
}

void CControlProfiler::DoProfile(FILE* fp)
{
	fprintf(fp, "Scene count: %u\n", CSceneManager::Instance()->GetSceneNum());
	fprintf(fp, "OnlieRole count: %u\n", CRoleManager::Instance()->GetRoleCount());
	fprintf(fp, "Waitting count: %u\n", QueuingMgr::Instance()->GetWaittingCount());
}
