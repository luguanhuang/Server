#include "pch.h"
#include <time.h>
#include "FatigueMgr.h"
#include "globalconfig.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
#include "role/ptcg2c_fatiguerecovertimenotify.h"
#include "payv2Record.h"

INSTANCE_SINGLETON(FatigueMgr)

FatigueMgr::FatigueMgr()
{

}

FatigueMgr::~FatigueMgr()
{

}

bool FatigueMgr::Init()
{
	return true;
}

void FatigueMgr::Uninit()
{
}

int FatigueMgr::AddFatigue(Role *pRole, int addFatigue, int fatigueEnum, int maxFatigue)
{
	int fatigue = (int)pRole->Get<Bag>()->CountVirtualItem(fatigueEnum);
	if (addFatigue > 0 && fatigue < maxFatigue)
	{
		int realAdd = 0;
		BagGiveItemTransition transition(pRole);
		transition.SetReason(ItemFlow_Fatigue, ItemFlow_Fatigue_Add);
		if (addFatigue + fatigue < maxFatigue)
		{
			realAdd = addFatigue;
			transition.GiveItem(fatigueEnum, addFatigue);
		}
		else
		{
			realAdd = maxFatigue - fatigue;
			transition.GiveItem(fatigueEnum, maxFatigue - fatigue);
		}

		transition.NotifyClient();

		return realAdd;
	}

	return 0;
}

void FatigueMgr::AddLoginFatigue(Role *pRole)
{
	if(pRole == NULL) return;

	UINT32 dwLastTime = pRole->GetRoleAllInfo().extrainfo().lastfatiguerecovertime();
	UINT32 dwNow = TimeUtil::GetTime();
	int timeInMinute = (int)((dwNow >= dwLastTime ? dwNow - dwLastTime : 0) / 60);
	int addFatigue = timeInMinute /  GetGlobalConfig().FatigueRegeneration;
	if(addFatigue == 0)
	{
		return;
	}

	pRole->GetRoleAllInfoPtr()->mutable_extrainfo()->set_lastfatiguerecovertime(dwNow);
	int nAdded = AddFatigue(pRole, addFatigue, FATIGUE,  GetGlobalConfig().MaxRecoverFatigue + pRole->Get<CPayV2Record>()->GetFatigueLimit());
	if (nAdded > 0)
	{
		pRole->OnRecoverFatigue(nAdded, dwLastTime, dwNow);
	}
}

void FatigueMgr::NotifyFatigueRecoverTime(Role *pRole)
{
	// 不用发了
}

void FatigueMgr::CheckFatigueRecover(Role* pRole)
{
	if(pRole == NULL) return;
	if(pRole->GetCurrScene() == NULL) return;

	UINT32 dwNow = TimeUtil::GetTime();
	UINT32 dwLastTime = pRole->GetRoleAllInfo().extrainfo().lastfatiguerecovertime();
	if(dwNow >= pRole->GetRoleAllInfo().extrainfo().lastfatiguerecovertime() +  GetGlobalConfig().FatigueRegeneration * 60)
	{
		pRole->GetRoleAllInfoPtr()->mutable_extrainfo()->set_lastfatiguerecovertime(dwNow);

		int nAdded = FatigueMgr::AddFatigue(pRole, 1, FATIGUE,  GetGlobalConfig().MaxRecoverFatigue+pRole->Get<CPayV2Record>()->GetFatigueLimit());
		if(nAdded > 0)
		{
			pRole->OnRecoverFatigue(nAdded, dwLastTime, dwNow);
		}
	}
}