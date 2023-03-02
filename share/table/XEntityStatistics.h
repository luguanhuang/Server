#ifndef __XEntityStatistics_H__
#define __XEntityStatistics_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/25 14:25:10


class XEntityStatistics : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	Name;
		uint	PresentID;
		float	WalkSpeed;
		float	RunSpeed;
		vector<float>	FloatHeight;
		float	RotateSpeed;
		float	MaxHP;
		float	AttackProb;
		float	Sight;
		int	Type;
		uint	ID;
		vector<Sequence<int, 2>>	InBornBuff;
		float	AttackBase;
		bool	IsWander;
		bool	Block;
		float	AIStartTime;
		float	AIActionGap;
		uint	FashionTemplate;
		int	MaxSuperArmor;
		bool	IsFixedInCD;
		uint	HpSection;
		bool	EndShow;
		bool	Highlight;
		float	MagicAttackBase;
		int	UseMyMesh;
		Sequence<uint, 2>	ExtraReward;
		float	FightTogetherDis;
		int	aihit;
		double	SuperArmorRecoveryValue;
		double	SuperArmorRecoveryMax;
		Sequence<int, 2>	SuperArmorBrokenBuff;
		bool	WeakStatus;
		int	InitEnmity;
		bool	AlwaysHpBar;
		string	AiBehavior;
		int	Fightgroup;
		bool	SoloShow;
		bool	UsingGeneralCutscene;
		bool	HideName;
		float	AttackSpeed;
		float	ratioleft;
		float	ratioright;
		float	ratioidle;
		float	ratiodistance;
		float	ratioskill;
		float	ratioexp;
		float	SkillCD;
		bool	BeLocked;
		vector<Sequence<float, 4>>	navigation;
		int	IsNavPingpong;
		bool	HideInMiniMap;
		uint	Fov;
		vector<Sequence<uint, 2>>	Access;
		int	SummonGroup;
		bool	SameBillBoard;
		vector<uint>	PandoraDropIDs;
		vector<uint>	DropIDs;
		bool	DynamicBlock;
		vector<uint>	Tag;
		double	MobaExp;
		double	MobaExpRange;
		bool	MobaKillNeedHint;
		uint	PosIndex;
		uint	BigMeleePoints;
		vector<uint>	HellDrop;
		int	LinkCombo;

		void CopyFrom(RowData *pRowData)
		{
			this->Name=pRowData->Name;
			this->PresentID=pRowData->PresentID;
			this->WalkSpeed=pRowData->WalkSpeed;
			this->RunSpeed=pRowData->RunSpeed;
			this->FloatHeight=pRowData->FloatHeight;
			this->RotateSpeed=pRowData->RotateSpeed;
			this->MaxHP=pRowData->MaxHP;
			this->AttackProb=pRowData->AttackProb;
			this->Sight=pRowData->Sight;
			this->Type=pRowData->Type;
			this->ID=pRowData->ID;
			this->InBornBuff=pRowData->InBornBuff;
			this->AttackBase=pRowData->AttackBase;
			this->IsWander=pRowData->IsWander;
			this->Block=pRowData->Block;
			this->AIStartTime=pRowData->AIStartTime;
			this->AIActionGap=pRowData->AIActionGap;
			this->FashionTemplate=pRowData->FashionTemplate;
			this->MaxSuperArmor=pRowData->MaxSuperArmor;
			this->IsFixedInCD=pRowData->IsFixedInCD;
			this->HpSection=pRowData->HpSection;
			this->EndShow=pRowData->EndShow;
			this->Highlight=pRowData->Highlight;
			this->MagicAttackBase=pRowData->MagicAttackBase;
			this->UseMyMesh=pRowData->UseMyMesh;
			this->ExtraReward=pRowData->ExtraReward;
			this->FightTogetherDis=pRowData->FightTogetherDis;
			this->aihit=pRowData->aihit;
			this->SuperArmorRecoveryValue=pRowData->SuperArmorRecoveryValue;
			this->SuperArmorRecoveryMax=pRowData->SuperArmorRecoveryMax;
			this->SuperArmorBrokenBuff=pRowData->SuperArmorBrokenBuff;
			this->WeakStatus=pRowData->WeakStatus;
			this->InitEnmity=pRowData->InitEnmity;
			this->AlwaysHpBar=pRowData->AlwaysHpBar;
			this->AiBehavior=pRowData->AiBehavior;
			this->Fightgroup=pRowData->Fightgroup;
			this->SoloShow=pRowData->SoloShow;
			this->UsingGeneralCutscene=pRowData->UsingGeneralCutscene;
			this->HideName=pRowData->HideName;
			this->AttackSpeed=pRowData->AttackSpeed;
			this->ratioleft=pRowData->ratioleft;
			this->ratioright=pRowData->ratioright;
			this->ratioidle=pRowData->ratioidle;
			this->ratiodistance=pRowData->ratiodistance;
			this->ratioskill=pRowData->ratioskill;
			this->ratioexp=pRowData->ratioexp;
			this->SkillCD=pRowData->SkillCD;
			this->BeLocked=pRowData->BeLocked;
			this->navigation=pRowData->navigation;
			this->IsNavPingpong=pRowData->IsNavPingpong;
			this->HideInMiniMap=pRowData->HideInMiniMap;
			this->Fov=pRowData->Fov;
			this->Access=pRowData->Access;
			this->SummonGroup=pRowData->SummonGroup;
			this->SameBillBoard=pRowData->SameBillBoard;
			this->PandoraDropIDs=pRowData->PandoraDropIDs;
			this->DropIDs=pRowData->DropIDs;
			this->DynamicBlock=pRowData->DynamicBlock;
			this->Tag=pRowData->Tag;
			this->MobaExp=pRowData->MobaExp;
			this->MobaExpRange=pRowData->MobaExpRange;
			this->MobaKillNeedHint=pRowData->MobaKillNeedHint;
			this->PosIndex=pRowData->PosIndex;
			this->BigMeleePoints=pRowData->BigMeleePoints;
			this->HellDrop=pRowData->HellDrop;
			this->LinkCombo=pRowData->LinkCombo;

		}
	};

	std::unordered_map<uint,int> ID_Index;


    RowData *GetByID(const uint &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Name", "PresentID", "WalkSpeed", "RunSpeed", "FloatHeight", "RotateSpeed", "MaxHP", "AttackProb", "Sight", "Type", "ID", "InBornBuff", "AttackBase", "IsWander", "Block", "AIStartTime", "AIActionGap", "FashionTemplate", "MaxSuperArmor", "IsFixedInCD", "HpSection", "EndShow", "Highlight", "MagicAttackBase", "UseMyMesh", "ExtraReward", "FightTogetherDis", "aihit", "SuperArmorRecoveryValue", "SuperArmorRecoveryMax", "SuperArmorBrokenBuff", "WeakStatus", "InitEnmity", "AlwaysHpBar", "AiBehavior", "Fightgroup", "SoloShow", "UsingGeneralCutscene", "HideName", "AttackSpeed", "ratioleft", "ratioright", "ratioidle", "ratiodistance", "ratioskill", "ratioexp", "SkillCD", "BeLocked", "navigation", "IsNavPingpong", "HideInMiniMap", "Fov", "Access", "SummonGroup", "SameBillBoard", "PandoraDropIDs", "DropIDs", "DynamicBlock", "Tag", "MobaExp", "MobaExpRange", "MobaKillNeedHint", "PosIndex", "BigMeleePoints", "HellDrop", "LinkCombo",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(XEntityStatistics  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByID(pSrcRow->ID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ID_Index[pNewRow->ID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.PresentID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.WalkSpeed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.RunSpeed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.FloatHeight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.RotateSpeed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MaxHP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.AttackProb)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Sight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.InBornBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.AttackBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.IsWander)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.Block)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.AIStartTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.AIActionGap)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.FashionTemplate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.MaxSuperArmor)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.IsFixedInCD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.HpSection)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.EndShow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.Highlight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.MagicAttackBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.UseMyMesh)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.ExtraReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.FightTogetherDis)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.aihit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.SuperArmorRecoveryValue)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.SuperArmorRecoveryMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.SuperArmorBrokenBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.WeakStatus)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.InitEnmity)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[33]], row.AlwaysHpBar)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[34]], row.AiBehavior)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[35]], row.Fightgroup)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[36]], row.SoloShow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[37]], row.UsingGeneralCutscene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[38]], row.HideName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[39]], row.AttackSpeed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[40]], row.ratioleft)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[41]], row.ratioright)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[42]], row.ratioidle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[43]], row.ratiodistance)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[44]], row.ratioskill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[45]], row.ratioexp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[46]], row.SkillCD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[47]], row.BeLocked)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[48]], row.navigation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[49]], row.IsNavPingpong)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[50]], row.HideInMiniMap)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[51]], row.Fov)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[52]], row.Access)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[53]], row.SummonGroup)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[54]], row.SameBillBoard)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[55]], row.PandoraDropIDs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[56]], row.DropIDs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[57]], row.DynamicBlock)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[58]], row.Tag)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[59]], row.MobaExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[60]], row.MobaExpRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[61]], row.MobaKillNeedHint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[62]], row.PosIndex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[63]], row.BigMeleePoints)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[64]], row.HellDrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[65]], row.LinkCombo)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~XEntityStatistics ()
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

		ID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif