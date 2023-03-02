#include "pch.h"
#include "PowerPointMgr.h"
#include "unit/combatattribute.h"
#include "unit/role.h"
#include "unit/attributewatcher.h"
#include "ProfessionMgr.h"


INSTANCE_SINGLETON(PowerPointMgr)

PowerPointMgr::PowerPointMgr()
{

}

PowerPointMgr::~PowerPointMgr()
{

}

bool PowerPointMgr::Init()
{
	return LoadFile();
}

void PowerPointMgr::Uninit()
{
	ClearFile();
}

void PowerPointMgr::ClearFile()
{
	m_deftable.Clear();
	m_oTable.Clear();
	m_oCoeffTable.Clear();
	m_oCoeff.clear();
}

bool PowerPointMgr::CheckFile()
{
	DefModTable tempdeftable;
	if (!tempdeftable.LoadFile("table/DefMod.txt"))
	{
		LogWarn("Load file %s failed!", "def");
		return false;
	}
	PowerPointAdjustTable tempTable;
	if (!tempTable.LoadFile("table/AtkDefMod.txt"))
	{
		LogWarn("Load file %s failed!", "ppt");
		return false;
	}
	PowerPointCoeffTable  tempCoeffTable;
	if (!tempCoeffTable.LoadFile("table/AttributeList.txt"))
	{
		LogWarn("Load file %s failed!", "table/AttributeList.txt");
		return false;
	}
	return true;
}

bool PowerPointMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	if (!m_deftable.LoadFile("table/DefMod.txt"))
	{
		LogWarn("Load file %s failed!", "def");
		return false;
	}
	if (!m_oTable.LoadFile("table/AtkDefMod.txt"))
	{
		LogWarn("Load file %s failed!", "ppt");
		return false;
	}
	if (!m_oCoeffTable.LoadFile("table/AttributeList.txt"))
	{
		LogWarn("Load file %s failed!", "table/AttributeList.txt");
		return false;
	}

	for(auto i = m_oCoeffTable.Table.begin(); i != m_oCoeffTable.Table.end(); i++)
	{
		m_oCoeff[(*i)->AttributeID].push_back(PowerPointCoeffConf((*i)->AttributeID, (*i)->Profession, (*i)->Weight));
	}

	return true;
}

float PowerPointMgr::GetAtkCoeffcient(int Level, double CurrentPPT, double NeedPPT)
{
	if (NeedPPT == 0 || CurrentPPT == 0)
	{
		return 0;
	}
	if (Level-1 >= (int)m_oTable.Table.size())
	{
		return 0;
	}

	PowerPointAdjustTable::RowData *pConf = m_oTable.Table[Level-1];

	int percent = (int)((CurrentPPT - NeedPPT) * 100 / CurrentPPT + 0.5);
	switch (percent)
	{
	case -19: return pConf->P_19;
	case -18: return pConf->P_18;
	case -17: return pConf->P_17;
	case -16: return pConf->P_16;
	case -15: return pConf->P_15;
	case -14: return pConf->P_14;
	case -13: return pConf->P_13;
	case -12: return pConf->P_12;
	case -11: return pConf->P_11;
	case -10: return pConf->P_10;
	case -9: return pConf->P_9;
	case -8: return pConf->P_8;
	case -7: return pConf->P_7;
	case -6: return pConf->P_6;
	case -5: return pConf->P_5;
	case -4: return pConf->P_4;
	case -3: return pConf->P_3;
	case -2: return pConf->P_2;
	case -1: return pConf->P_1;
	case 0: return pConf->P_0;
	case 1: return pConf->P1;
	case 2: return pConf->P2;
	case 3: return pConf->P3;
	case 4: return pConf->P4;
	case 5: return pConf->P5;
	case 6: return pConf->P6;
	case 7: return pConf->P7;
	case 8: return pConf->P8;
	case 9: return pConf->P9;
	default: 
		if (percent > 9) return pConf->P10;
		if (percent < -19) return pConf->P_20;
	}

	return 0;
}

