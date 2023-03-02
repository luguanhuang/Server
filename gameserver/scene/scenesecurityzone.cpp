#include "pch.h"
#include "scenesecurityzone.h"
#include "math/XVector3.h"
#include "gamelogic/XCombat.h"
#include "table/RegionSkillList.h"
#include "table/RegionList.h"
#include "foreach.h"
#include "util/XCommon.h"


SceneSecurityZone::SceneSecurityZone()
{

}

SceneSecurityZone::~SceneSecurityZone()
{
	foreach(i in m_RegionDefines)
	{
		delete i->second;
	}

	m_SkillRegions.clear();
	m_RegionDefines.clear();
}

bool SceneSecurityZone::IsInZone(UINT32 sceneID, UINT32 m_skillId, const Vector3 & pos, ZoneEffect &eff)
{
	// 正确的场景
	auto it = m_SkillRegions.find(sceneID);
	if (it == m_SkillRegions.end())
	{
		return false;
	}

	foreach (j in it->second)
	{
		// 正确的技能
		if (j->SkillHash == m_skillId)
		{
			auto k = m_RegionDefines.find(j->RegionID);
			if (k != m_RegionDefines.end())
			{
				// 正确的范围
				if (k->second->InRegion(pos))
				{
					eff.ChangeCombatResult = j->RegionEffect;
					return true;
				}
			}
		}
	}
	
	return false;
}

static RegionShape *CreateShape(int	RegionType, std::vector<float> &RegionPos, std::vector<float> &RegionSize);

void SceneSecurityZone::Load(const char *skillFile, const char *regionFile)
{
	RegionSkillList SkillTable;
	if (!SkillTable.LoadFile(skillFile))
	{
		LogWarn("load %s failed!", skillFile);
		return ;
	}

	RegionList RegionTable;
	if (!RegionTable.LoadFile(regionFile))
	{
		LogWarn("load %s failed!", regionFile);
		return ;
	}

	size_t i = 0;
	for (i = 0; i < SkillTable.Table.size(); ++i)
	{
		SkillRegion region;
		auto pRow = SkillTable.Table[i];
		region.RegionID = pRow->RegionID;
		region.SkillHash = XCommon::XHash(pRow->SkillScript.c_str());
		region.RegionEffect = pRow->RegionEffect;
		m_SkillRegions[pRow->SceneID].push_back(region);

		LogInfo("Scene %u skill %u add region %d", pRow->SceneID, region.SkillHash, region.RegionID);
	}

	for (i = 0; i < RegionTable.Table.size(); ++i)
	{
		auto pRow = RegionTable.Table[i];
		m_RegionDefines[pRow->RegionID] = CreateShape(pRow->RegionType, pRow->RegionPos, pRow->RegionSize);
	}
}


class CircleRegion : public RegionShape
{
	float m_x;
	float m_z;
	float m_RR;

public:

	CircleRegion(std::vector<float> &RegionPos, std::vector<float> &RegionSize)
	{
		m_x = RegionPos[0];
		m_z = RegionPos[2];
		m_RR = RegionSize[0] * RegionSize[0];
	}

	virtual bool InRegion(const Vector3 &pos)
	{
		float dx = m_x - pos.x;
		float dz = m_z - pos.z;
		return (dx*dx+dz*dz) <= m_RR;
	}
};

class RectangleRegion : public RegionShape
{
	float m_x;
	float m_z;
	float m_xside;
	float m_zside;

public:

	RectangleRegion(std::vector<float> &RegionPos, std::vector<float> &RegionSize)
	{
		assert(RegionSize.size() == 2);

		m_x = RegionPos[0];
		m_z = RegionPos[2];
		m_xside = RegionSize[0]/2;
		m_zside = RegionSize[1]/2;
	}

	virtual bool InRegion(const Vector3 &pos)
	{
		return (pos.x >= m_x - m_xside && pos.x <= m_x + m_xside) &&
			   (pos.z >= m_z - m_zside && pos.z <= m_z + m_zside);
	}

};

RegionShape *CreateShape(int RegionType, std::vector<float> &RegionPos, std::vector<float> &RegionSize)
{
	if (RegionType == 1)
	{
		return new RectangleRegion(RegionPos, RegionSize);
	}
	else if (RegionType == 2)
	{
		return new CircleRegion(RegionPos, RegionSize);
	}
	else
	{
		LogWarn("Unknow region type %d in file table/RegionList.txt", RegionType);
		return NULL;
	}
}
