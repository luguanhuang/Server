#include "pch.h"
#include "SkillReader.h"
#include <tinyxml2.h>
#include "baseconfig.h"
#include "util.h"

using namespace tinyxml2;

bool isTrue(const char *value)
{
	return strcmp(value, "true") == 0 || strcmp(value, "True") == 0 || strcmp(value, "TRUE") == 0;
}

#define IsEnum(strEnum, value) if (strcmp(str, strEnum) == 0) return value

XWarningType XWarningTypeConvert(const char *str)
{
	IsEnum("Warning_None", Warning_None);
	IsEnum("Warning_Target", Warning_Target);
	IsEnum("Warning_Multiple", Warning_Multiple);
	IsEnum("Warning_All",  Warning_All);
	return Warning_None;
}

XStrickenResponse XStrickenResponseConvert(const char *str)
{
	IsEnum("Invincible", XSR_Invincible);
	IsEnum("Half_Endure", XSR_Half_Endure);
	IsEnum("Full_Endure", XSR_Full_Endure);
	IsEnum("Cease",  XSR_Cease);
	return XSR_Cease;
}

XResultAffectDirection XResultAffectDirectionConvert(const char *str)
{
	IsEnum("AttackDir", AttackDir);
	IsEnum("ChargeDir",  ChargeDir);
	return AttackDir;
}

XResultBulletType XResultBulletTypeConvert(const char *str)
{
	IsEnum("Sphere", Sphere);
	IsEnum("Plane", Plane);
	IsEnum("Satellite", Satellite);
	IsEnum("Ring", Ring);
	return Sphere;
}

XBeHitState XBeHitStateConvert(const char *str)
{
	IsEnum("Hit_Back", Hit_Back);
	IsEnum("Hit_Fly",  Hit_Fly);
	IsEnum("Hit_Roll",  Hit_Roll);
	IsEnum("Hit_Freezed",  Hit_Freezed);
	IsEnum("Hit_Free",  Hit_Free);
	return Hit_Back;
}

XBeHitState_Animation XBeHitState_AnimationConvert(const char *str)
{
	IsEnum("Hit_Back_Front", Hit_Back_Front);
	IsEnum("Hit_Back_Left", Hit_Back_Left);
	IsEnum("Hit_Back_Right", Hit_Back_Right);
	return Hit_Back_Front;
}

#define GetFloatValue(pointer, Member) node = current->FirstChildElement(#Member); if (node) { pointer->Member = convert<float>(node->GetText()); }
#define GetBoolValue(pointer, Member)  node = current->FirstChildElement(#Member); if (node) { pointer->Member = isTrue(node->GetText()); }
#define GetIntValue(pointer, Member)   node = current->FirstChildElement(#Member); if (node) { pointer->Member = convert<int>(node->GetText()); }
#define GetEnumValue(pointer, Member, EnumType) node = current->FirstChildElement(#Member); if (node) { pointer->Member = EnumType##Convert(node->GetText()); }
#define GetStringValue(pointer, Member) node = current->FirstChildElement(#Member); if (node) { pointer->Member = (node->GetText() ? node->GetText() : ""); }

static void LoadJa(XSkillData * skillData, XMLElement * elem)
{
	const char *ListRootNodeName = "XJAData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Ja.push_back(XJAData());
		XJAData *pJa = &skillData->Ja.back();
		
		GetFloatValue(pJa, At);
		GetFloatValue(pJa, End);
		GetFloatValue(pJa, Point);
		GetStringValue(pJa, Next_Name);
		GetStringValue(pJa, Name);
		GetIntValue(pJa, Index);
	}
}

static void LoadCombined(XSkillData * skillData, XMLElement * elem)
{
	const char *ListRootNodeName = "XCombinedData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Combined.push_back(XCombinedData());
		XCombinedData *pCombined = &skillData->Combined.back();

		GetFloatValue(pCombined, At);
		GetFloatValue(pCombined, End);
		GetStringValue(pCombined, Name);
		GetIntValue(pCombined, Index);
	}
}

static void LoadWarning(XSkillData * skillData, XMLElement * elem)
{
	const char *ListRootNodeName = "XWarningData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Warning.push_back(XWarningData());
		XWarningData *pWarningData = &skillData->Warning.back();

		GetEnumValue(pWarningData, Type, XWarningType);
		GetFloatValue(pWarningData, OffsetX);
		GetFloatValue(pWarningData, OffsetY);
		GetFloatValue(pWarningData, OffsetZ);

		GetIntValue(pWarningData, Index);
		GetFloatValue(pWarningData, At);
		GetBoolValue(pWarningData, Mobs_Inclusived);

		GetIntValue(pWarningData, MaxRandomTarget);
		GetIntValue(pWarningData, PosRandomCount);
		GetFloatValue(pWarningData, PosRandomRange);
		GetBoolValue(pWarningData, RandomWarningPos);
	}
}

