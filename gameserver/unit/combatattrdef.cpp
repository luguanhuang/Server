#include "pch.h"
#include "combatattrdef.h"

#define DefAttr(Name, Value, Comment) BASIC_##Name

CombatAttrDef AttributeDefInfo::UsedAttributeArray[] = {
	AttributeDef
};

#undef DefAttr

int AttributeDefInfo::UsedAttributeCount = sizeof(AttributeDefInfo::UsedAttributeArray)/sizeof(AttributeDefInfo::UsedAttributeArray[0]);

struct AttrNamePair
{
	CombatAttrDef AttrID;
	const char *Name;
};

#define DefAttr(Name, Value, Comment) {BASIC_##Name, Comment}

static AttrNamePair Names[] = {
	AttributeDef
};

#undef DefAttr

const char * AttributeDefInfo::GetAttributeName(CombatAttrDef attr)
{
	for (unsigned i = 0; i < sizeof(Names)/sizeof(Names[0]); ++i)
	{
		if (Names[i].AttrID == attr)
		{
			return Names[i].Name;
		}
	}

	return "<Undefined Attribute>";
}

static bool  IsOffsetBuffInit = false;
static short OffsetBuff[CA_ATTRIBUTE_COUNT] = {0};

static CombatAttrDef _FirstLevelAttributesList[]= {
	BASIC_Strength, BASIC_Agility, BASIC_Intelligence, BASIC_Vitality,
	PERCENT_Strength, PERCENT_Agility, PERCENT_Intelligence, PERCENT_Vitality,
	TOTAL_Strength, TOTAL_Agility, TOTAL_Intelligence, TOTAL_Vitality
};

static std::set<CombatAttrDef> _FirstLevelAttributesSet;
bool AttributeDefInfo::_InitFirstLevel()
{
	//CombatAttrDef _FirstLevelAttributesList[]= {
	//	BASIC_Strength, BASIC_Agility, BASIC_Intelligence, BASIC_Vitality,
	//	PERCENT_Strength, PERCENT_Agility, PERCENT_Intelligence, PERCENT_Vitality,
	//	TOTAL_Strength, TOTAL_Agility, TOTAL_Intelligence, TOTAL_Vitality
	//};

	int length = 12;
	for(int i = 0; i < length; ++i)
	{
		_FirstLevelAttributesSet.insert(_FirstLevelAttributesList[i]);
	}

	return true;
}
static bool _bFirstLevel = false;

int AttributeDefInfo::GetAttributeOffset(int attr)
{
	if (IsOffsetBuffInit == false)
	{
		IsOffsetBuffInit = true;
		unsigned nameSize = sizeof(Names) / sizeof(Names[0]);
		for (unsigned i = 0; i < nameSize; ++i)
		{
			OffsetBuff[Names[i].AttrID] = i;
		}
	}

	return OffsetBuff[attr];
}

bool AttributeDefInfo::IsBasicAttr(CombatAttrDef attrDef)
{
	if((int)attrDef >= CA_BASIC_START && (int)attrDef <= CA_BASIC_END)
	{
		return true;
	}
	return false;
}

bool AttributeDefInfo::IsPercentAttr(CombatAttrDef attrDef)
{
	if((int)attrDef >= CA_PERCENT_START && (int)attrDef <= CA_PERCENT_END)
	{
		return true;
	}
	return false;
}

bool AttributeDefInfo::IsTotalAttr(CombatAttrDef attrDef)
{
	if((int)attrDef >= CA_TOTAL_START && (int)attrDef <= CA_TOTAL_END)
	{
		return true;
	}
	return false;

}

bool AttributeDefInfo::IsFirstLevelAttr(CombatAttrDef attrDef)
{
	if (!_bFirstLevel)
	{
		_InitFirstLevel();
		_bFirstLevel = true;
	}
	return _FirstLevelAttributesSet.count(attrDef) > 0;
}

CombatAttrDef AttributeDefInfo::GetAttrCurAttr(CombatAttrDef limitAttr)
{
	switch(limitAttr)
	{
	case BASIC_MaxHP:
	case TOTAL_MaxHP:
	case PERCENT_MaxHP:
		return BASIC_CurrentHP;
	case BASIC_MaxMP:
	case TOTAL_MaxMP:
	case PERCENT_MaxMP:
		return BASIC_CurrentMP;
	case BASIC_MaxSuperArmor:
	case TOTAL_MaxSuperArmor:
	case PERCENT_MaxSuperArmor:
		return BASIC_CurrentSuperArmor;
	default:
		return Invalid;
	}
}
