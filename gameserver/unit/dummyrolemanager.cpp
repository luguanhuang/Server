#include "pch.h"
#include "dummyrolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "dummyrole.h"
#include <functional>
#include "foreach.h"


INSTANCE_SINGLETON(DummyRoleManager)

DummyRoleManager::DummyRoleManager()
{

}

DummyRoleManager::~DummyRoleManager()
{

}

bool DummyRoleManager::Init()
{
	return true;
}

void DummyRoleManager::Uninit()
{

}

bool DummyRoleManager::Add(DummyRole *pAlly)
{
	auto ret = m_DummyRoles.insert(std::make_pair(pAlly->GetID(), pAlly));
	if(!ret.second)
	{
		return false;
	}
	return true;
}

void DummyRoleManager::Remove(DummyRole *pAlly)
{
	m_DummyRoles.erase(pAlly->GetID());
	pAlly->Uninit();
	delete pAlly;
}

DummyRole * DummyRoleManager::Find( UINT64 uID )
{
	auto it = m_DummyRoles.find(uID);
	return (it == m_DummyRoles.end()) ? NULL : it->second;
}

DummyRole * DummyRoleManager::Create(RoleSummary *pRobotSummary, const Vector3& pos, const float face, UINT32 fightGroup, const EnterSceneInfo& info, DummyType type)
{
	DummyRole *pAlly = new DummyRole();
	pAlly->Init(pRobotSummary, pos, face, fightGroup, info, type);
	if(!Add(pAlly))
	{
		pAlly->Uninit();
		delete pAlly;
		return NULL;
	}
	return pAlly;
}

void DummyRoleManager::Update(float delta)
{
	foreach(i in m_DummyRoles)
	{
		i->second->Update(delta);
	}
}

UINT64 DummyRoleManager::CreateNewID()
{
	static UINT32 dummyRoleID = 1;

	UINT64 gsLine = (UINT64)GSConfig::Instance()->GetLine();
	UINT64 gsLineBit = (gsLine << 32);//33--->40

	UINT64 Hitbit = KKSG::Category_DummyRole;
	Hitbit = ((Hitbit << UnitTypeOffset) | gsLineBit | dummyRoleID);
	++dummyRoleID;
	return Hitbit;
}