static void LoadChargeData(XSkillData * skillData, XMLElement * elem)
{
	const char *ListRootNodeName = "XChargeData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Charge.push_back(XChargeData());
		XChargeData *pChargeData = &skillData->Charge.back();
		
		GetFloatValue(pChargeData, At);
		GetFloatValue(pChargeData, End);
		GetFloatValue(pChargeData, Offset);
		GetFloatValue(pChargeData, Velocity);
		GetFloatValue(pChargeData, Height);
		GetFloatValue(pChargeData, Rotation_Speed);
		GetBoolValue(pChargeData,  Using_Curve);
		GetBoolValue(pChargeData,  Using_Up);
		GetStringValue(pChargeData, Curve_Forward);
		GetStringValue(pChargeData, Curve_Side);
		GetStringValue(pChargeData, Curve_Up);
		GetBoolValue(pChargeData,  StandOnAtEnd);
		GetBoolValue(pChargeData,  AimTarget);
		GetBoolValue(pChargeData,  Control_Towards);
		GetIntValue(pChargeData, Index);
	}
}

static void LoadLongAttackData(XResultData * pResult, XMLElement * elem)
{
	XMLElement *node = NULL;
	XMLElement *current = elem;

	XLongAttackResultData *pLongAttackData = &pResult->LongAttackData;

	pLongAttackData->Valid = true;

	GetBoolValue(pLongAttackData,  WithCollision);
	GetBoolValue(pLongAttackData,  Follow);
	GetFloatValue(pLongAttackData, Runningtime);
	GetFloatValue(pLongAttackData, Stickytime);
	GetFloatValue(pLongAttackData, Velocity);
	GetFloatValue(pLongAttackData, Radius);
	GetFloatValue(pLongAttackData, Palstance);
	GetFloatValue(pLongAttackData, RingRadius);
	GetFloatValue(pLongAttackData, RingVelocity);
	GetBoolValue(pLongAttackData,  RingFull);
	GetBoolValue(pLongAttackData,  TriggerOnce);
	GetBoolValue(pLongAttackData,  TriggerAtEnd);
	GetBoolValue(pLongAttackData,  Reinforce);
	GetFloatValue(pLongAttackData, TriggerAtEnd_Cycle);
	GetIntValue(pLongAttackData, TriggerAtEnd_Count);
	GetBoolValue(pLongAttackData,  FlyWithTerrain);
	GetBoolValue(pLongAttackData,  AimTargetCenter);
	GetIntValue(pLongAttackData, FireAngle);
	GetFloatValue(pLongAttackData, At_X);
	GetFloatValue(pLongAttackData, At_Y);
	GetFloatValue(pLongAttackData, At_Z);
	GetEnumValue(pLongAttackData, Type, XResultBulletType);
	GetFloatValue(pLongAttackData, Refine_Cycle);
	GetIntValue(pLongAttackData, Refine_Count);
	GetBoolValue(pLongAttackData, IsPingPong);
	GetBoolValue(pLongAttackData, StaticCollider);
	GetBoolValue(pLongAttackData, DynamicCollider);
	GetBoolValue(pLongAttackData, AutoRefine_at_Half);
	GetBoolValue(pLongAttackData, Manipulation);
	GetFloatValue(pLongAttackData, ManipulationRadius);
	GetFloatValue(pLongAttackData, ManipulationForce);
}

static void LoadResult(XSkillData * skillData, XMLElement * elem)
{
	const char *ListRootNodeName = "XResultData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Result.push_back(XResultData());
		XResultData *pResult = &skillData->Result.back();

		GetIntValue(pResult, Index);

		GetFloatValue(pResult, At);
		GetFloatValue(pResult, Range);
		GetFloatValue(pResult, Low_Range);
		GetFloatValue(pResult, Scope);
		GetBoolValue(pResult,  Loop);
		GetBoolValue(pResult,  Group);
		GetFloatValue(pResult, Cycle);
		GetIntValue(pResult, Deviation_Angle);
		GetIntValue(pResult, Angle_Step);
		GetFloatValue(pResult, Time_Step);
		GetIntValue(pResult, Group_Count);
		GetIntValue(pResult, Loop_Count);
		GetIntValue(pResult, Token);
		GetBoolValue(pResult,  Clockwise);
		GetBoolValue(pResult, LongAttackEffect);
		GetBoolValue(pResult, Sector_Type);
		GetBoolValue(pResult, Rect_HalfEffect);
		GetIntValue(pResult, None_Sector_Angle_Shift);
		GetBoolValue(pResult, Attack_All);
		GetBoolValue(pResult, Attack_Only_Target);
		GetIntValue(pResult, Warning_Idx);
		GetBoolValue(pResult, Warning);
		GetFloatValue(pResult, Offset_X);
		GetFloatValue(pResult, Offset_Z);
		GetEnumValue(pResult, Affect_Direction, XResultAffectDirection);
		GetBoolValue(pResult, Mobs_Inclusived);

		XMLElement *e = current->FirstChildElement("LongAttackData");
		if (e != NULL)
		{
			LoadLongAttackData(pResult, e);
		}
	}
}

