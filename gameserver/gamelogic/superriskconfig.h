#ifndef __SUPERRISKCONFIG_H__
#define __SUPERRISKCONFIG_H__
#include "table/RiskDiceRandomTable.h"
#include "table/RiskMapBattle.h"
#include "table/RiskMapFile.h"
#include "table/RiskMapGenerateTable.h"
#include "table/RiskTriggerBuyTable.h"

enum RiskDirection
{
	Right = 0,
	Down,
	Left,
	Up,
};

struct Coordinate
{
	Coordinate(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	void Clear()
	{
		x = 0;
		y = 0;
	}
	static Coordinate* Invalid;

	void operator = (Coordinate& b)
	{
		this->x = b.x;
		this->y = b.y;
	}
	
	const bool operator == (const Coordinate& b)
	{
		if (this->x == b.x && this->y == b.y)
		{
			return true;
		}

		return false;
	}

	const bool operator < (const Coordinate& b) const
	{
		if ((100 * y + x) < (100 * b.y + b.x))
		{
			return true;
		}

		return false;
	}

	int x;
	int y;
};

class XSuperRiskMapNode
{
public:
	XSuperRiskMapNode():coord(-1,-1){}
	char value;
	Coordinate coord;
	XSuperRiskMapNode* neighbour[4];
};

class XSuperRiskMapStaticInfo
{
public:
	XSuperRiskMapStaticInfo(int id, int level);

	void ReadMapConfig(const std::string& fileName);

	XSuperRiskMapNode* FindMapNode(const Coordinate& coord) const;

public:
	std::vector<XSuperRiskMapNode*> Nodes;
	int width;
	int height;

	int needLevel;
	int mapID;

	XSuperRiskMapNode* finalNode;
	XSuperRiskMapNode* startNode;
};

class XSuperRiskMapDynamicInfo
{

};


class XSuperRiskMapContentMgr
{
public:

	void Clear()
	{
	}

	void InitMapContent(const std::vector<RiskMapFile::RowData*>& mapTable);
public:
	std::map<int, XSuperRiskMapStaticInfo*> m_maps;

};

class SuperRiskConfig
{
	SuperRiskConfig();
	~SuperRiskConfig();
	DECLARE_SINGLETON(SuperRiskConfig)

	bool Init();
	void Uninit();


	int GetRandomBattleSceneID(int mapID);

	bool GetNeedOpenMapID(int level, std::vector<int>& mapIDs);

	bool IsTerminalPoint(int mapID, Coordinate& pos);

	bool GetStartNode(int mapID, Coordinate&start);

	void GetMapGenerateTemplateInfo(int mapID, std::list<RiskMapGenerateTable::RowData*>& vec);

	const XSuperRiskMapStaticInfo* GetRiskMapStaticInfo(int mapID);
	const RiskMapBattle::RowData* GetRiskMapBattleData(int mapID, int sceneID);
	const RiskTriggerBuyTable::RowData* GetTriggerBuyData(int mapID, int gridType);
	const std::vector<RiskDiceRandomTable::RowData*>& GetDiceRandomTable();

protected:
	RiskDiceRandomTable m_riskDiceRandomTable;
	RiskMapBattle m_riskMapBattleTable;
	RiskMapFile m_riskMapFileTable;
	RiskMapGenerateTable m_riskMapGenerateTable;
	RiskTriggerBuyTable m_riskTriggerBuyTable;

	XSuperRiskMapContentMgr m_mapContent;
};
class SuperRiskMapContent
{

};

#endif