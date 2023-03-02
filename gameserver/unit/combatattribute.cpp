#include "pch.h"
#include "combatattribute.h"
#include "pb/project.pb.h"
#include "timeutil.h"
#include "role.h"
#include "scene/ptcg2c_attributechangenotify.h"
#include "foreach.h"


//#define between(min_, max_, value) (((int)(value) >= (min_)) && ((int)(value) <= (max_)))

const static CombatAttrDef sBriefAttr[] = {BASIC_RUN_SPEED, BASIC_WALK_SPEED, BASIC_ROTATION_SPEED, BASIC_AUTOROTATION_SPEED, \
	BASIC_CurrentHP, BASIC_MaxHP, BASIC_CurrentMP, BASIC_MaxMP, BASIC_MaxSuperArmor, BASIC_CurrentSuperArmor, BASIC_CurrentXULI, BASIC_POWERPOINT};

struct AttrHelper
{
	AttrType m_type;
	int      m_index;
	CombatAttribute *m_pAttr;
	bool m_bIgnorePercentGetTotal;

	AttrHelper(CombatAttribute *pAttr, CombatAttrDef attrDef)
	{
		m_pAttr = pAttr;

		if (AttributeDefInfo::IsBasicAttr(attrDef))
		{
			m_type = AT_BASIC;
			m_index = AttributeDefInfo::GetAttributeOffset(attrDef - CA_BASIC_START);
		}
		else if (AttributeDefInfo::IsPercentAttr(attrDef))
		{
			m_type = AT_PERCENT;
			m_index = AttributeDefInfo::GetAttributeOffset(attrDef - CA_PERCENT_START);
		}
		else if (AttributeDefInfo::IsTotalAttr(attrDef))
		{
			m_type = AT_TOTAL;
			m_index = AttributeDefInfo::GetAttributeOffset(attrDef - CA_TOTAL_START);
			m_bIgnorePercentGetTotal = (attrDef == TOTAL_CurrentHP || attrDef == TOTAL_CurrentMP);
		}
		else
		{
			m_type = AT_NONE;
			m_index = 0;
		}
	}

	double Get()
	{
		switch(m_type)
		{
		case AT_BASIC:   return m_pAttr->m_BasicAttr[m_index];
		case AT_PERCENT: return m_pAttr->m_PercentAttr[m_index];
		case AT_TOTAL:   return m_bIgnorePercentGetTotal ? 
							 m_pAttr->m_BasicAttr[m_index] : 
							(m_pAttr->m_BasicAttr[m_index] * (std::max(0.0, m_pAttr->m_PercentAttr[m_index] + 1.0)));
		default:         return 0;
		}
	}

	void Set(double value)
	{
		switch(m_type)
		{
		case AT_BASIC:   m_pAttr->m_BasicAttr[m_index] = value; break;
		case AT_PERCENT: m_pAttr->m_PercentAttr[m_index] = value/100.0; break;
		case AT_TOTAL:
			{
				LogWarn("error, can't set total attribute value for attr[%u]", AttributeDefInfo::UsedAttributeArray[m_index]);
				assert("set total attr" && false);
				break;
			}
		default:         break;
		}
	}

	void Add(double value)
	{
		switch(m_type)
		{
		case AT_BASIC:   m_pAttr->m_BasicAttr[m_index] += value; break;
		case AT_PERCENT: m_pAttr->m_PercentAttr[m_index] += value/100.0; break;
		case AT_TOTAL:   LogWarn("error, can't add total attribute value for attr[%u]", AttributeDefInfo::UsedAttributeArray[m_index]); break;
		default:         break;
		}
	}
};



CombatAttribute::CombatAttribute()
{
	m_BasicAttr.resize(AttributeDefInfo::UsedAttributeCount, 0);
	m_PercentAttr.resize(AttributeDefInfo::UsedAttributeCount, 0);
	m_IsLoad = false;
	Clear();
}

CombatAttribute::~CombatAttribute()
{
}

void CombatAttribute::Init()
{
	
}

void CombatAttribute::Uninit()
{

}


double CombatAttribute::GetAttr(CombatAttrDef attrDef)
{
	AttrHelper h(this, attrDef);
	return h.Get();
}

void CombatAttribute::SetAttr(CombatAttrDef attrDef, double value)
{
	AttrHelper h(this, attrDef);
	h.Set(value);
}

double CombatAttribute::CheckAttributeRange(CombatAttrDef attrDef, CombatAttrDef maxAttrDef)
{
	double curV = GetAttr(attrDef);
	double maxV = GetAttr(maxAttrDef);
	if (curV > maxV)
	{
		SetAttr(attrDef, maxV);
		return curV - maxV;
	}

	return 0;
}


double CombatAttribute::AddAttr(CombatAttrDef attrDef, double value)
{	
	AttrHelper h(this, attrDef);
	h.Add(value);

	double actualValue = value;

	if (attrDef == BASIC_CurrentHP)
	{
		actualValue -= CheckAttributeRange(BASIC_CurrentHP, TOTAL_MaxHP);
	}

	else if (attrDef == BASIC_CurrentMP)
	{
		actualValue -= CheckAttributeRange(BASIC_CurrentMP, TOTAL_MaxMP);
	}

	else if (attrDef == BASIC_CurrentSuperArmor)
	{
		actualValue -= CheckAttributeRange(BASIC_CurrentSuperArmor, TOTAL_MaxSuperArmor);
	}

	return actualValue;
}


