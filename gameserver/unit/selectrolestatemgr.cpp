#include "pch.h"
#include "selectrolestatemgr.h"
#include "cmdline.h"

INSTANCE_SINGLETON(SelectRoleStateMgr)

SelectRoleStateMgr::SelectRoleStateMgr()
{

}

SelectRoleStateMgr::~SelectRoleStateMgr()
{

}

bool ShowSelectRoleState(const CmdLineArgs &args, std::string &outputMessage)
{
	SelectRoleStateMgr::Instance()->Dump();
	return true;
}

bool SelectRoleStateMgr::Init()
{
	CmdLine::RegistCmd("srs", ShowSelectRoleState);
	return true;
}

void SelectRoleStateMgr::Uninit()
{

}

SessionState SelectRoleStateMgr::FindSessionState(UINT64 session)
{
	auto i = m_SessionStates.find(session);
	return i == m_SessionStates.end() ? SESSIONSTATE_NOT_EXIST : i->second;
}

void SelectRoleStateMgr::SetSessionState(UINT64 session, SessionState state)
{
	m_SessionStates[session] = state;
}

void SelectRoleStateMgr::RemoveSessionState(UINT64 session)
{
	m_SessionStates.erase(session);
}

bool SelectRoleStateMgr::TryChangeState(UINT64 session, SessionState newState)
{
	auto state = FindSessionState(session);
	if (state == SESSIONSTATE_FREE || state == SESSIONSTATE_NOT_EXIST)
	{
		SetSessionState(session, newState);
		return true;
	}
	else
	{
		return false;
	}
}


const char *StateStr(SessionState state)
{
#define CASE(x) case SESSIONSTATE_##x: return #x
	switch(state)
	{
		CASE(NOT_EXIST);
		CASE(FREE);
		CASE(CREATE_ROLE);
		CASE(SELECT_ROLE);
		CASE(INGAME);
	default: break;
	}
#undef CASE
	return "UNKNOW";
}

void SelectRoleStateMgr::Dump()
{
	int k = 0;
	SSInfo << "NO. | Session | State\n";
	for (auto i = m_SessionStates.begin(); i != m_SessionStates.end(); ++i, ++k)
	{
		SSInfo << k << ". " << Hex(i->first) << " " << StateStr(i->second) << "\n";
	}

	SSInfo << END;
}

