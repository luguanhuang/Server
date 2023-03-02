#ifndef __SUPERRISKMAP_H__
#define __SUPERRISKMAP_H__

#include "unit/roleserilizeinterface.h"
#include "superriskconfig.h"

class Role;
namespace KKSG
{
	class RiskGridInfo;
}

struct GridInfo
{
	GridInfo():pos(-1,-1)
	{
		//dropID = 0;
		itemID = 0;
		itemNum = 0;
		gridType = 0;
		state = KKSG::RISK_BOX_LOCKED;
	}
	void Convert2KKSGRiskGridInfo(KKSG::RiskGridInfo& info);

	Coordinate pos;
	//int dropID;
	int itemID;
	int itemNum;
	int gridType;
	KKSG::RiskBoxState state;
};
struct SuperRiskMapGeneratedInfo
{
	SuperRiskMapGeneratedInfo()//:m_curPos(0,0)
	{
		mapID = 0;
	}
	void Clear()
	{
		mapID = 0;
		m_grids.clear();
		//m_curPos.Clear();
	}

	const GridInfo* GetGridInfo(Coordinate& pos) const;

	void UpdateGridBoxState(Coordinate& pos, KKSG::RiskBoxState state);

	int mapID;
	//Coordinate m_curPos;
	std::map<Coordinate, GridInfo> m_grids;
};
//class Role;
class SuperRiskMapGenerator
{
public:
	static bool GenerateOneRiskMapInfo(Role* role, int mapID, SuperRiskMapGeneratedInfo& mapInfo);
};

//Íæ¼Ò²Ù×÷
class XSuperRiskMap
{
public:
	XSuperRiskMap(int mapID, int curx, int cury, RiskDirection direct);
	const XSuperRiskMapStaticInfo* m_staticInfo;

	const XSuperRiskMapDynamicInfo* m_dynamicInfo;

	Coordinate m_playerCoord;
	RiskDirection m_playerMoveDirection;

	//XSuperRiskMapRenderer renderer = new XSuperRiskMapRenderer();

	void Clear()
	{
		m_staticInfo = NULL;
		m_dynamicInfo = NULL;

		m_playerCoord = *Coordinate::Invalid;
		m_playerMoveDirection = Up;
	}

	RiskDirection StartMoveNext(Coordinate& targetCoord);

protected:
	//auto search order: right -> down -> left -> up
	RiskDirection GetNextTryDirection(RiskDirection dir, int offset);

	void MoveNext();

	//Coordinate GoWithDirection(Coordinate c, RiskDirection dir);
	void GoWithDirection(Coordinate& c, RiskDirection dir, Coordinate& dest);
};

#endif