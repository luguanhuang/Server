#include "pch.h"
#include "vsstate.h"
#include "scene.h"
#include "scenepk.h"
#include <time.h>
#include "table/globalconfig.h"
#include "gamelogic/pvpmgr.h"
#include "scenepvp.h"
#include "scenegmf.h"
#include "sceneleague.h"

VsWaitLoad VsWaitLoad::GlobalVsWaitLoad;

void VsWaitLoad::OnEnter(SceneVsBase* holder)
{
	holder->SetStateStartTime(time(NULL));
	holder->OnEnterWaitLoad();
}

void VsWaitLoad::OnLeave(SceneVsBase* holder)
{
	
}

void VsWaitLoad::OnTick(SceneVsBase* holder)
{
	VsStateType state = holder->IsAllLoad();
	if(VS_STATE_DUMMY != state)
	{
		holder->SetState(state);
		return;
	}

	if (holder->GetStateStartTime() + holder->GetLoadWaitTime() <= time(NULL))
	{
		state = holder->OnLoadTimeOut();//ÇÐ»»µ½ÆäËü×´Ì¬
		if(VS_STATE_DUMMY != state)
		{
			holder->SetState(state);
		}
	}
}

///////////////////////////////////////////////////////////////////////
VsPrepare321 VsPrepare321::GlobalVsPrepare321;

void VsPrepare321::OnEnter(SceneVsBase* holder)
{
	holder->SetStateStartTime(time(NULL));
	holder->OnEnterP321();
	holder->Fight321Ntf(0);
}

void VsPrepare321::OnLeave(SceneVsBase* holder)
{
	holder->Fight321Ntf(1);
}

void VsPrepare321::OnTick(SceneVsBase* holder)
{
	VsStateType state = holder->CheckP321End();
	if(VS_STATE_DUMMY != state)
	{
		holder->SetState(state);
	}
}

////////////////////////////////////////////////////////////////////

VsInFight VsInFight::GlobalVsInFight;

void VsInFight::OnEnter(SceneVsBase* holder)
{
	holder->SetStateStartTime(time(NULL));
	holder->SceneReady();
	holder->OnEnterFight();
}

void VsInFight::OnLeave(SceneVsBase* holder)
{

}

void VsInFight::OnTick(SceneVsBase* holder)
{
	VsStateType state = holder->CheckFightEnd();
	if(VS_STATE_DUMMY != state)
	{
		holder->SetState(state);	
		return ;
	}
}

//////////////////////////////////////////////////////////////////

VsEnd VsEnd::GlobalVsEnd;

void VsEnd::OnEnter(SceneVsBase* holder)
{
	holder->SetStateStartTime(time(NULL));
	holder->OnStateEnd();
}

void VsEnd::OnLeave(SceneVsBase* holder)
{

}

//////////////////////////////////////////////////////////////////

void VsAfterFight::OnEnter( SceneVsBase* holder )
{
	holder->SetStateStartTime(time(NULL));
	holder->OnEnterAfterFight();
}

void VsAfterFight::OnLeave( SceneVsBase* holder )
{
	
}

void VsAfterFight::OnTick( SceneVsBase* holder )
{
	VsStateType state = holder->CheckAfterFightEnd();
	if(VS_STATE_DUMMY != state)
	{
		holder->SetState(state);
	}
}

VsAfterFight VsAfterFight::GlobalVsAfterFight;


void VsReadyGmf::OnEnter(SceneVsBase* holder)
{
	SSDebug << __FUNCTION__ << END;
	holder->SetStateStartTime(time(NULL));
	auto gbh = holder->GetHandler<SceneGWB>();
	if(gbh)
	{
		gbh->OnJoinStart();
	}
}

void VsReadyGmf::OnLeave(SceneVsBase* holder)
{
	SSDebug << __FUNCTION__ << END;
	holder->SetStateStartTime(time(NULL));
}

void VsReadyGmf::OnTick(SceneVsBase* holder)
{
	auto gbh = holder->GetHandler<SceneGWB>();
	if(NULL == gbh)
	{
		return ;
	}
	UINT32 lefttime = gbh->JoinLeftTime();
	if(!gbh->EnterStoped() && lefttime <= gbh->EnterLessTime())
	{
		gbh->OnEnterStop();
	}

	if(0 == lefttime)
	{
		VsStateType state = gbh->OnJoinStop();
		if(VS_STATE_DUMMY != state)
		{
			holder->SetState(state);
		}
	}
}

VsReadyGmf VsReadyGmf::GlobalVsReadyGmf;

void VsReadyLeague::OnEnter(SceneVsBase* holder)
{
	SSDebug << __FUNCTION__ << END;
	holder->SetStateStartTime(time(NULL));
	auto l = holder->GetHandler<SceneLeague>();
	if (l)
	{
		l->OnReadyStart();
	}
}

void VsReadyLeague::OnLeave(SceneVsBase* holder)
{
	SSDebug << __FUNCTION__ << END;
	holder->SetStateStartTime(time(NULL));
	auto l = holder->GetHandler<SceneLeague>();
	if (l)
	{
		l->OnReadyEnd();
	}
}

void VsReadyLeague::OnTick(SceneVsBase* holder)
{
	auto l = holder->GetHandler<SceneLeague>();
	if (l == NULL)
	{
		return;
	}
	VsStateType state = l->CheckReadyEnd();
	if(VS_STATE_DUMMY != state)
	{
		holder->SetState(state);
	}
}

VsReadyLeague VsReadyLeague::GlobalVsReadyLeague;
