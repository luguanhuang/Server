#include "pch.h"
#include "superriskconfig.h"
#include "pb/project.pb.h"
#include "foreach.h"
#include "util/XRandom.h"

INSTANCE_SINGLETON(SuperRiskConfig)

Coordinate* Coordinate::Invalid = new Coordinate(-100, -100);

SuperRiskConfig::SuperRiskConfig()
{
}

SuperRiskConfig::~SuperRiskConfig()
{
}

bool SuperRiskConfig::Init()
{
	if (!m_riskMapFileTable.LoadFile("table/RiskMapFile.txt"))
	{
		LogWarn("Load RiskMapFile.txt failed!");
		return false;
	}

	if (!m_riskMapBattleTable.LoadFile("table/RiskMapBattle.txt"))
	{
		LogWarn("Load RiskMapBattle.txt failed!");
		return false;
	}

	if (!m_riskDiceRandomTable.LoadFile("table/RiskDiceRandom.txt"))
	{
		LogWarn("Load RiskDiceRandom.txt failed!");
		return false;
	}

	if (!m_riskMapGenerateTable.LoadFile("table/RiskMapGenerate.txt"))
	{
		LogWarn("Load RiskMapGenerate.txt failed!");
		return false;
	}

	if (!m_riskTriggerBuyTable.LoadFile("table/RiskTriggerBuy.txt"))
	{
		LogWarn("Load RiskTriggerBuy.txt failed!");
		return false;
	}

	m_mapContent.InitMapContent(m_riskMapFileTable.Table);

	return true;
}

void SuperRiskConfig::Uninit()
{

}

void SuperRiskConfig::GetMapGenerateTemplateInfo(int mapID, std::list<RiskMapGenerateTable::RowData*>& vec)
{
	for (auto iter = m_riskMapGenerateTable.Table.begin(); iter != m_riskMapGenerateTable.Table.end(); ++iter)
	{
		if ((*iter)->MapID == mapID)
		{
			vec.push_back(*iter);
		}
	}
}

const XSuperRiskMapStaticInfo* SuperRiskConfig::GetRiskMapStaticInfo(int mapID)
{
	auto find = m_mapContent.m_maps.find(mapID);
	if (find == m_mapContent.m_maps.end())
	{
		return NULL;
	}
	return find->second;
}

bool SuperRiskConfig::GetNeedOpenMapID(int level, std::vector<int>& mapIDs)
{
	for (auto iter = m_riskMapFileTable.Table.begin(); iter != m_riskMapFileTable.Table.end(); ++iter)
	{
		if ((*iter)->NeedLevel <= level)
		{
			mapIDs.push_back((*iter)->MapID);
		}
	}

	return true;
}

const std::vector<RiskDiceRandomTable::RowData*>& SuperRiskConfig::GetDiceRandomTable()
{
	return m_riskDiceRandomTable.Table;
}

int SuperRiskConfig::GetRandomBattleSceneID(int mapID)
{
	int total = 0;
	for (auto iter = m_riskMapBattleTable.Table.begin(); iter != m_riskMapBattleTable.Table.end(); ++iter)
	{
		if ((*iter)->MapID == mapID)
		{
			total += (*iter)->Probability;
		}
	}

	int rand = XRandom::randInt(1, total + 1);
	int value = 0;
	for (auto iter = m_riskMapBattleTable.Table.begin(); iter != m_riskMapBattleTable.Table.end(); ++iter)
	{
		if ((*iter)->MapID != mapID)
			continue;
		value += (*iter)->Probability;
		if (rand <= value)
		{
			return (*iter)->SceneID;
		}
	}
	
	return 0;
}

const RiskMapBattle::RowData* SuperRiskConfig::GetRiskMapBattleData(int mapID, int sceneID)
{
	for (auto iter = m_riskMapBattleTable.Table.begin(); iter != m_riskMapBattleTable.Table.end(); ++iter)
	{
		if ((*iter)->MapID == mapID && (*iter)->SceneID == sceneID)
		{
			return *iter;
		}
	}

	return NULL;
}

