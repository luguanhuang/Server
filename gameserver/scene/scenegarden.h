#ifndef __SCENEGARDEN_H__
#define __SCENEGARDEN_H__

#include "scenecb.h"

namespace KKSG
{
	class SceneGardenInfo;
};

class SceneGarden
{
public:
	SceneGarden(Scene* scene);
	~SceneGarden();

	bool Init(const KKSG::SceneGardenInfo& info);

	void AddPlant(UINT32 slot, UINT64 plantid);
	const std::map<UINT32, UINT64>& GetPlants();

	// farm enemy
	void DeleteFarmlandEnemy(const UINT32 farmland_id);
	void AddFarmlandEnemy(const UINT32 farmland_id,const UINT64 enemy_id);
	UINT64 GetEnemyID(const UINT32 farmland_id);

private:
	Scene* m_scene;
	std::map<UINT32, UINT64> m_slot2plant;
	std::unordered_map<UINT32,UINT64> farmland_enemy_;
};

#endif