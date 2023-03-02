#ifndef __ALLYMANAGER_H__
#define __ALLYMANAGER_H__

#include "math/XVector3.h"
#include "dummyrole.h"

class DummyRole;
class RoleSummary;

class DummyRoleManager
{
public:

	DummyRoleManager();
	~DummyRoleManager();
	DECLARE_SINGLETON(DummyRoleManager)

public:

	bool Init();
	void Uninit();

	UINT32 GetCount() { return m_DummyRoles.size(); }
	UINT64 CreateNewID();
	bool Add(DummyRole *pAlly);
	void Remove(DummyRole *pAlly);
	DummyRole *Find(UINT64 uID);
	DummyRole *Create(RoleSummary *pRobotSummary, const Vector3& pos, const float face,UINT32 fightGroup, const EnterSceneInfo& info, DummyType type = SCENE_ALLY);
	void Update(float delta);

private:
	std::unordered_map<UINT64, DummyRole *> m_DummyRoles;
};

#endif // __ALLYMANAGER_H__