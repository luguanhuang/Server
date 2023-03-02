#ifndef __VSSTATE_H__
#define __VSSTATE_H__

#include "utility/state.h"

//编号不可变
enum VsRoleState
{
	VS_ROLE_STATE_NONE = 0,
	VS_ROLE_WAITLOAD = 1,
	VS_ROLE_LOADED = 2,
	VS_ROLE_DIE = 3,
	VS_ROLE_QUIT = 4,
};

//加新的事件请通知 sherlockzhang
enum VsEventType
{
	VS_EVENT_ROLE_LOADED,
	VS_EVENT_ROLE_DIE,
	VS_EVENT_ROLE_QUIT,
	VS_EVENT_ROLE_LOADING_QUIT,//加载中退出(例如:顶号)
	VS_EVENT_ROLE_REVIVE,//复活

	VS_EVENT_DUMMY
};

struct VsEvent
{
	VsEventType type;
	UINT64 roleid;

	VsEvent()
	{
		type = VS_EVENT_DUMMY;
		roleid = 0;
	}
};

enum VsStateType
{
	VS_STATE_WAIT_LOAD,
	VS_STATE_PREPARE321,
	VS_STATE_IN_FIGHT,
	VS_STATE_AFTER_FIGHT,
	VS_STATE_END,

	VS_STATE_READY_GMF,//gmf

	VS_STATE_READY_LEAGUE,	// 联赛准备状态

	VS_STATE_DUMMY,
};

class SceneVsBase;

//////////////////////////////////////////////////////////////////

class VsWaitLoad : public State<SceneVsBase, VsEvent>
{
public:
	VsWaitLoad() { SetStateType(VS_STATE_WAIT_LOAD); }

	virtual void OnEnter(SceneVsBase* holder);
	virtual void OnLeave(SceneVsBase* holder);
	virtual void OnTick(SceneVsBase* holder);

	static VsWaitLoad GlobalVsWaitLoad;
};

class VsPrepare321: public State<SceneVsBase, VsEvent>
{
public:
	VsPrepare321() { SetStateType(VS_STATE_PREPARE321); }

	virtual void OnEnter(SceneVsBase* holder);
	virtual void OnLeave(SceneVsBase* holder);
	virtual void OnTick(SceneVsBase* holder);

	static VsPrepare321 GlobalVsPrepare321;
};

class VsInFight : public State<SceneVsBase, VsEvent>
{
public:
	VsInFight() { SetStateType(VS_STATE_IN_FIGHT); }

	virtual void OnEnter(SceneVsBase* holder);
	virtual void OnLeave(SceneVsBase* holder);
	virtual void OnTick(SceneVsBase* holder);

	static VsInFight GlobalVsInFight;
};

class VsAfterFight : public State<SceneVsBase, VsEvent>
{
public:
	VsAfterFight(){SetStateType(VS_STATE_AFTER_FIGHT);}

	virtual void OnEnter( SceneVsBase* holder );

	virtual void OnLeave( SceneVsBase* holder );

	virtual void OnTick( SceneVsBase* holder );

	static VsAfterFight GlobalVsAfterFight;
};

class VsEnd : public State<SceneVsBase, VsEvent>
{
public:
	VsEnd() { SetStateType(VS_STATE_END); }

	virtual void OnEnter(SceneVsBase* holder);
	virtual void OnLeave(SceneVsBase* holder);

	static VsEnd GlobalVsEnd;
};

///////////////////////////////////////////////////////////////

class VsReadyGmf : public State<SceneVsBase, VsEvent>
{
public:
	VsReadyGmf(){ SetStateType(VS_STATE_READY_GMF); }

	virtual void OnEnter( SceneVsBase* holder );

	virtual void OnLeave( SceneVsBase* holder );

	virtual void OnTick( SceneVsBase* holder );

	static VsReadyGmf GlobalVsReadyGmf;
};

class VsReadyLeague : public State<SceneVsBase, VsEvent>
{
public:
	VsReadyLeague() { SetStateType(VS_STATE_READY_LEAGUE); }

	virtual void OnEnter(SceneVsBase* holder);
	virtual void OnLeave(SceneVsBase* holder);
	virtual void OnTick(SceneVsBase* holder);

	static VsReadyLeague GlobalVsReadyLeague;
};

///////////////////////////////////////////////////////////////
#endif