bool CombatAttribute::LoadFromAttributes(const KKSG::Attribute &attr)
{
	Clear();

	for (int i = 0; i < attr.attrid_size(); ++i)
	{
		int attrIDOffset = AttributeDefInfo::GetAttributeOffset(attr.attrid(i));
		m_BasicAttr[attrIDOffset] = attr.basicattribute(i);
		m_PercentAttr[attrIDOffset] = attr.percentattribute(i);
	}

	m_IsLoad = true;
	return true;
}

bool CombatAttribute::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	const KKSG::Attribute &attr = poRoleAllInfo->attributes();
	return LoadFromAttributes(attr);
}

void CombatAttribute::SaveToAttributes(KKSG::Attribute &attr)
{
	attr.Clear();
	for (int i = 0; i < AttributeDefInfo::UsedAttributeCount; ++i)
	{
		CombatAttrDef attrID = AttributeDefInfo::UsedAttributeArray[i];
		attr.add_attrid(attrID);
		attr.add_basicattribute(m_BasicAttr[i]);
		attr.add_percentattribute(m_PercentAttr[i]);
	}
}

void CombatAttribute::SaveToBriefAttributes(KKSG::Attribute &attr)
{
	attr.Clear();
	int attrSize = sizeof(sBriefAttr) / sizeof(sBriefAttr[0]);
	for (int i = 0; i < attrSize; ++i)
	{
		attr.add_attrid(sBriefAttr[i]);
		int index = AttributeDefInfo::GetAttributeOffset(sBriefAttr[i] - CA_BASIC_START);
		attr.add_basicattribute(m_BasicAttr[index]);
		attr.add_percentattribute(m_PercentAttr[index]);
	}
}

void CombatAttribute::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	KKSG::Attribute &attr = *poRoleAllInfo->mutable_attributes();
	SaveToAttributes(attr);
	roChanged.insert(&attr);
}

bool CombatAttribute::IsLoad()
{
	return m_IsLoad;
}


void CombatAttribute::Clear()
{
	m_BasicAttr.clear();
	m_BasicAttr.resize(AttributeDefInfo::UsedAttributeCount, 0);
	m_PercentAttr.clear();
	m_PercentAttr.resize(AttributeDefInfo::UsedAttributeCount, 0);
}

void CombatAttribute::operator=(const CombatAttribute &attr)
{
	m_IsLoad = attr.m_IsLoad;
	m_BasicAttr = attr.m_BasicAttr;
	m_PercentAttr = attr.m_PercentAttr;
}

std::vector<AttrDiff> CombatAttribute::Diff(CombatAttribute *pOldAttr)
{
	AttrDiff aDiff;
	std::vector<AttrDiff> ret;
	for (int i = 0; i < AttributeDefInfo::UsedAttributeCount; ++i)
	{
		if (pOldAttr->m_BasicAttr[i] != m_BasicAttr[i])
		{
			aDiff.id = (CombatAttrDef)(AttributeDefInfo::UsedAttributeArray[i] + CA_BASIC_START);
			aDiff.oldValue = pOldAttr->m_BasicAttr[i];
			aDiff.newValue = m_BasicAttr[i];
			ret.push_back(aDiff);
		}

		if (pOldAttr->m_PercentAttr[i] != m_PercentAttr[i])
		{
			aDiff.id = (CombatAttrDef)(AttributeDefInfo::UsedAttributeArray[i] + CA_PERCENT_START);
			aDiff.oldValue = pOldAttr->m_PercentAttr[i];
			aDiff.newValue = m_PercentAttr[i];
			ret.push_back(aDiff);
		}
	}

	return ret;
}

void CombatAttribute::Scale(float scale)
{
	int off = AttributeDefInfo::GetAttributeOffset(BASIC_RUN_SPEED);
	for (int i = 0; i < off; ++i)
	{
		m_BasicAttr[i] *= scale;
	}
}

CombatAttributeSnapshot::CombatAttributeSnapshot(CombatAttribute *pAttr)
{
	m_savedAttribute = *pAttr;
}

void CombatAttributeSnapshot::NotifyAttributeChange(CombatAttribute *pNewAttr, Role *pRole)
{
	auto V = pNewAttr->Diff(&m_savedAttribute);
	if (!V.empty())
	{
		PtcG2C_AttributeChangeNotify oAttrPtc;
		foreach(i in V)
		{
			AttrDiff &df = *i;
			oAttrPtc.m_Data.add_attrid(df.id);
			oAttrPtc.m_Data.add_attrvalue(df.newValue);
		}

		oAttrPtc.m_Data.set_uid(pRole->GetID());
		oAttrPtc.m_Data.set_time(TimeUtil::GetFrameTime());
		pRole->Send(oAttrPtc);
	}
}
