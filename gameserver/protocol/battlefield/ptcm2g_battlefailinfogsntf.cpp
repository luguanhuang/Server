#include "pch.h"
#include "battlefield/ptcm2g_battlefailinfogsntf.h"
#include "battlefield/ptcg2c_battlefieldreadyinfontf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "gamelogic/battlefieldgsmgr.h"


// generate by ProtoGen at date: 2017/9/12 15:15:50

void PtcM2G_BattleFailInfoGsNtf::Process(UINT32 dwConnID)
{
	for (int i = 0; i < m_Data.roleids_size(); i++)
	{
		Role *pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleids(i));
		if (pRole)
		{
			Scene * pScene = pRole->GetCurrScene();
			if (pScene&&pScene->GetSceneType()==KKSG::SCENE_BATTLEFIELD_READY)
			{

				SceneBFReady* pBfReady= pScene->GetSceneBFReady();
				if (pBfReady)
				{
					PtcG2C_BattleFieldReadyInfoNtf ntf;
					ntf.m_Data.set_round(BattleFieldGSMgr::Instance()->GetRound());
					ntf.m_Data.set_time(GetGlobalConfig().BattleFieldMatchTime);
					ntf.m_Data.set_failed(true);
					pRole->Send(ntf);
				}

			}
		}
	}
}