float PowerPointMgr::GetDefCoeffcient(int Level, double CurrentPPT, double NeedPPT)
{
	if (NeedPPT == 0 || CurrentPPT == 0)
	{
		return 0;
	}
	if (Level-1 >= (int)m_deftable.Table.size())
	{
		return 0;
	}

	DefModTable::RowData *pConf = m_deftable.Table[Level-1];

	int percent = (int)((CurrentPPT - NeedPPT) * 100 / CurrentPPT + 0.5);
	switch (percent)
	{
	case -19: return pConf->P_19;
	case -18: return pConf->P_18;
	case -17: return pConf->P_17;
	case -16: return pConf->P_16;
	case -15: return pConf->P_15;
	case -14: return pConf->P_14;
	case -13: return pConf->P_13;
	case -12: return pConf->P_12;
	case -11: return pConf->P_11;
	case -10: return pConf->P_10;
	case -9: return pConf->P_9;
	case -8: return pConf->P_8;
	case -7: return pConf->P_7;
	case -6: return pConf->P_6;
	case -5: return pConf->P_5;
	case -4: return pConf->P_4;
	case -3: return pConf->P_3;
	case -2: return pConf->P_2;
	case -1: return pConf->P_1;
	case 0: return pConf->P_0;
	case 1: return pConf->P1;
	case 2: return pConf->P2;
	case 3: return pConf->P3;
	case 4: return pConf->P4;
	case 5: return pConf->P5;
	case 6: return pConf->P6;
	case 7: return pConf->P7;
	case 8: return pConf->P8;
	case 9: return pConf->P9;
	default: 
		if (percent > 9) return pConf->P10;
		if (percent < -19) return pConf->P_20;
	}

	return 0;
}

void PowerPointMgr::AdjustAttributeByCoeffcient(Role *pRole, float atkcoeff, float defcoeff)
{
	CombatAttribute *combatAttr = pRole->GetAttrPtr();
	AttributeWatcher watch(pRole);

	watch.SetAttr(BASIC_PhysicalAtkMod, combatAttr->GetAttr(BASIC_PhysicalAtk) * (1 + atkcoeff));
	watch.SetAttr(BASIC_MagicAtkMod, combatAttr->GetAttr(BASIC_MagicAtk) * (1 + atkcoeff));
	watch.SetAttr(BASIC_PhysicalDefMod, combatAttr->GetAttr(BASIC_PhysicalDef) * (1 + defcoeff));
	watch.SetAttr(BASIC_MagicDefMod, combatAttr->GetAttr(BASIC_MagicDef) * (1 + defcoeff));

	watch.SetAttr(PERCENT_PhysicalAtkMod, combatAttr->GetAttr(PERCENT_PhysicalAtk));
	watch.SetAttr(PERCENT_PhysicalDefMod, combatAttr->GetAttr(PERCENT_PhysicalDef));
	watch.SetAttr(PERCENT_MagicAtkMod, combatAttr->GetAttr(PERCENT_MagicAtk));
	watch.SetAttr(PERCENT_MagicDefMod, combatAttr->GetAttr(PERCENT_MagicDef));

	watch.CheckChangedAttributeAndNotify(false);
}

///> Ôö¼ÓÊôÐÔ
void PowerPointMgr::AdjustOneAttributeByCoeffcient(Role *pRole, int nAttr, float coeff)
{
	int nAttrMod = 0;
	switch (nAttr)
	{
	case BASIC_PhysicalAtk:
		{
			nAttrMod = BASIC_PhysicalAtkMod;
			break;
		}
	case BASIC_PhysicalDef:
		{
			nAttrMod = BASIC_PhysicalDefMod;
			break;
		}
	case BASIC_MagicAtk:
		{
			nAttrMod = BASIC_MagicAtkMod;
			break;
		}
	case BASIC_MagicDef:
		{
			nAttrMod = BASIC_MagicDefMod;
			break;
		}
	default:
		break;
	}
	if (0 == nAttrMod)
	{
		return;
	}
	CombatAttribute *combatAttr = pRole->GetAttrPtr();
	AttributeWatcher watch(pRole);

	watch.AddAttr((CombatAttrDef)(nAttrMod + CA_PERCENT_START), coeff*100);

	watch.CheckChangedAttributeAndNotify(false);
}

double PowerPointMgr::_GetPowerPointAttributeWeight(int attribute, UINT32 prof)
{
	auto iter = m_oCoeff.find(attribute);
	if(iter == m_oCoeff.end())
	{
		LogError("attribute:%d, prof:%u can't find in AttributeList.txt", attribute, prof);
		return 0;
	}
	if(iter->second.size() == 1)
	{
		return iter->second[0].weight;
	}
	for(UINT32 i = 0; i < iter->second.size(); i++)
	{
		if(iter->second[i].prof == prof)
		{
			return iter->second[i].weight;
		}
	}
	LogError("attribute:%d, prof:%u can't find in AttributeList.txt", attribute, prof);
	return 0;
}

double PowerPointMgr::CalcPowerPoint(CombatAttribute *combatAttr, int prof)
{
	double ppt = 0;
	prof = ProfessionMgr::Instance()->GetBasicProfession(prof);
	for (auto i = m_oCoeff.begin(); i != m_oCoeff.end(); ++i)
	{
		double weight = _GetPowerPointAttributeWeight(i->first, prof);
		ppt += combatAttr->GetAttr((CombatAttrDef)(CA_TOTAL_START + i->first)) * weight;
	}

	return ppt;
}
