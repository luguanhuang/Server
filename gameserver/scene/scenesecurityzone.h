#ifndef __SCENESECURITYZONE_H__
#define __SCENESECURITYZONE_H__

class Vector3;

struct ZoneEffect
{
	int ChangeCombatResult;
};

class RegionShape
{
public:
	virtual ~RegionShape() {}
	virtual bool InRegion(const Vector3 &pos) = 0;
};

struct SkillRegion
{
	UINT32 SkillHash;
	int    RegionID;
	int    RegionEffect;
};

class SceneSecurityZone
{
public:
	SceneSecurityZone();
	~SceneSecurityZone();
	
	void Load(const char *skillFile, const char *regionFile);
	bool IsInZone(UINT32 sceneID, UINT32 m_skillId, const Vector3 & pos, ZoneEffect &eff);

private:

	std::unordered_map<UINT32, std::list<SkillRegion>> m_SkillRegions;
	std::unordered_map<int, RegionShape*>   m_RegionDefines;

};

#endif // __SCENESECURITYZONE_H__