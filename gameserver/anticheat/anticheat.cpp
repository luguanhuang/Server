#include "pch.h"
#include "anticheat/anticheat.h"
#include "scene/sceneconfig.h"
#include "timeutil.h"
#include "unit/role.h"
#include "gamelogic/XLevelSpawnMgr.h"


bool AntiCheat::IsCheat(Role* poRole, UINT32 dwSceneTmplID, UINT32 dwTimeBegin, const KKSG::CliAntiCheatInfo& roInfo)
{
	if(poRole == NULL) return false;

	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(dwSceneTmplID);
	if(pSceneInfo == NULL)
	{
		LogError("Invalid scene template id [%u]", dwSceneTmplID);
		return false;
	}
	
	if(!poRole->IsSameBattleStamp(roInfo.battlestamp()))
	{
		LogWarn("Role %llu anti-cheat check failed, map [%u], battleStamp:%s(wrong) != %s(correct)", poRole->GetID(), dwSceneTmplID, roInfo.battlestamp().c_str(), poRole->GetBattleStamp().c_str());
		return true;
	}

	UINT32 dwCurrTime = TimeUtil::GetTime();
	UINT32 dwTimeUsed = (dwCurrTime >= (UINT32)poRole->GetEnterSceneTime() ? dwCurrTime - (UINT32)poRole->GetEnterSceneTime() : 0);
	if(dwTimeUsed < pSceneInfo->m_pConf->MinPassTime)
	{
		LogWarn("Role %llu anti-cheat check failed, map [%u], pass time [%u], required time [%u]", poRole->GetID(), dwSceneTmplID, dwTimeUsed, pSceneInfo->m_pConf->MinPassTime);
		return true;
	}

	/*UINT32 dwMonsterTotalHp = 0;
	double dLeftHp = poRole->GetAttr(TOTAL_CurrentHP) - (double)roInfo.totalhurt() + (double)roInfo.totalrecovery();
	double dDiff = (double)roInfo.currenthp() - dLeftHp;
	if(dDiff > 500.0)
	{
		LogWarn("Role %llu anti-cheat check failed, map [%u], left hp [%u], required left-hp [%u]", poRole->GetID(), dwSceneTmplID, roInfo.currenthp(), (UINT32)dLeftHp);
		return true;
	}

	if(roInfo.totaldamage() < dwMonsterTotalHp)
	{
		return true;
	}*/

	//std::vector<XLevelWave*> oWaveVec;
	//XLevelSpawnMgr::Instance()->GetWaveInfo(dwSceneTmplID, oWaveVec);
	if(roInfo.monsterrfstimes_size() == 0)
	{
		LogWarn("Role %llu anti-cheat check failed, map [%u], monster refresh times [%d]", poRole->GetID(), dwSceneTmplID, roInfo.monsterrfstimes_size());
		return true;
	}

	/*UINT32 dwMinReqTime = 0;
	for(INT32 i = 0; i < roInfo.monsterrfstimes_size(); ++i)
	{
		UINT32 dwTimeBegin = roInfo.monsterrfstimes(i);
		dwMinReqTime += (UINT32)oWaveVec[i]->m_Time;
		if(dwTimeBegin < dwMinReqTime)
		{
			LogWarn("Role %llu anti-cheat check failed, map [%u], monster wave [%d] refresh time [%u sec], required times [%u sec]", poRole->GetID(), dwSceneTmplID, i, dwTimeBegin, dwMinReqTime);
			return true;
		}
	}*/

	return false;
}