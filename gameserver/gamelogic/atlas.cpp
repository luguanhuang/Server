#include "pch.h"
#include "atlas.h"
#include "atlasconfigmgr.h"
#include "unit/role.h"
#include "unit/calcattribute.h"
#include "gamelogic/itemsuffix.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/itemconfig.h"
#include "atlas/ptcg2c_synatlasattr.h"
#include "foreach.h"
#include "table/PowerPointMgr.h"
#include "reportdatamgr.h"
#include "popwindows.h"
#include "utility/tlogger.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"

CAtlasSys::CAtlasSys()
{

}
CAtlasSys::CAtlasSys(Role* pRole)
{
	m_pRole = pRole;
}
CAtlasSys::~CAtlasSys()
{

}

void CAtlasSys::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(m_pRole->GetProfession(), GSConfig::Instance()->GetBackFlowLevel());
		if (pData)
		{
			for (auto it = pData->Atlas.begin(); it != pData->Atlas.end(); ++it)
			{
				m_groupMap[CAtlasConfigMagr::Instance()->GetGroupId(*it)].insert(*it);
			}
			m_switch.Set();
		}
	}
}


bool CAtlasSys::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	m_groupMap.clear();
	if (poRoleAllInfo->has_atlas())
	{
		const KKSG::SAtlasRecord& record = poRoleAllInfo->atlas();
		for (UINT32 cnt = 0; cnt < record.atlas_size(); ++cnt)
		{
			m_groupMap[CAtlasConfigMagr::Instance()->GetGroupId(record.atlas(cnt))].insert(record.atlas(cnt));
		}

		for (UINT32 cnt = 0; cnt < record.finishdata_size(); ++cnt)
		{
			m_finishRcd[record.finishdata(cnt).groupid()] = record.finishdata(cnt).finishid(); 
		}
	}

	return true;
}

void CAtlasSys::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!_IsChange())
	{
		return ;
	}

	KKSG::SAtlasRecord* record = poRoleAllInfo->mutable_atlas();
	record->clear_atlas();
	foreach (i in m_groupMap)
	{
		foreach (j in i->second)
		{
			record->add_atlas(*j);
		}
	}

	record->clear_finishdata();
	foreach (j in m_finishRcd)
	{
		KKSG::atlasdata* pdata = record->add_finishdata();
		pdata->set_groupid(j->first);
		pdata->set_finishid(j->second);
	}

	roChanged.insert(record);
}

bool CAtlasSys::_IsGroupOpen(UINT32 groupId)
{
	return CAtlasConfigMagr::Instance()->IsGroupOpen(groupId, m_pRole->GetLevel());
}

void CAtlasSys::GetAllAttr(std::vector<ItemChangeAttr>& attrsVec)
{
	std::map<UINT32, UINT32> allAttrMap;
	CAtlasConfigMagr::Instance()->GetAllAttr(m_pRole->GetProfession(), allAttrMap, m_groupMap, m_finishRcd);

	foreach (iter in allAttrMap)
	{
		attrsVec.push_back(ItemChangeAttr(iter->first, iter->second));
	}
}

UINT32 CAtlasSys::ActivatAtlas(UINT32 teamId)
{
	UINT32 groupId = CAtlasConfigMagr::Instance()->GetGroupId(teamId);

	if (!_IsGroupOpen(groupId))
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (_HasALreadyTeam(teamId))
	{
		return KKSG::ERR_COMMENDWATCH_COUNTLIMIT;
	}

	UINT32 errorCode = CAtlasConfigMagr::Instance()->TryCreateTeam(m_pRole, teamId, GetFinishId(groupId));
	if (errorCode != KKSG::ERR_SUCCESS)
	{
		return errorCode;
	}

	m_groupMap[groupId].insert(teamId);
	SSInfo <<"CAtlasSys::ActivatAtlas roleid : " << m_pRole->GetID() << " groupid : " << groupId << "teamid : " << teamId << END; 

	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Atlas);
	SynAtlasAttr();

	DoTxLog(groupId, teamId);
	ReportDataMgr::Instance()->AtlasActive(m_pRole, groupId, m_groupMap[groupId].size());
	PopWindows::Instance()->CheckComment(m_pRole,KKSG::COMMENT_ATLAS,groupId,m_groupMap[groupId].size());
	_SetChange();
	return KKSG::ERR_SUCCESS;
}

bool CAtlasSys::_IsGroupFinish(UINT32 groupId)
{
	std::set<UINT32> teamSet;
	CAtlasConfigMagr::Instance()->GetGroupItem(groupId, teamSet);

	if (teamSet.empty())
	{
		return false;
	}

	auto iter = m_groupMap.find(groupId);
	if (iter == m_groupMap.end())
	{
		return false;
	}

	foreach (i in teamSet)
	{
		if (iter->second.find(*i) == iter->second.end())
		{
			return false;
		}
	}

	return true;
}