static void LoadManipulation(XSkillData* skillData, XMLElement* elem)
{
	const char *ListRootNodeName = "XManipulationData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Manipulation.push_back(XManipulationData());
		XManipulationData *pmanipulation = &skillData->Manipulation.back();

		GetFloatValue(pmanipulation, At);
		GetFloatValue(pmanipulation, End);
		GetFloatValue(pmanipulation, OffsetX);
		GetFloatValue(pmanipulation, OffsetZ);
		GetFloatValue(pmanipulation, Radius);
		GetFloatValue(pmanipulation, Force);
		GetFloatValue(pmanipulation, Degree);
	}
}

static void LoadHit(XSkillData * skillData, XMLElement * elem)
{
	const char *ListRootNodeName = "XHitData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Hit.push_back(XHitData());
		XHitData *pHit = &skillData->Hit.back();

		GetIntValue(pHit, Index);

		GetFloatValue(pHit, Time_Present_Straight);
		GetFloatValue(pHit, Time_Hard_Straight);
		GetFloatValue(pHit, Offset);
		GetFloatValue(pHit, Height);
		GetEnumValue(pHit, State, XBeHitState);
		GetEnumValue(pHit, State_Animation, XBeHitState_Animation);
		GetFloatValue(pHit, Random_Range);
		GetBoolValue(pHit,  CurveUsing);
		GetFloatValue(pHit, FreezeDuration);

		GetBoolValue(pHit,  Additional_Using_Default);
		GetFloatValue(pHit, Additional_Hit_Time_Present_Straight);
		GetFloatValue(pHit, Additional_Hit_Time_Hard_Straight);
		GetFloatValue(pHit, Additional_Hit_Offset);
		GetFloatValue(pHit, Additional_Hit_Height);
	}
}

static void LoadMobData(XSkillData * skillData, XMLElement * elem)
{
	const char *ListRootNodeName = "XMobUnitData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Mobs.push_back(XMobUnitData());
		XMobUnitData *pmob = &skillData->Mobs.back();

		GetIntValue(pmob, TemplateID);

		GetFloatValue(pmob, At);
		GetFloatValue(pmob, Offset_At_X);
		GetFloatValue(pmob, Offset_At_Y);
		GetFloatValue(pmob, Offset_At_Z);

		GetBoolValue(pmob, LifewithinSkill);
		GetBoolValue(pmob, Shield);
	}
}

static void LoadChainData(XSkillData * skillData, XMLElement * elem)
{
	XCastChain *pChain = &skillData->Chain;
	XMLElement *node = NULL;
	XMLElement *current = elem;

	GetIntValue(pChain, TemplateID);

	GetFloatValue(pChain, At);
	GetStringValue(pChain, Name);
}

static void LoadQTEData(XSkillData * skillData, XMLElement * elem)
{
	const char *ListRootNodeName = "XQTEData";
	for (XMLElement *current = elem->FirstChildElement(ListRootNodeName); 
		current != NULL; current = current->NextSiblingElement(ListRootNodeName))
	{
		XMLElement *node = NULL;
		skillData->Logical.QTEData.push_back(XQTEData());
		XQTEData *pQTEData = &skillData->Logical.QTEData.back();

		GetIntValue(pQTEData, QTE);
		GetFloatValue(pQTEData, At);
		GetFloatValue(pQTEData, End);
	}
}

static void LoadLogicalData(XSkillData * skillData, XMLElement * elem)
{
	XLogicalData *pLogical = &skillData->Logical;
	XMLElement *node = NULL;
	XMLElement *current = elem;

	GetIntValue(pLogical, Index);

	GetIntValue(pLogical, CanReplacedby);
	GetEnumValue(pLogical, StrickenMask, XStrickenResponse);

	GetFloatValue(pLogical, Not_Move_At);
	GetFloatValue(pLogical, Not_Move_End);
	GetFloatValue(pLogical, Not_Dash_At);
	GetFloatValue(pLogical, Not_Dash_End);
	GetFloatValue(pLogical, Rotate_At);
	GetFloatValue(pLogical, Rotate_End);
	GetFloatValue(pLogical, Rotate_Speed);
	GetFloatValue(pLogical, CanCancelAt);

	GetIntValue(pLogical, PreservedStrength);
	GetFloatValue(pLogical, PreservedAt);
	GetFloatValue(pLogical, PreservedEndAt);

	GetIntValue(pLogical, CanCastAt_QTE);

	GetBoolValue(pLogical,  MoveType);
	GetBoolValue(pLogical,  AttackOnHitDown);
	GetBoolValue(pLogical,  Association);

	GetStringValue(pLogical, Association_Skill);
	GetStringValue(pLogical, Syntonic_CoolDown_Skill);

	GetStringValue(pLogical, Exstring);
	GetFloatValue(pLogical, Exstring_At);

	XMLElement *e = elem->FirstChildElement("QTEData");
	if (e != NULL)
	{
		LoadQTEData(skillData, e);
	}
}

