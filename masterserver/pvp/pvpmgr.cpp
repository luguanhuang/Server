#include "pch.h"
#include "pvpmgr.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammember.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "table/expeditionconfigmgr.h"
#include "table/globalconfig.h"
#include "role/role.h"
#include <time.h>
#include "network/gslink.h"
#include "util/XCommon.h"
#include "notice/noticemgr.h"
#include "team/teamforceop.h"
#include "team/teammatchmgr.h"
#include "cross/crosszonestatus.h"
#include "role/rolemanager.h"
#include "other/matchhandler.h"

INSTANCE_SINGLETON(PvpMgr);


PvpMgr::PvpMgr()
{
}

PvpMgr::~PvpMgr()
{

}

bool PvpMgr::Init()
{
	m_worldMatchNum = 0;

	return true;
}

void PvpMgr::Uninit()
{
}

int PvpMgr::GetInMatchCount(CRole* pRole)
{
	return m_worldMatchNum;
}
