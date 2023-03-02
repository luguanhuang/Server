#include "pch.h"
#include "unit.h"
#include "timeutil.h"
#include "attributewatcher.h"
#include "scene/ptcg2c_attributechangenotify.h"
#include "scene/scene.h"
#include "foreach.h"
#include "combatattribute.h"

void AttributeWatcher::CheckChangedAttributeAndNotify(bool broadcast, bool showHUD, UINT64 casterID)
{
	///> 如果是定时同步
	if (m_pUnit->IsCycleSync())
	{
		UINT64 milliontime = TimeUtil::GetMilliSecond();
		if (!m_pUnit->IsSyncTimeOut(milliontime))
		{
			return;
		}
		///> 定时同步时间到了
		m_pUnit->SetLastSync(milliontime);
		m_AttrWatch = m_pUnit->GetSyncAttrs();
		m_pUnit->ClearSyncAttr();
		///> 定时同步避免集中飘伤害
		showHUD = false;
	}

	if(m_pUnit->IsRole() && m_pUnit->GetCurrScene() != NULL && m_pUnit->GetCurrScene()->GetSceneType() == KKSG::SCENE_WORLDBOSS)
	{
		broadcast = false;
	}

	int Count = 0;
	double newValue;
	PtcG2C_AttributeChangeNotify oAttrNtfPtc;

	bool isWorlBoss = (m_pUnit->GetCurrScene() != NULL && m_pUnit->GetCurrScene()->GetSceneType() == KKSG::SCENE_WORLDBOSS);
	foreach (i in m_AttrWatch)
	{
		newValue = m_pUnit->GetAttr((CombatAttrDef)i->first);
		if (newValue != i->second)
		{
			if(isWorlBoss && (i->first == 19 || i->first == 24)) continue;

			oAttrNtfPtc.m_Data.add_attrid(i->first);
			oAttrNtfPtc.m_Data.add_attrvalue(newValue);
			
			++Count;
		}
	}

	if (Count > 0)
	{
		if (showHUD)
		{
			oAttrNtfPtc.m_Data.set_showhud(true);
		}
		if(casterID != 0)
		{
			oAttrNtfPtc.m_Data.set_casterid(casterID);
		}

		oAttrNtfPtc.m_Data.set_uid(m_pUnit->GetID());
		oAttrNtfPtc.m_Data.set_time(TimeUtil::GetFrameTime());

		if (broadcast)
		{
			if (m_pUnit->GetCurrScene())
			{
				m_pUnit->GetCurrScene()->Broadcast(m_pUnit, oAttrNtfPtc);
			}
		}
		else
		{
			m_pUnit->Send(oAttrNtfPtc);
		}
	}
}

void AttributeWatcher::AddWatchedAttribute(CombatAttrDef attrID)
{
	//AttrPair pair;
	//pair.attrID = attrID;
	//pair.attrValue = m_pUnit->GetAttr(attrID);
	//m_AttrWatch.insert(pair);

	if (m_pUnit->IsCycleSync())
	{
		m_pUnit->AddSyncAttr((int)attrID, m_pUnit->GetAttr(attrID));
	}
	else
	{
		m_AttrWatch.insert(std::unordered_map<int, double>::value_type((int)attrID, m_pUnit->GetAttr(attrID)));
	}
}

AttributeWatcher::AttributeWatcher(Unit *pUnit) : m_pUnit(pUnit)
{

}

void AttributeWatcher::SetAttr(CombatAttrDef attrDef, double value)
{
	AddWatchedAttribute(attrDef);
	m_pUnit->SetAttr(attrDef, value);
}

void AttributeWatcher::AddAttr(CombatAttrDef attrDef, double value)
{
	AddWatchedAttribute(attrDef);
	m_pUnit->AddAttr(attrDef, value);
}

