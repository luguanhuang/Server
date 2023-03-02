#include "pch.h"
#include <math.h>
#include "attributepk.h"
#include "unit/unit.h"
#include "unit/attributewatcher.h"
#include "unit/role.h"
#include "unit/dummyrole.h"
#include "table/globalconfig.h"
#include "gamelogic/levelsealMgr.h"

INSTANCE_SINGLETON(AttributePkMgr)

AttributePkMgr::AttributePkMgr()
{

}

AttributePkMgr::~AttributePkMgr()
{

}

bool AttributePkMgr::Init()
{
	return LoadFile();
}

void AttributePkMgr::Uninit()
{
	ClearFile();
}

bool AttributePkMgr::CheckFile()
{
	PkProfessionTable tmpPkProfessionTable;
	if (!tmpPkProfessionTable.LoadFile("table/PkProfession.txt"))
	{
		SSWarn<<"load file table/PkProfession.txt failed"<<END;
		return false;
	}

	return true;
}

bool AttributePkMgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_PkProTable.LoadFile("table/PkProfession.txt");
	InitUseSceneType();

	return true;
}

void AttributePkMgr::ClearFile()
{
	m_PkProTable.Clear();
	m_usescenetype.clear();
}

void AttributePkMgr::SetPkAttribute( Unit* unit, int scenetype) 
{
	if(NULL == unit)
	{
		return ;
	}
	AttributeWatcher watcher(unit);
	int profession = 0;
	if(unit->IsRole())
	{
		profession = ((Role*)unit)->GetProfession();
	}
	else if(unit->IsDummyRole())
	{
		profession = ((DummyRole*)unit)->GetProfession();
	}
	PkProfessionTable::RowData* data = GetPkProConf(profession, scenetype);
	if(NULL == data)
	{
		SSError << " find pkprofession conf err profession = " << profession << " scenetype = " << scenetype << " level = " << unit->GetLevel() << END;
		return;
	}

	unit->SetCombatUseLevel(data->Level);		

	watcher.SetAttr(BASIC_PhysicalAtk, data->PhysicalAtk);
	watcher.SetAttr(PERCENT_PhysicalAtk, 0.0);
	watcher.SetAttr(BASIC_PhysicalAtkMod, data->PhysicalAtk);
	watcher.SetAttr(PERCENT_PhysicalAtkMod, 0.0);

	watcher.SetAttr(BASIC_PhysicalDef, data->PhysicalDef);
	watcher.SetAttr(PERCENT_PhysicalDef, 0.0);
	watcher.SetAttr(BASIC_PhysicalDefMod, data->PhysicalDef);
	watcher.SetAttr(PERCENT_PhysicalDefMod, 0.0);

	watcher.SetAttr(BASIC_MaxHP, data->MaxHP);
	watcher.SetAttr(PERCENT_MaxHP, 0.0);

	watcher.SetAttr(BASIC_HPRecovery, data->HPRecovery);
	watcher.SetAttr(PERCENT_HPRecovery, 0.0);

	watcher.SetAttr(BASIC_MagicAtk, data->MagicAtk);
	watcher.SetAttr(PERCENT_MagicAtk, 0.0);
	watcher.SetAttr(BASIC_MagicAtkMod, data->MagicAtk);
	watcher.SetAttr(PERCENT_MagicAtkMod, 0.0);

	watcher.SetAttr(BASIC_MagicDef, data->MagicDef);
	watcher.SetAttr(PERCENT_MagicDef, 0.0);
	watcher.SetAttr(BASIC_MagicDefMod, data->MagicDef);
	watcher.SetAttr(PERCENT_MagicDefMod, 0.0);

	watcher.SetAttr(BASIC_MaxMP, data->MaxMP);
	watcher.SetAttr(PERCENT_MaxMP, 0.0);

	watcher.SetAttr(BASIC_MPRecovery, data->MPRecovery);
	watcher.SetAttr(PERCENT_MPRecovery, 0.0);

	watcher.SetAttr(BASIC_Critical, data->Critical);
	watcher.SetAttr(PERCENT_Critical, 0.0);
	
	watcher.SetAttr(BASIC_CritResist, data->CritResist);
	watcher.SetAttr(PERCENT_CritResist, 0.0);

	const auto& AttrValue = data->AttrValue_s;
	for(size_t i = 0; i < AttrValue.size(); ++i)
	{
		int attr = (int)AttrValue[i][0];
		double value = AttrValue[i][1];
		watcher.SetAttr(CombatAttrDef(attr), value);
	}

	double hpMax = unit->GetAttr(TOTAL_MaxHP);
	watcher.SetAttr(BASIC_CurrentHP, hpMax);
	double mpMax = unit->GetAttr(TOTAL_MaxMP);
	watcher.SetAttr(BASIC_CurrentMP, mpMax);

	if(unit->IsRole())
	{
		watcher.CheckChangedAttributeAndNotify(false);
	}
	else if(unit->IsDummyRole())
	{
		//进入场景之前设置的 所以不通知
	}
}

