#ifndef __ROLESUMMARY_H__
#define __ROLESUMMARY_H__

#include "bag.h"
#include "unit/pet.h"
#include "skilllevel.h"
#include "arenahistory.h"
#include "unit/combatattribute.h"
#include "rolesummary/rolesummarybase.h"


class RoleSummary : public RoleSummaryBase
{
	friend class RoleSummaryMgr;
public:
	RoleSummary();

	void Update(Role *pRole, int mask, void* arg);

	UINT64 GetRobotID() { return qwRobotID; }

	UINT32 GetLastUpdateTime() { return lastupdatetime; }

public:
	bool IsRobot;//是否是配置生成的
	UINT64 qwRobotID;//正在被使用作机器人时候的临时ID

	///> skill
	SkillSetSyncToOtherPlayer oSkills;
	///> attr
	CombatAttribute oAttribute;

private:
	UINT32 lastupdatetime;

	bool istodelete;
	///> concrete info
	FashionBrief oFashion;
	EquipBrief oEquip;
	EmblemBrief oEmblem;
	ArtifactBrief oArtifact;
	///> pet
	RolePetInfo oPetInfo;
	///> sprite
	std::vector<KKSG::SpriteInfo> oSprites;

	std::set<UINT32> display_fashion;
};

#endif
