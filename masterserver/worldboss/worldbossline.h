#ifndef __WORLDBOSSLINE_H__
#define __WORLDBOSSLINE_H__

#include "commondef.h"
#include "scene/scenecreate.h"

struct WorldBossLine
{
	UINT32 sceneid;
	UINT32 gsline;
	std::vector<UINT64> m_roleids;

	WorldBossLine()
	{
		sceneid = 0;
		gsline = INVALID_LINE_ID;
	}
};

class CRole;
class WorldBossLineMgr : public ICreateBattleListener
{
public:
	WorldBossLineMgr();
	~WorldBossLineMgr();
	DECLARE_SINGLETON(WorldBossLineMgr);
public:
	bool Init();
	void Uninit();

	void Clear();
	
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

	const WorldBossLine* GetAvaliableLine(UINT64 roleid);
	void RoleEnterDelay(UINT64 roleid);

private:
	void OnLineReady(UINT32 sceneid, UINT32 gsline);

	void EnterScene(CRole* role, UINT32 sceneid, UINT32 gsline);

	WorldBossLine* CreateScene();

private:
	// lines
	std::map<UINT32, WorldBossLine*> m_sceneid2line;
	std::map<UINT64, WorldBossLine*> m_roleid2line;

	// pre line
	std::deque<WorldBossLine*> m_prelines;
	std::set<UINT64> m_waitlist;
};

#endif