PkProfessionTable::RowData* AttributePkMgr::GetPkProConf( int profession, int scenetype)
{
	UINT32 sealType = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	if(0 == sealType)
	{
		sealType = 1;
	}
	for(size_t i = 0; i < m_PkProTable.Table.size(); ++i)
	{
		PkProfessionTable::RowData* pConf = m_PkProTable.Table[i];
		if(pConf->Profession == profession && 
			(pConf->SealRange[0] <= sealType && sealType <= pConf->SealRange[1]) && 
			std::find(pConf->SceneType.begin(), pConf->SceneType.end(), scenetype) != pConf->SceneType.end())
		{
			return m_PkProTable.Table[i];
		}
	}
	return NULL;
}

bool AttributePkMgr::IsUseSceneType(int scenetype)
{
	return (m_usescenetype.find(scenetype) != m_usescenetype.end());
}

void AttributePkMgr::InitUseSceneType()
{
	for(size_t i = 0; i < m_PkProTable.Table.size(); ++i)
	{
		auto pConf = m_PkProTable.Table[i];
		for(size_t j = 0; j < pConf->SceneType.size(); ++j)
		{
			UINT32 scenetype = pConf->SceneType[j];
			m_usescenetype.insert(scenetype);

			SceneConfig::Instance()->MarkPvpAttr(scenetype);
		}
	}
}

void AttributePkMgr::AdjustAttrs(Role *pRole,KKSG::SceneType nSceneType,UINT32 nPPT,UINT32 nAvg)
{
	if(!pRole)
		return;
	int nJob =  pRole->GetProfession();
	PkProfessionTable::RowData* pRowData = AttributePkMgr::Instance()->GetPkProConf(nJob, nSceneType);
	if (!pRowData)
		return;
	float fFinalRate = 0.0;
	if (nPPT > nAvg)
	{
		float fTemp = pow((float)(nPPT)/nAvg,(float)GetGlobalConfig().PowPointBaseValue);
		fFinalRate  = fTemp>(float)GetGlobalConfig().PowPointMaxPoint?(float)GetGlobalConfig().PowPointMaxPoint:fTemp;
	}
	else
	{
		fFinalRate  = 1.0;  
	}
	AttributeWatcher watcher(pRole);
	for (auto iter = GetGlobalConfig().PowPointAdjust.begin(); iter != GetGlobalConfig().PowPointAdjust.end(); iter++)
	{
		CombatAttrDef key = (CombatAttrDef)(*iter);
		switch(key)
		{
		case BASIC_PhysicalAtk:
			{
				watcher.SetAttr(BASIC_PhysicalAtk, pRowData->PhysicalAtk*fFinalRate);
				watcher.SetAttr(BASIC_PhysicalAtkMod, pRowData->PhysicalAtk*fFinalRate);
				break;
			}
		case BASIC_MagicAtk:
			{
				watcher.SetAttr(BASIC_MagicAtk, pRowData->MagicAtk*fFinalRate);
				watcher.SetAttr(BASIC_MagicAtkMod, pRowData->MagicAtk*fFinalRate);
				break;
			}
		case TOTAL_MaxHP:
			{
				watcher.SetAttr(BASIC_MaxHP, pRowData->MaxHP*fFinalRate);
				watcher.SetAttr(BASIC_CurrentHP, pRole->GetAttr(BASIC_MaxHP));
				break;
			}
		default:
			break;
		}
	}
	watcher.CheckChangedAttributeAndNotify(false);
}