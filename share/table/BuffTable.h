#ifndef __BuffTable_H__
#define __BuffTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/28 11:10:24


class BuffTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	BuffID;
		int	BuffLevel;
		float	BuffDuration;
		vector<Sequence<float, 3>>	BuffChangeAttribute;
		vector<int>	BuffState;
		int	BuffMergeType;
		int	TargetType;
		string	BuffIcon;
		vector<Sequence<float, 3>>	BuffDOT;
		string	BuffFx;
		string	BuffDoodadFx;
		string	BuffName;
		int	BuffTriggerCond;
		float	BuffTriggerRate;
		vector<string>	BuffTriggerParam;
		vector<Sequence<int, 2>>	BuffTriggerBuff;
		double	CostModify;
		double	CoolDownModify;
		double	BuffTriggerCD;
		vector<Sequence<int, 2>>	AuraAddBuffID;
		bool	BuffIsVisible;
		string	BuffEffectFx;
		vector<float>	AuraParams;
		vector<Sequence<float, 2>>	DamageReduce;
		bool	DontShowText;
		vector<int>	EffectGroup;
		vector<Sequence<int, 2>>	BuffDOTValueFromCaster;
		Sequence<double, 2>	ChangeDamage;
		int	BuffClearType;
		vector<int>	ClearTypes;
		float	DamageReflection;
		uint	MobID;
		Sequence<float, 2>	BuffHP;
		uint	StackMaxCount;
		int	ChangeFightGroup;
		uint	BuffTriggerCount;
		Sequence<float, 2>	LifeSteal;
		vector<Sequence<string, 3>>	ReduceSkillCD;
		int	StateParam;
		bool	IsGlobalTrigger;
		vector<uint>	Tags;
		string	BuffSpriteFx;
		string	MiniMapIcon;
		string	BuffTriggerSkill;
		vector<Sequence<string, 2>>	ChangeSkillDamage;
		vector<int>	SceneEffect;
		vector<Sequence<float, 2>>	TargetLifeAddAttack;
		int	AIEvent;
		vector<string>	RelevantSkills;
		bool	IsTriggerImm;
		vector<float>	Manipulate;
		vector<Sequence<string, 2>>	SkillsReplace;
		vector<Sequence<float, 2>>	SelfLifeAddAttack;
		vector<Sequence<float, 2>>	ChangeCastDamageByDistance;
		int	Kill;

		void CopyFrom(RowData *pRowData)
		{
			this->BuffID=pRowData->BuffID;
			this->BuffLevel=pRowData->BuffLevel;
			this->BuffDuration=pRowData->BuffDuration;
			this->BuffChangeAttribute=pRowData->BuffChangeAttribute;
			this->BuffState=pRowData->BuffState;
			this->BuffMergeType=pRowData->BuffMergeType;
			this->TargetType=pRowData->TargetType;
			this->BuffIcon=pRowData->BuffIcon;
			this->BuffDOT=pRowData->BuffDOT;
			this->BuffFx=pRowData->BuffFx;
			this->BuffDoodadFx=pRowData->BuffDoodadFx;
			this->BuffName=pRowData->BuffName;
			this->BuffTriggerCond=pRowData->BuffTriggerCond;
			this->BuffTriggerRate=pRowData->BuffTriggerRate;
			this->BuffTriggerParam=pRowData->BuffTriggerParam;
			this->BuffTriggerBuff=pRowData->BuffTriggerBuff;
			this->CostModify=pRowData->CostModify;
			this->CoolDownModify=pRowData->CoolDownModify;
			this->BuffTriggerCD=pRowData->BuffTriggerCD;
			this->AuraAddBuffID=pRowData->AuraAddBuffID;
			this->BuffIsVisible=pRowData->BuffIsVisible;
			this->BuffEffectFx=pRowData->BuffEffectFx;
			this->AuraParams=pRowData->AuraParams;
			this->DamageReduce=pRowData->DamageReduce;
			this->DontShowText=pRowData->DontShowText;
			this->EffectGroup=pRowData->EffectGroup;
			this->BuffDOTValueFromCaster=pRowData->BuffDOTValueFromCaster;
			this->ChangeDamage=pRowData->ChangeDamage;
			this->BuffClearType=pRowData->BuffClearType;
			this->ClearTypes=pRowData->ClearTypes;
			this->DamageReflection=pRowData->DamageReflection;
			this->MobID=pRowData->MobID;
			this->BuffHP=pRowData->BuffHP;
			this->StackMaxCount=pRowData->StackMaxCount;
			this->ChangeFightGroup=pRowData->ChangeFightGroup;
			this->BuffTriggerCount=pRowData->BuffTriggerCount;
			this->LifeSteal=pRowData->LifeSteal;
			this->ReduceSkillCD=pRowData->ReduceSkillCD;
			this->StateParam=pRowData->StateParam;
			this->IsGlobalTrigger=pRowData->IsGlobalTrigger;
			this->Tags=pRowData->Tags;
			this->BuffSpriteFx=pRowData->BuffSpriteFx;
			this->MiniMapIcon=pRowData->MiniMapIcon;
			this->BuffTriggerSkill=pRowData->BuffTriggerSkill;
			this->ChangeSkillDamage=pRowData->ChangeSkillDamage;
			this->SceneEffect=pRowData->SceneEffect;
			this->TargetLifeAddAttack=pRowData->TargetLifeAddAttack;
			this->AIEvent=pRowData->AIEvent;
			this->RelevantSkills=pRowData->RelevantSkills;
			this->IsTriggerImm=pRowData->IsTriggerImm;
			this->Manipulate=pRowData->Manipulate;
			this->SkillsReplace=pRowData->SkillsReplace;
			this->SelfLifeAddAttack=pRowData->SelfLifeAddAttack;
			this->ChangeCastDamageByDistance=pRowData->ChangeCastDamageByDistance;
			this->Kill=pRowData->Kill;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BuffID", "BuffLevel", "BuffDuration", "BuffChangeAttribute", "BuffState", "BuffMergeType", "TargetType", "BuffIcon", "BuffDOT", "BuffFx", "BuffDoodadFx", "BuffName", "BuffTriggerCond", "BuffTriggerRate", "BuffTriggerParam", "BuffTriggerBuff", "CostModify", "CoolDownModify", "BuffTriggerCD", "AuraAddBuffID", "BuffIsVisible", "BuffEffectFx", "AuraParams", "DamageReduce", "DontShowText", "EffectGroup", "BuffDOTValueFromCaster", "ChangeDamage", "BuffClearType", "ClearTypes", "DamageReflection", "MobID", "BuffHP", "StackMaxCount", "ChangeFightGroup", "BuffTriggerCount", "LifeSteal", "ReduceSkillCD", "StateParam", "IsGlobalTrigger", "Tags", "BuffSpriteFx", "MiniMapIcon", "BuffTriggerSkill", "ChangeSkillDamage", "SceneEffect", "TargetLifeAddAttack", "AIEvent", "RelevantSkills", "IsTriggerImm", "Manipulate", "SkillsReplace", "SelfLifeAddAttack", "ChangeCastDamageByDistance", "Kill",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BuffTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.BuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.BuffLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.BuffDuration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.BuffChangeAttribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.BuffState)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BuffMergeType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.TargetType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.BuffIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.BuffDOT)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.BuffFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.BuffDoodadFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.BuffName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.BuffTriggerCond)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.BuffTriggerRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.BuffTriggerParam)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.BuffTriggerBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.CostModify)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.CoolDownModify)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.BuffTriggerCD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.AuraAddBuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.BuffIsVisible)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.BuffEffectFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.AuraParams)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.DamageReduce)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.DontShowText)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.EffectGroup)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.BuffDOTValueFromCaster)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.ChangeDamage)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.BuffClearType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.ClearTypes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.DamageReflection)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.MobID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.BuffHP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[33]], row.StackMaxCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[34]], row.ChangeFightGroup)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[35]], row.BuffTriggerCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[36]], row.LifeSteal)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[37]], row.ReduceSkillCD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[38]], row.StateParam)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[39]], row.IsGlobalTrigger)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[40]], row.Tags)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[41]], row.BuffSpriteFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[42]], row.MiniMapIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[43]], row.BuffTriggerSkill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[44]], row.ChangeSkillDamage)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[45]], row.SceneEffect)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[46]], row.TargetLifeAddAttack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[47]], row.AIEvent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[48]], row.RelevantSkills)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[49]], row.IsTriggerImm)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[50]], row.Manipulate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[51]], row.SkillsReplace)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[52]], row.SelfLifeAddAttack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[53]], row.ChangeCastDamageByDistance)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[54]], row.Kill)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~BuffTable ()
	{
		Clear();
	}

	void Clear()
	{
		for (UINT32 i = 0; i < Table.size(); ++i)
		{
			delete Table[i];
		}

		Table.clear();


	}

	std::vector<RowData*> Table;
};

#endif