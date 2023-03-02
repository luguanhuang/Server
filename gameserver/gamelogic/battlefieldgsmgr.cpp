#include "pch.h"
#include "battlefieldgsmgr.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "battlefield/ptcg2c_battlefieldreadyinfontf.h" 
#include "table/globalconfig.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "gamelogic/MultActivityMgr.h"


INSTANCE_SINGLETON(BattleFieldGSMgr);

BattleFieldGSMgr::BattleFieldGSMgr()
{
	m_nRound	= 0;
	m_nTime		= 0;
	m_nSceneID  = 0;
	m_bEnd      = false;
	m_timerId	= CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);
}

BattleFieldGSMgr::~BattleFieldGSMgr()
{
	CTimerMgr::Instance()->KillTimer(m_timerId);
}

bool BattleFieldGSMgr::Init()
{
	bool bRet =CheckFile();
	if(!bRet)
		return false;
	LoadFile();
	return true;
}

void BattleFieldGSMgr::Uninit()
{
	ClearFile();
}

bool BattleFieldGSMgr::LoadFile()
{
	bool bRet = CheckFile();
	if (!bRet)
		return false;
	ClearFile();

	if( !m_Table.LoadFile("table/BattleFieldPointReward.txt"))
	{
		SSWarn<<"load file BattleFieldPointReward.txt failed"<<END;
		return false;
	}
	return true;

}

void BattleFieldGSMgr::ClearFile()
{
	m_Table.Clear();
}

bool BattleFieldGSMgr::CheckFile()
{
	BattleFieldPointReward tmpTable;
	bool bRet = tmpTable.LoadFile("table/BattleFieldPointReward.txt");
	return bRet;
}


void BattleFieldGSMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	SyncTime();
	if(GameTime::GetTime()>=MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_BATTLEFIELD))
	{
		Scene *pScene = SceneManager::Instance()->FindBySceneID(m_nSceneID);
		if (pScene&&pScene->GetSceneBFReady())
		{
			pScene->SetDontDestory(false);
		}
	}
}

void BattleFieldGSMgr::SetTime(UINT32 nTime)
{
	m_nTime = nTime;

}

void BattleFieldGSMgr::SetSceneID(UINT32 nSceneID)
{
	m_nSceneID = nSceneID;
	LogInfo("SetSceneID=%d",nSceneID);
}

UINT32 BattleFieldGSMgr::GetTime()
{
	return m_nTime;
}


void BattleFieldGSMgr::Start()
{
	m_bEnd =  false;

}

void BattleFieldGSMgr::ResetTime(UINT32 nRound,UINT32 nNextMatchTime,bool bFlag,bool bEnd)
{
	LogInfo("ResetTime m_nRound =%u  m_nNextMatchTime=%u flag=%d end=%u", nRound,nNextMatchTime,bFlag,bEnd);
	m_nTime		  = GameTime::GetTime()+GetGlobalConfig().BattleFieldMatchTime;
	m_nRound	  = nRound;
	Scene *pScene = SceneManager::Instance()->FindBySceneID(m_nSceneID);
	if (pScene&&pScene->GetSceneBFReady())
	{
		if (!bEnd)
		{
			PtcG2C_BattleFieldReadyInfoNtf ntf;
			ntf.m_Data.set_round(m_nRound);
			ntf.m_Data.set_time(GetGlobalConfig().BattleFieldMatchTime);
			pScene->Broadcast(ntf);
		}
		else
		{

			m_bEnd  = true;
			PtcG2C_BattleFieldReadyInfoNtf ntf;
			ntf.m_Data.set_end(true);
			pScene->Broadcast(ntf);
		}
	}
}

void BattleFieldGSMgr::OnEnterScene(Role* role)
{
	if (m_bEnd)
	{
		PtcG2C_BattleFieldReadyInfoNtf ntf;
		ntf.m_Data.set_end(true);
		role->Send(ntf);
	}else
	{
		UINT32 nTime =  GameTime::GetTime();
		PtcG2C_BattleFieldReadyInfoNtf ntf;
		ntf.m_Data.set_round(m_nRound);
		if (nTime < m_nTime)
		{	
			ntf.m_Data.set_time(m_nTime-nTime);
		}else
		{
			ntf.m_Data.set_time(GetGlobalConfig().BattleFieldMatchTime);
		}
		role->Send(ntf);
	}

}


void BattleFieldGSMgr::SyncTime()
{
	if (!m_bEnd)
	{
		Scene *pScene = SceneManager::Instance()->FindBySceneID(m_nSceneID);
		if (pScene&&pScene->GetSceneBFReady())
		{
			UINT32 nTime =  GameTime::GetTime();
			PtcG2C_BattleFieldReadyInfoNtf ntf;
			ntf.m_Data.set_round(m_nRound);
			if (nTime <= m_nTime)
			{	
				ntf.m_Data.set_time(m_nTime-nTime);
				pScene->Broadcast(ntf);
			}

		}
	}

}
