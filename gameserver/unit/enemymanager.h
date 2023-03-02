#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "cmdline.h"
#include "enemy.h"

#define EnemyMask ((UINT64)KKSG::Category_Enemy << 60)

class Role;
class Enemy;
class Vector3;
class CombatAttribute;
namespace KKSG
{
	class UnitAppearance;
};

class EnemyManager
{
	EnemyManager();
	~EnemyManager();
	DECLARE_SINGLETON(EnemyManager)

public:

	bool Init();
	void Uninit();

	void Update(float delta);

	void AddEnemy(Enemy *enemy);
	void RemoveEnemy(UINT64 uID);
	Enemy *FindEnemy(UINT64 uID);

	Enemy *CreateEnemy(UINT32 uTemplateID, const Vector3& pos, const float face, float scale = 1.0f);
	//fight group is same with caller
	Enemy *CreateEnemyByCaller(Unit* caller, UINT32 uTemplateID, const Vector3& pos, const float face, bool shieldable, float scale = 1.0f);

	UINT64 NewEnemyID();
	UINT32 GetCount() { return m_Enemys.size(); }

	static bool LoadEnemyAttribute(UINT32 uTemplateID, CombatAttribute *pCombatAttr, float scale = 1.0f);
	static bool LoadEnemyAttribute(const EnemyTemplate *template_, CombatAttribute *pCombatAttr, float scale = 1.0f);
	static bool LoadRoleAttribute(Role* role, CombatAttribute *pCombatAttr, UINT32 profession, float scale = 1.0f);

	static bool LoadEnemyAppearance(UINT32 uTemplateID, KKSG::UnitAppearance *pData, bool loadAttr = true, float scale = 1.0f);
	static bool CmdLineEnemyManager(const CmdLineArgs &args, std::string &outputMessage);
	
private:
	UINT32 m_CreateCount;
	UINT32 m_DestoryCount;

	UINT64 m_EnemyIDCount;

	std::vector<UINT64> m_todelete;
	std::unordered_map<UINT64, Enemy*> m_Enemys;
};


#endif 