bool CAtlasSys::_HasALreadyTeam(UINT32 teamId)
{
	foreach(i in m_groupMap)
	{
		if (i->second.find(teamId) != i->second.end())
		{
			return true;
		}
	}
	return false;
}

void CAtlasSys::ApplyEffect(CombatAttribute *roleattrs)
{
	std::vector<ItemChangeAttr> attrsVec;
	GetAllAttr(attrsVec);
	ApplyEffectAux(attrsVec, roleattrs);
}

UINT32 CAtlasSys::GetAtlasPowerPoint()
{
	CombatAttribute atlasAttr;
	ApplyEffect(&atlasAttr);
	CalcRoleAttribute InitRoleAttribute;
	InitRoleAttribute.CalcAttributeConvert(&atlasAttr, m_pRole->GetProfession());
	return (UINT32)PowerPointMgr::Instance()->CalcPowerPoint(&atlasAttr, m_pRole->GetProfession());
}
void CAtlasSys::BreakCard(UINT32 itemId, UINT32 itemNum)
{	
	std::map<UINT32, UINT32> itemMap;
	ItemConfig::Instance()->GetDecomposedItem(itemId, itemNum, itemMap);

	BagTakeItemTransition take(m_pRole);
	take.SetReason(ItemFlow_Atlas, ItemFlow_BreakAtlas);
	if (!take.TakeItem(itemId, itemNum))
	{
		take.RollBack();
		return ;
	}
	take.NotifyClient();

	BagGiveItemTransition give(m_pRole);
	give.SetReason(ItemFlow_Atlas, ItemFlow_BreakAtlas);
	foreach (i in itemMap)
	{
		give.GiveItem(i->first, i->second);
	}
	give.NotifyClient();
	_SetChange();
}

void CAtlasSys::SynAtlasAttr()
{
	PtcG2C_SynAtlasAttr msg;
	std::vector<ItemChangeAttr> attrsVec; 
	GetAllAttr(attrsVec);
	KKSG::SynCardAttr* pCardAttr = msg.m_Data.add_allattrs();
	foreach (i in attrsVec)
	{
		KKSG::CardAttr* pData = pCardAttr->add_allattr();
		pData->set_id(i->AttrID);
		pData->set_num(i->AttrValue);
	}

	m_pRole->Send(msg);

	/*
	foreach (i in m_groupMap)
	{
		UINT32 finishId = _GetFinishId(i->first); 
		vector<Sequence<uint, 2>> allAttrVec;
		std::map<UINT32, UINT32> allPerMap;
		KKSG::SynCardAttr* pCardAttr = msg.m_Data.add_allattrs();

		pCardAttr->set_groupid(i->first);

		CAtlasConfigMagr::Instance()->GetAddPer(allPerMap, i->first, i->second.size(), finishId);
		foreach(j in allPerMap)
		{
			KKSG::CardAttr* pAtt = pCardAttr->add_addper();
			pAtt->set_id(j->first);
			pAtt->set_num(j->second);
		}

		CAtlasConfigMagr::Instance()->GetAddAttr(m_pRole->GetProfession(), allAttrVec, i->second);
		if (allAttrVec.size() > finishId)
		{
			KKSG::CardAttr* pAttr = pCardAttr->add_addattr();
			pAttr->set_id(allAttrVec[finishId].seq[0]);
			pAttr->set_num(allAttrVec[finishId].seq[1]);

			KKSG::CardAttr* p = pCardAttr->add_allattr();
			p->set_id(allAttrVec[finishId].seq[0]);
			auto iter = allPerMap.find(allAttrVec[finishId].seq[0]);
			if (iter != allPerMap.end())
			{
				p->set_num(allAttrVec[finishId].seq[1] * (100 + iter->second) / 100);
			}
			else
			{
				p->set_num(allAttrVec[finishId].seq[1]);
			}
		}
		*/
		/*
		foreach(k in allAttrMap)
		{
			KKSG::CardAttr* pAttr = pCardAttr->add_addattr();
			pAttr->set_id(k->first);
			pAttr->set_num(k->second);
			
			KKSG::CardAttr* p = pCardAttr->add_allattr();
			p->set_id(k->first);
			auto iter = allPerMap.find(k->first);
			if (iter != allPerMap.end())
			{
				p->set_num(k->second * (100 + iter->second) / 100);
			}
			else
			{
				p->set_num(k->second);
			}
		}
		*/
	//}
}