static void LoadScriptData(XSkillData * skillData, XMLElement * elem)
{
	XScriptData *pScript = &skillData->Script;
	XMLElement *node = NULL;
	XMLElement *current = elem;

	GetStringValue(pScript, Start_Name);
	GetStringValue(pScript, Update_Name);
	GetStringValue(pScript, Stop_Name);
	GetStringValue(pScript, Result_Name);
}

void XSkillData::Release()
{
	if(_reference > 0) 
	{
		--_reference;
	}
	else
		LogError("Reference of skill %s has already been 0.", Name.c_str());

	if(0 == _reference && _dirty) 
	{
		LogInfo("Deleted skill data of name %s when released.", Name.c_str());
		delete this;
	}
}

static std::map<std::string, XSkillData*> Cache;

void SkillReader::HotPatchSkill(const char *fileName)
{
	LogInfo("Hot patch skill data of filename %s", fileName);

	auto i = Cache.find(fileName);

	if (i != Cache.end())
	{
		XSkillData* data = i->second;
		data->Dirty();

		Cache.erase(i);

		if(data->IsNoneReferenced())
		{
			LogInfo("Deleted skill data of name %s.", data->Name.c_str());
			delete data;
		}
	}
}

XSkillData* SkillReader::LoadSkill(const char *fileName)
{
	auto i = Cache.find(fileName);
	if (i != Cache.end())
	{
		i->second->Referenced();
		return i->second;
	}

	tinyxml2::XMLDocument doc;
	XMLError error = doc.LoadFile(Config::GetConfig()->GetFilePath(fileName).c_str());
	if (error != XML_SUCCESS)
	{
		LogError("parser skill file %s get XMLError code: %d", fileName, error);
		assert(false);
	}

	XMLElement *SkillDataNode = doc.FirstChildElement("XSkillData");
	XSkillData *skillData = new XSkillData();
	XMLElement *elem = NULL;
	XMLElement *node = NULL;
	XMLElement *current = SkillDataNode;

	GetStringValue(skillData, Name);
	GetIntValue(skillData, TypeToken);
	GetBoolValue(skillData, NeedTarget);
	GetBoolValue(skillData, OnceOnly);
	GetBoolValue(skillData, ForCombinedOnly);
	GetBoolValue(skillData, IgnoreCollision);
	GetFloatValue(skillData, CoolDown);
	GetFloatValue(skillData, Time);
	GetBoolValue(skillData, Cast_Range_Rect);
	GetFloatValue(skillData, Cast_Offset_X);
	GetFloatValue(skillData, Cast_Offset_Z);
	GetFloatValue(skillData, Cast_Range_Upper);
	GetFloatValue(skillData, Cast_Range_Lower);
	GetFloatValue(skillData, Cast_Scope);
	GetFloatValue(skillData, Cast_Scope_Shift);
	GetBoolValue(skillData, MultipleAttackSupported);
	GetFloatValue(skillData, BackTowardsDecline);
	GetStringValue(skillData, PVP_Script_Name);

	elem = SkillDataNode->FirstChildElement("Logical");
	if (elem != NULL)
	{
		LoadLogicalData(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Mob");
	if (elem != NULL)
	{
		LoadMobData(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Chain");
	if (elem != NULL)
	{
		LoadChainData(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Script");
	if (elem != NULL)
	{
		LoadScriptData(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Charge");
	if (elem != NULL)
	{
		LoadChargeData(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Result");
	if (elem != NULL)
	{
		LoadResult(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Hit");
	if (elem != NULL)
	{
		LoadHit(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Manipulation");
	if (elem != NULL)
	{
		LoadManipulation(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Ja");
	if (elem != NULL)
	{
		LoadJa(skillData, elem);
	}
	
	elem = SkillDataNode->FirstChildElement("Warning");
	if (elem != NULL)
	{
		LoadWarning(skillData, elem);
	}

	elem = SkillDataNode->FirstChildElement("Combined");
	if (elem != NULL)
	{
		LoadCombined(skillData, elem);
	}

	Cache[fileName] = skillData;
	skillData->Referenced();

	return skillData;
}
