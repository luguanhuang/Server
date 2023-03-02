#ifndef __SCENEVIEWLINE_H__
#define __SCENEVIEWLINE_H__

#include "sceneline.h"

class SceneViewLine : public SceneLine
{
public:
	SceneViewLine(Grid* grid);
	virtual ~SceneViewLine();

	virtual void Update();
	virtual void Broadcast(Unit* poSender, const CProtocol& roPtc, Unit* pExludeUnit = NULL);
	virtual void BroadcastCanBeSee(Unit* poSender, const CProtocol& roPtc);

	virtual void AddRole(Role* poRole);
	virtual void RemoveRole(Role* poRole);

	virtual void GetCanSee(Role* role, std::vector<Role*>& roles);
	virtual void GetCanBeSee(Role* role, std::vector<Role*>& roles, int limit);

	// calculate view grid
	virtual void UpdateViewGrid(Role* role);

	virtual void FillAll(std::vector<Unit*>& units) { };
	virtual void GetUnitsCanSee(Role* role, std::vector<Unit*>& units) { }

	///> 当两个人绑定的时候，需要让别人同时看到这两个人
	void UpdateBindRoleView(Role* self, Role* other);

private:
	RoleViewSet* GetViewSet(Role* role);
	void BindRoleView(Role* role, Role* bindrole);
	void PackAppear(KKSG::UnitAppearance *pAppearance, Role* other, Role* self, RoleSet& roleset);

	bool FillOne(Role* roleint, int scale[], Role* originrole, std::unordered_set<Role*>& newset, bool ismustsee = false);
	void FillNewSet(Role* role, int offset, int x, int z, const std::list<Role*>& lastset, std::unordered_set<Role*>& newset);

	inline bool IsValidOffset(int offset) { return offset >= 0 && offset < m_gridSize; }

public:
	Grid* m_grid;
	///> 视野格子对应玩家，玩家进入，离开，移动会改变
	RoleSet* m_grid2roles;
	int m_gridSize;
};

#endif