UINT32 CAtlasSys::AutoBreakCard(const std::set<UINT32>& qualitSet, UINT32 groupId)
{
	std::set<UINT32> hasTeamid;
	foreach (i in m_groupMap)
	{
		foreach (j in i->second)
		{
			hasTeamid.insert(*j);
		}
	}

	BagTakeItemTransition take(m_pRole);
	take.SetReason(ItemFlow_Atlas, ItemFlow_CreateAtlasTeam);

	std::vector<ItemDesc> giveItems;
	bool isbreaked = false;
	foreach (i in qualitSet)
	{
		std::map<UINT32, UINT32> cardMap;
		CAtlasConfigMagr::Instance()->GetQualitCards(*i, cardMap, groupId);
		foreach(j in cardMap)
		{
			Bag* pBag = m_pRole->Get<Bag>();
			UINT32 hasNum = pBag->CountItemInBag(j->first); 
			if (hasNum == 0)
			{
				continue;
			}

			UINT32 needNum = CAtlasConfigMagr::Instance()->NeedCardTimes(m_pRole, j->first, hasTeamid);
			UINT32 delNum = hasNum > needNum ? hasNum - needNum : 0; 
			if (delNum > 0)
			{
				if (!take.TakeItem(j->first, delNum))
				{
					take.RollBack();
					continue;
				}

				std::map<UINT32, UINT32> itemMap;
				ItemConfig::Instance()->GetDecomposedItem(j->first, j->second, itemMap);
				foreach (k in itemMap)
				{
					giveItems.push_back(ItemDesc(k->first, k->second * delNum));
				}
				
				isbreaked = true;
			}
		}
	}
	take.NotifyClient();

	BagGiveItemTransition give(m_pRole);
	give.SetReason(ItemFlow_Atlas, ItemFlow_CreateAtlasTeam);
	for (auto i = giveItems.begin(); i != giveItems.end(); i++)
	{
		give.GiveItem(*i);
	}
	give.NotifyClient();

	if (isbreaked)
	{
		_SetChange();
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		return KKSG::ERR_ATLAS_NOT_BREAK;
	}
}

UINT32 CAtlasSys::_GetFinishId(UINT32 groupId)
{
	auto iter = m_finishRcd.find(groupId);
	return iter == m_finishRcd.end() ? 0 : iter->second;
}

bool CAtlasSys::UpStar(UINT32 groupId)
{
	if (CAtlasConfigMagr::Instance()->GetUpStarTimes(groupId) <= _GetFinishId(groupId))
	{
		LogWarn("role [%llu], groupId [%u], UpStarTimes <= FinishId", m_pRole->GetID(), groupId);
		return false;
	}

	if (!CAtlasConfigMagr::Instance()->CanUpStar(groupId, m_pRole->GetLevel(), _GetFinishId(groupId)))
	{
		LogWarn("role [%llu], groupId [%u], level [%d] low", m_pRole->GetID(), groupId, m_pRole->GetLevel());
		return false;
	}

	if (_IsGroupFinish(groupId))
	{
		++m_finishRcd[groupId];
		m_groupMap[groupId].clear();
		_SetChange();
		return true;
	}
	LogWarn("role [%llu], groupId [%u], card not enough", m_pRole->GetID(), groupId);
	return false;
}

UINT32 CAtlasSys::GetFinishId(UINT32 groupId)
{
	auto iter = m_finishRcd.find(groupId);
	return iter == m_finishRcd.end() ? 0 : iter->second;
}

void CAtlasSys::openGroup(UINT32 groupId)
{
	if (groupId == 0)
	{
		m_groupMap.clear();
		_SetChange();
		return ;
	}

	std::set<UINT32> itemSet;
	CAtlasConfigMagr::Instance()->GetGroupItem(groupId, itemSet);
	std::set<UINT32>& teamSet = m_groupMap[groupId];
	foreach (i in itemSet)
	{
		teamSet.insert(*i);
	}
	SynAtlasAttr();
	_SetChange();
}

void CAtlasSys::DoTxLog(UINT32 groupId, UINT32 teamId, UINT32 op, const std::string& trans)
{
	TAtlasFlow oLog(m_pRole);
	oLog.m_GroupId = groupId;
	oLog.m_TeamId = teamId;
	oLog.m_Num = m_groupMap[groupId].size();
	oLog.m_AtlasPowerPoint = GetAtlasPowerPoint();
	oLog.m_Round = GetFinishId(groupId);
	oLog.m_op = op;
	if (!trans.empty())
	{
		oLog.SetTransTag(trans);
	}
	oLog.Do();
}

void CAtlasSys::Logout(const std::string& trans)
{
	for (auto it = m_groupMap.begin(); it != m_groupMap.end(); it ++)
	{
		DoTxLog(it->first, 0, 1, trans);
	}
}