bool SuperRiskConfig::IsTerminalPoint(int mapID, Coordinate& pos)
{
	auto find = m_mapContent.m_maps.find(mapID);
	if(find == m_mapContent.m_maps.end())
		return false;
	if (find->second->finalNode && find->second->finalNode->coord == pos)
	{
		return true;
	}

	return false;
}

bool SuperRiskConfig::GetStartNode(int mapID, Coordinate& start)
{
	auto find = m_mapContent.m_maps.find(mapID);
	if(find == m_mapContent.m_maps.end())
		return false;
	if (find->second->startNode)
	{
		start = find->second->startNode->coord;
		return true;
	}

	return false;
}

const RiskTriggerBuyTable::RowData* SuperRiskConfig::GetTriggerBuyData(int mapID, int gridType)
{
	for (auto iter = m_riskTriggerBuyTable.Table.begin(); iter != m_riskTriggerBuyTable.Table.end(); ++iter)
	{
		if ((*iter)->MapID == mapID && (*iter)->GridType == gridType)
		{
			return *iter;
		}
	}

	return NULL;
}


void XSuperRiskMapStaticInfo::ReadMapConfig(const std::string& fileName)
{
	std::string path = string("table/SuperRisk/" + fileName);
	std::string data = GSConfig::Instance()->ReadFileContent(path.c_str());
	std::stringstream ss(data);

	string line;
	int height = 0;
	int width = 0;

	if (data.empty())
	{
		LogWarn("map empty:%s", path.c_str());
	}

	while(true)
	{
		if (ss.eof())
		{
			break;
		}
		std::getline(ss, line);
		width = line.size();
		if (width == 0)
		{
			break;
		}

		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == '0' || line[i] == ' ' || line[i] == '\r') continue;

			XSuperRiskMapNode* node = new XSuperRiskMapNode(); 
			node->coord.x = i;
			node->coord.y = height;
			node->value = line[i];

			if (i > 0)
			{
				XSuperRiskMapNode* leftnode = this->FindMapNode(Coordinate(i - 1, height));
				if (leftnode != NULL)
				{
					leftnode->neighbour[(int)Right] = node;
					node->neighbour[(int)Left] = leftnode;
				}
			}

			if (height > 0)
			{
				XSuperRiskMapNode* upnode = this->FindMapNode(Coordinate(i, height - 1));
				if (upnode != NULL)
				{
					upnode->neighbour[(int)Down] = node;
					node->neighbour[(int)Up] = upnode;
				}
			}

			this->Nodes.push_back(node);
			if (node->value == 'T')
			{
				finalNode = node;
			}
			if (node->value == 'S')
			{
				startNode = node;
			}
		}

		++height;
	}

	this->width = width;
	this->height = height;
}

void XSuperRiskMapContentMgr::InitMapContent(const std::vector<RiskMapFile::RowData*>& mapTable)
{
	for (auto iter = mapTable.begin(); iter != mapTable.end(); ++iter)
	{
		XSuperRiskMapStaticInfo* info = new XSuperRiskMapStaticInfo((*iter)->MapID, (*iter)->NeedLevel);
		info->ReadMapConfig((*iter)->FileName);
		m_maps[info->mapID] = info;

		LogInfo("read map:%d", (*iter)->MapID);
	}
}


XSuperRiskMapStaticInfo::XSuperRiskMapStaticInfo(int id, int level)
{
	mapID = id;
	needLevel = level;
	height = 0;
	width = 0;
	finalNode = NULL;
	startNode = NULL;
}

XSuperRiskMapNode* XSuperRiskMapStaticInfo::FindMapNode(const Coordinate& coord) const
{
	for (int i = 0; i < Nodes.size(); ++i)
	{
		if ((Nodes[i]->coord.x == coord.x) && (Nodes[i]->coord.y == coord.y))
			return Nodes[i];
	}

	return NULL;
}
