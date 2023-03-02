#ifndef __SkillList_H__
#define __SkillList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/25 14:53:13


class SkillList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	SkillScript;
		int	SkillLevel;
		string	ScriptName;
		vector<Sequence<float, 2>>	PhysicalRatio;
		vector<Sequence<float, 2>>	PhysicalFixed;
		vector<Sequence<int, 2>>	DefenceSlotReduce;
		vector<int>	AddBuffPoint;
		vector<Sequence<int, 3>>	BuffID;
		int	HpMaxLimit;
		int	UnlockLevel;
		vector<int>	LevelupCost;
		vector<int>	UpReqRoleLevel;
		string	CurrentLevelDescription;
		string	NextLevelDescription;
		string	Icon;
		vector<Sequence<float, 2>>	MagicRatio;
		vector<Sequence<float, 2>>	MagicFixed;
		int	Element;
		int	Profession;
		int	SkillType;
		int	IncreaseSuperArmor;
		vector<float>	DecreaseSuperArmor;
		int	IsBasicSkill;
		Sequence<float, 2>	CostMP;
		vector<Sequence<float, 2>>	TipsRatio;
		vector<Sequence<float, 2>>	TipsFixed;
		string	PreSkill;
		int	XPostion;
		int	YPostion;
		vector<Sequence<float, 4>>	StartBuffID;
		vector<Sequence<int, 3>>	PassiveAttribute;
		int	LevelupCostType;
		vector<int>	UpReqGuildLevel;
		Sequence<int, 3>	AuraBuffID;
		int	HpMinLimit;
		Sequence<float, 2>	CDRatio;
		Sequence<float, 2>	PvPCDRatio;
		int	SuperArmorMax;
		int	SuperArmorMin;
		uint	XEntityStatisticsID;
		int	PvPIncreaseSuperArmor;
		vector<float>	PvPDecreaseSuperArmor;
		vector<Sequence<float, 2>>	PvPRatio;
		vector<Sequence<float, 2>>	PvPFixed;
		int	ResetSkillTo;
		float	InitCD;
		float	PvPInitCD;
		vector<int>	StudyLevelCost;
		vector<Sequence<float, 2>>	PvPMagicRatio;
		vector<Sequence<float, 2>>	PvPMagicFixed;
		string	PreviewScript;
		vector<uint>	MobBuffs;
		string	Atlas;
		vector<uint>	Flag;
		int	PreSkillPoint;
		string	SuperIndureAttack;
		string	SuperIndureDefense;
		string	ExSkillScript;
		int	UnchangableCD;
		float	EnmityRatio;
		int	EnmityExtValue;
		vector<Sequence<float, 2>>	PercentDamage;
		string	LinkedSkill;
		int	LinkType;
		float	RemainingCDNotify;
		int	StrengthValue;
		int	UsageCount;
		vector<uint>	ExclusiveMask;
		vector<Sequence<float, 2>>	DistanceScale;
		string	BindSkill;

		void CopyFrom(RowData *pRowData)
		{
			this->SkillScript=pRowData->SkillScript;
			this->SkillLevel=pRowData->SkillLevel;
			this->ScriptName=pRowData->ScriptName;
			this->PhysicalRatio=pRowData->PhysicalRatio;
			this->PhysicalFixed=pRowData->PhysicalFixed;
			this->DefenceSlotReduce=pRowData->DefenceSlotReduce;
			this->AddBuffPoint=pRowData->AddBuffPoint;
			this->BuffID=pRowData->BuffID;
			this->HpMaxLimit=pRowData->HpMaxLimit;
			this->UnlockLevel=pRowData->UnlockLevel;
			this->LevelupCost=pRowData->LevelupCost;
			this->UpReqRoleLevel=pRowData->UpReqRoleLevel;
			this->CurrentLevelDescription=pRowData->CurrentLevelDescription;
			this->NextLevelDescription=pRowData->NextLevelDescription;
			this->Icon=pRowData->Icon;
			this->MagicRatio=pRowData->MagicRatio;
			this->MagicFixed=pRowData->MagicFixed;
			this->Element=pRowData->Element;
			this->Profession=pRowData->Profession;
			this->SkillType=pRowData->SkillType;
			this->IncreaseSuperArmor=pRowData->IncreaseSuperArmor;
			this->DecreaseSuperArmor=pRowData->DecreaseSuperArmor;
			this->IsBasicSkill=pRowData->IsBasicSkill;
			this->CostMP=pRowData->CostMP;
			this->TipsRatio=pRowData->TipsRatio;
			this->TipsFixed=pRowData->TipsFixed;
			this->PreSkill=pRowData->PreSkill;
			this->XPostion=pRowData->XPostion;
			this->YPostion=pRowData->YPostion;
			this->StartBuffID=pRowData->StartBuffID;
			this->PassiveAttribute=pRowData->PassiveAttribute;
			this->LevelupCostType=pRowData->LevelupCostType;
			this->UpReqGuildLevel=pRowData->UpReqGuildLevel;
			this->AuraBuffID=pRowData->AuraBuffID;
			this->HpMinLimit=pRowData->HpMinLimit;
			this->CDRatio=pRowData->CDRatio;
			this->PvPCDRatio=pRowData->PvPCDRatio;
			this->SuperArmorMax=pRowData->SuperArmorMax;
			this->SuperArmorMin=pRowData->SuperArmorMin;
			this->XEntityStatisticsID=pRowData->XEntityStatisticsID;
			this->PvPIncreaseSuperArmor=pRowData->PvPIncreaseSuperArmor;
			this->PvPDecreaseSuperArmor=pRowData->PvPDecreaseSuperArmor;
			this->PvPRatio=pRowData->PvPRatio;
			this->PvPFixed=pRowData->PvPFixed;
			this->ResetSkillTo=pRowData->ResetSkillTo;
			this->InitCD=pRowData->InitCD;
			this->PvPInitCD=pRowData->PvPInitCD;
			this->StudyLevelCost=pRowData->StudyLevelCost;
			this->PvPMagicRatio=pRowData->PvPMagicRatio;
			this->PvPMagicFixed=pRowData->PvPMagicFixed;
			this->PreviewScript=pRowData->PreviewScript;
			this->MobBuffs=pRowData->MobBuffs;
			this->Atlas=pRowData->Atlas;
			this->Flag=pRowData->Flag;
			this->PreSkillPoint=pRowData->PreSkillPoint;
			this->SuperIndureAttack=pRowData->SuperIndureAttack;
			this->SuperIndureDefense=pRowData->SuperIndureDefense;
			this->ExSkillScript=pRowData->ExSkillScript;
			this->UnchangableCD=pRowData->UnchangableCD;
			this->EnmityRatio=pRowData->EnmityRatio;
			this->EnmityExtValue=pRowData->EnmityExtValue;
			this->PercentDamage=pRowData->PercentDamage;
			this->LinkedSkill=pRowData->LinkedSkill;
			this->LinkType=pRowData->LinkType;
			this->RemainingCDNotify=pRowData->RemainingCDNotify;
			this->StrengthValue=pRowData->StrengthValue;
			this->UsageCount=pRowData->UsageCount;
			this->ExclusiveMask=pRowData->ExclusiveMask;
			this->DistanceScale=pRowData->DistanceScale;
			this->BindSkill=pRowData->BindSkill;

		}
	};

	std::unordered_map<string,int> SkillScript_Index;
	std::unordered_map<uint,int> XEntityStatisticsID_Index;


    RowData *GetBySkillScript(const string &key)
    {
        auto i = SkillScript_Index.find(key);
        return (i == SkillScript_Index.end()) ? NULL : Table[i->second];
    }
    
    RowData *GetByXEntityStatisticsID(const uint &key)
    {
        auto i = XEntityStatisticsID_Index.find(key);
        return (i == XEntityStatisticsID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SkillScript", "SkillLevel", "ScriptName", "PhysicalRatio", "PhysicalFixed", "DefenceSlotReduce", "AddBuffPoint", "BuffID", "HpMaxLimit", "UnlockLevel", "LevelupCost", "UpReqRoleLevel", "CurrentLevelDescription", "NextLevelDescription", "Icon", "MagicRatio", "MagicFixed", "Element", "Profession", "SkillType", "IncreaseSuperArmor", "DecreaseSuperArmor", "IsBasicSkill", "CostMP", "TipsRatio", "TipsFixed", "PreSkill", "XPostion", "YPostion", "StartBuffID", "PassiveAttribute", "LevelupCostType", "UpReqGuildLevel", "AuraBuffID", "HpMinLimit", "CDRatio", "PvPCDRatio", "SuperArmorMax", "SuperArmorMin", "XEntityStatisticsID", "PvPIncreaseSuperArmor", "PvPDecreaseSuperArmor", "PvPRatio", "PvPFixed", "ResetSkillTo", "InitCD", "PvPInitCD", "StudyLevelCost", "PvPMagicRatio", "PvPMagicFixed", "PreviewScript", "MobBuffs", "Atlas", "Flag", "PreSkillPoint", "SuperIndureAttack", "SuperIndureDefense", "ExSkillScript", "UnchangableCD", "EnmityRatio", "EnmityExtValue", "PercentDamage", "LinkedSkill", "LinkType", "RemainingCDNotify", "StrengthValue", "UsageCount", "ExclusiveMask", "DistanceScale", "BindSkill",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SkillList  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySkillScript(pSrcRow->SkillScript);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SkillScript_Index[pNewRow->SkillScript]=Table.size()-1;
                XEntityStatisticsID_Index[pNewRow->XEntityStatisticsID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SkillScript)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SkillLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ScriptName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PhysicalRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PhysicalFixed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.DefenceSlotReduce)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.AddBuffPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.BuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.HpMaxLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.UnlockLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.LevelupCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.UpReqRoleLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.CurrentLevelDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.NextLevelDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.MagicRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.MagicFixed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.Element)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.SkillType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.IncreaseSuperArmor)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.DecreaseSuperArmor)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.IsBasicSkill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.CostMP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.TipsRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.TipsFixed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.PreSkill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.XPostion)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.YPostion)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.StartBuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.PassiveAttribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.LevelupCostType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.UpReqGuildLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[33]], row.AuraBuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[34]], row.HpMinLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[35]], row.CDRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[36]], row.PvPCDRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[37]], row.SuperArmorMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[38]], row.SuperArmorMin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[39]], row.XEntityStatisticsID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[40]], row.PvPIncreaseSuperArmor)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[41]], row.PvPDecreaseSuperArmor)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[42]], row.PvPRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[43]], row.PvPFixed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[44]], row.ResetSkillTo)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[45]], row.InitCD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[46]], row.PvPInitCD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[47]], row.StudyLevelCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[48]], row.PvPMagicRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[49]], row.PvPMagicFixed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[50]], row.PreviewScript)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[51]], row.MobBuffs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[52]], row.Atlas)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[53]], row.Flag)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[54]], row.PreSkillPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[55]], row.SuperIndureAttack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[56]], row.SuperIndureDefense)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[57]], row.ExSkillScript)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[58]], row.UnchangableCD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[59]], row.EnmityRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[60]], row.EnmityExtValue)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[61]], row.PercentDamage)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[62]], row.LinkedSkill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[63]], row.LinkType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[64]], row.RemainingCDNotify)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[65]], row.StrengthValue)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[66]], row.UsageCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[67]], row.ExclusiveMask)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[68]], row.DistanceScale)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[69]], row.BindSkill)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SkillScript_Index[row.SkillScript] = Table.size()-1;
		XEntityStatisticsID_Index[row.XEntityStatisticsID] = Table.size()-1;


		return true;
	}

	~SkillList ()
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

		SkillScript_Index.clear();
		XEntityStatisticsID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif