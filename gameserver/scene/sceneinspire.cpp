#include "pch.h"
#include "sceneinspire.h"
#include "scene.h"
#include "scenegmf.h"
#include "scenegwb.h"
#include "unit/unit.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "table/InspireConfig.h"
#include "table/PowerPointMgr.h"
#include "gamelogic/notice.h"

SceneInspire::SceneInspire(Scene* scene)
{
	m_pScene = scene;
}

SceneInspire::~SceneInspire()
{

}

bool SceneInspire::NeedInspire(Scene* pScene)
{
	if(NULL == pScene)
	{
		return false;
	}

	if(InspireConfig::Instance()->HaveSceneType(pScene->GetSceneType()))
	{
		return true;
	}

	return false;
}

void SceneInspire::HandlerReq(Role* pRole, const KKSG::InspireArg& oArg, KKSG::InspireRes& oRes)
{
	if(NULL == pRole)
	{
		return;
	}
	switch(m_pScene->GetSceneType())
	{
	case KKSG::SCENE_GMF:
	case KKSG::SCENE_GPR:
		{
			GwbInspire(pRole, oRes);
		}
		break;
	default:
		break;
	}
}

void SceneInspire::GwbInspire(Role* pRole, KKSG::InspireRes& oRes)
{
	oRes.set_errorcode(KKSG::ERR_SUCCESS);
	UINT64 roleid = pRole->GetID();		
	if(!pRole->IsWatcher())
	{
		return; 
	}

	SceneGWB* pGwb = m_pScene->GetGWBHandler();
	if(NULL == pGwb)
	{
		return;
	}
	std::vector<UINT64> addattrIDs;
	UINT64 inspireID = pGwb->GetInspireID(roleid, addattrIDs);

	if(0 == inspireID)
	{
		return ;
	}

	UINT32 coolDownTime = GetCoolDownTime(roleid, GetGlobalConfig().GMFInspireCoolDown);
	if(coolDownTime > 0)
	{
		oRes.set_cooldowntime(coolDownTime);
		//SSDebug << __FUNCTION__ << " cooldwontime = " << coolDownTime << END;
		oRes.set_errorcode(KKSG::ERR_INSPIRE_COOLDOWN);
		return;
	}

	AddOneInspire(roleid, inspireID);
	pGwb->TLogInspireID(pRole, inspireID);

	INT32 dwCount = GetInspireCount(inspireID); 
	const auto& CountConf = GetGlobalConfig().GMFInspireCountConf;
	if(CountConf.find(dwCount) != CountConf.end())
	{
		Notice notice(GuildArenaInspire);
		notice.SendGuild(inspireID);	
	}
	//¼ÓÊôÐÔ
	AddTempAttr(addattrIDs, inspireID, false);

	pGwb->RoomStateToClient();
}

bool SceneInspire::InCoolDown(UINT64 roleid, UINT32 cooldown)
{
	return (GetCoolDownTime(roleid, cooldown) > 0);
}

UINT32 SceneInspire::GetCoolDownTime(UINT64 roleid, UINT32 cooldown)
{
	auto it = m_inspireTime.find(roleid);
	if(it == m_inspireTime.end())
	{
		return 0;
	}
	time_t now = time(NULL);
	if(it->second + cooldown > now)
	{
		return UINT32(it->second + cooldown - now);
	}
	return 0;
}

void SceneInspire::AddOneInspire(UINT64 roleid, UINT64 inspireID)
{
	m_inspireTime[roleid] = time(NULL);

	m_inspireCount[inspireID] += 1;
}

void SceneInspire::GwbOnFightBegin(UINT64 roleID, UINT64 guildID)
{
	std::vector<UINT64> tmp;
	tmp.push_back(roleID);
	AddTempAttr(tmp, guildID, true);
}

void SceneInspire::AddTempAttr(std::vector<UINT64>& addAttrIDs, UINT64 inspireID, bool isfirst)
{
	INT32 dwCount = GetInspireCount(inspireID); 
	std::vector<Sequence<int,2>> addattr;
	InspireConfig::Instance()->GetAddAttr(m_pScene->GetSceneType(), addattr);

	for(size_t i = 0; i < addAttrIDs.size(); ++i)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(addAttrIDs[i]);
		if(NULL == pRole)
		{
			continue;
		}
		for(size_t j = 0; j < addattr.size(); ++j)
		{
			dwCount = isfirst ? dwCount : 1;
			PowerPointMgr::Instance()->AdjustOneAttributeByCoeffcient(pRole, addattr[j].seq[0], (float)(addattr[j].seq[1]*dwCount)/100.0f);
		}
	}
}

INT32 SceneInspire::GetInspireCount(UINT64 inspireID)
{
	auto it = m_inspireCount.find(inspireID);
	if(it == m_inspireCount.end())
	{
		return 0;
	}
	return it->second;
}
