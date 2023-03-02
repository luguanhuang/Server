#include "pch.h"
#include "pb/project.pb.h"
#include "superriskmap.h"
#include "unit/role.h"
#include "util/XRandom.h"
#include "gamelogic/itemconfig.h"
#include "config/itemdropconfig.h"

bool IsValidGridType(int gridType)
{
	if (gridType >= KKSG::RISK_GRID_MAX || gridType < KKSG::RISK_GRID_EMPTY)
	{
		return false;
	}

	return true;
}

XSuperRiskMap::XSuperRiskMap(int mapID, int curx, int cury, RiskDirection direct) : m_playerCoord(curx, cury)
{
	m_staticInfo = SuperRiskConfig::Instance()->GetRiskMapStaticInfo(mapID);

	m_playerMoveDirection = direct;
}

RiskDirection XSuperRiskMap::StartMoveNext(Coordinate& targetCoord)
{
	m_playerMoveDirection = GetNextTryDirection(m_playerMoveDirection, 3);
	// determine next step direction
	for (int i = 0; i < 4; i++)
	{
		Coordinate nextPos(-1, -1);
		GoWithDirection(m_playerCoord, m_playerMoveDirection, nextPos);
		if (m_staticInfo->FindMapNode(nextPos) != NULL)
		{
			targetCoord = nextPos;
			m_playerCoord = targetCoord;
			break;
		}

		m_playerMoveDirection = GetNextTryDirection(m_playerMoveDirection, 1);
	}

	return m_playerMoveDirection;
}

RiskDirection XSuperRiskMap::GetNextTryDirection(RiskDirection dir, int offset)
{
	return (RiskDirection)(((int)dir + offset) % 4);
}

void XSuperRiskMap::MoveNext()
{
	GoWithDirection(m_playerCoord, m_playerMoveDirection, m_playerCoord);

	//XDebug.singleton.AddGreenLog("SuperRisk: go to ", PlayerCoord.x.ToString(), ",",PlayerCoord.y.ToString());
	if (m_staticInfo->FindMapNode(m_playerCoord) == NULL)
	{
		//XDebug.singleton.AddErrorLog("SuperRisk: why do I go to a null place??");
	}

}
void XSuperRiskMap::GoWithDirection(Coordinate& c, RiskDirection dir, Coordinate& dest)
{
	int destX = -1;
	int destY = -1;
	switch (dir)
	{
	case Up:
		destX = c.x;
		destY = c.y - 1;
		break;
	case Down:
		destX = c.x;
		destY = c.y + 1;
		break;
	case Left:
		destX = c.x - 1;
		destY = c.y;
		break;
	case Right:
		destX = c.x + 1;
		destY = c.y;
		break;
	default:
		break;
	}

	dest.x = destX;
	dest.y = destY;
}

bool SuperRiskMapGenerator::GenerateOneRiskMapInfo(Role* role, int mapID, SuperRiskMapGeneratedInfo& mapInfo)
{
	if (!role)
	{
		return false;
	}
	auto staticInfo = SuperRiskConfig::Instance()->GetRiskMapStaticInfo(mapID);
	if (!staticInfo)
	{
		return false;
	}
	if (role->GetLevel() < staticInfo->needLevel)
	{
		return false;
	}

	std::list<RiskMapGenerateTable::RowData*> templateInfo;
	SuperRiskConfig::Instance()->GetMapGenerateTemplateInfo(mapID, templateInfo);

	mapInfo.mapID = mapID;
	std::map<char, std::list<XSuperRiskMapNode*>> staticNodesMapInfo;
	for (auto iter = staticInfo->Nodes.begin(); iter != staticInfo->Nodes.end(); ++iter)
	{
		staticNodesMapInfo[(*iter)->value].push_back(*iter);
	}

	for (auto iter = staticNodesMapInfo.begin(); iter != staticNodesMapInfo.end(); ++iter)
	{
		if (iter->second.empty())
		{
			continue;
		}
		int temp = 0;
		for (auto i = templateInfo.begin(); i != templateInfo.end(); ++i, ++temp)
		{
			if (!IsValidGridType((*i)->GridType))
			{
				continue;
			}
			if ((*i)->MapGridNum.find(iter->first) != std::string::npos && (*i)->Quantity)
			{
				for (int j = 0; j < ((*i)->Quantity); ++j)
				{
					int random = XRandom::randInt(1, 101);
					if ((*i)->Probability >= random)
					{
						if (iter->second.empty())
						{
							break;
						}
						int rand = XRandom::randInt(0, iter->second.size());
						int p = 0;
						for (auto k = iter->second.begin(); k != iter->second.end(); ++k, ++p)
						{
							if (p == rand)
							{
								GridInfo info;
								info.pos = (*k)->coord;
								info.gridType = (*i)->GridType;
								//info.dropID = (*i)->DropID;
								const ItemDesc* pConf = ItemDropConfig::Instance()->GetRandomDrop((*i)->DropID);
								if (pConf)
								{
									info.itemID = pConf->itemID;
									info.itemNum = pConf->itemCount;
								}
								mapInfo.m_grids[(*k)->coord] = info;
								LogDebug("superrisk,roleid:%llu, map:%u, char:%c, tableindex:%u, pos:%d,%d, grid type:%d, itemID:%u, itemNum:%u", role->GetID(), mapID, iter->first, temp, info.pos.x, info.pos.y, info.gridType, info.itemID, info.itemNum);
								iter->second.erase(k);
								break;
							}
						}
					}
				}
			}
		}
	}

	return true;
}

void GridInfo::Convert2KKSGRiskGridInfo(KKSG::RiskGridInfo& info)
{
	info.set_x(pos.x);
	info.set_y(pos.y);
	info.set_gridtype((KKSG::RiskGridType)gridType);
	info.set_boxstate(state);
	KKSG::ItemBrief* brief = info.mutable_rewarditem();
	brief->set_itemid(itemID);
	brief->set_itemcount(itemNum);
}

const GridInfo* SuperRiskMapGeneratedInfo::GetGridInfo(Coordinate& pos) const
{
	auto find = m_grids.find(pos);

	if (find == m_grids.end())
	{
		return NULL;
	}

	return &find->second;
}

void SuperRiskMapGeneratedInfo::UpdateGridBoxState(Coordinate& pos, KKSG::RiskBoxState state)
{
	auto find = m_grids.find(pos);

	if (find == m_grids.end())
	{
		return;
	}

	find->second.state = state;
}
