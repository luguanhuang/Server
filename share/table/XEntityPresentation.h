#ifndef __XEntityPresentation_H__
#define __XEntityPresentation_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/16 16:00:11


class XEntityPresentation : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	Prefab;
		string	A;
		string	AA;
		string	AAA;
		string	AAAA;
		string	AAAAA;
		string	Ultra;
		vector<string>	OtherSkills;
		vector<string>	Hit_f;
		vector<string>	HitFly;
		string	Idle;
		string	Walk;
		string	Run;
		string	Death;
		string	Appear;
		vector<string>	Hit_l;
		vector<string>	Hit_r;
		uint	PresentID;
		vector<string>	HitCurves;
		string	AnimLocation;
		string	SkillLocation;
		string	CurveLocation;
		string	Avatar;
		vector<string>	DeathCurve;
		vector<float>	HitBackOffsetTimeScale;
		vector<float>	HitFlyOffsetTimeScale;
		float	Scale;
		string	BoneSuffix;
		string	Disappear;
		bool	Shadow;
		string	Dash;
		string	Freeze;
		vector<float>	HitBack_Recover;
		vector<float>	HitFly_Bounce_GetUp;
		float	BoundRadius;
		float	BoundHeight;
		string	AttackIdle;
		string	AttackWalk;
		string	AttackRun;
		string	EnterGame;
		vector<string>	Hit_Roll;
		vector<float>	HitRollOffsetTimeScale;
		float	HitRoll_Recover;
		bool	Huge;
		string	MoveFx;
		string	FreezeFx;
		string	HitFx;
		string	DeathFx;
		string	SuperArmorRecoverySkill;
		string	Feeble;
		string	FeebleFx;
		string	ArmorBroken;
		string	RecoveryFX;
		string	RecoveryHitSlowFX;
		string	RecoveryHitStopFX;
		string	FishingIdle;
		string	FishingCast;
		string	FishingPull;
		vector<float>	BoundRadiusOffset;
		vector<Sequence<float, 4>>	HugeMonsterColliders;
		float	UIAvatarScale;
		float	UIAvatarAngle;
		string	FishingWait;
		string	FishingWin;
		string	FishingLose;
		string	Dance;
		vector<string>	AvatarPos;
		string	InheritActionOne;
		string	InheritActionTwo;
		string	Kiss;
		string	RunLeft;
		string	RunRight;
		string	AttackRunLeft;
		string	AttackRunRight;
		string	Atlas;
		int	AngluarSpeed;
		int	SkillNum;
		string	Atlas2;
		string	Avatar2;

		void CopyFrom(RowData *pRowData)
		{
			this->Prefab=pRowData->Prefab;
			this->A=pRowData->A;
			this->AA=pRowData->AA;
			this->AAA=pRowData->AAA;
			this->AAAA=pRowData->AAAA;
			this->AAAAA=pRowData->AAAAA;
			this->Ultra=pRowData->Ultra;
			this->OtherSkills=pRowData->OtherSkills;
			this->Hit_f=pRowData->Hit_f;
			this->HitFly=pRowData->HitFly;
			this->Idle=pRowData->Idle;
			this->Walk=pRowData->Walk;
			this->Run=pRowData->Run;
			this->Death=pRowData->Death;
			this->Appear=pRowData->Appear;
			this->Hit_l=pRowData->Hit_l;
			this->Hit_r=pRowData->Hit_r;
			this->PresentID=pRowData->PresentID;
			this->HitCurves=pRowData->HitCurves;
			this->AnimLocation=pRowData->AnimLocation;
			this->SkillLocation=pRowData->SkillLocation;
			this->CurveLocation=pRowData->CurveLocation;
			this->Avatar=pRowData->Avatar;
			this->DeathCurve=pRowData->DeathCurve;
			this->HitBackOffsetTimeScale=pRowData->HitBackOffsetTimeScale;
			this->HitFlyOffsetTimeScale=pRowData->HitFlyOffsetTimeScale;
			this->Scale=pRowData->Scale;
			this->BoneSuffix=pRowData->BoneSuffix;
			this->Disappear=pRowData->Disappear;
			this->Shadow=pRowData->Shadow;
			this->Dash=pRowData->Dash;
			this->Freeze=pRowData->Freeze;
			this->HitBack_Recover=pRowData->HitBack_Recover;
			this->HitFly_Bounce_GetUp=pRowData->HitFly_Bounce_GetUp;
			this->BoundRadius=pRowData->BoundRadius;
			this->BoundHeight=pRowData->BoundHeight;
			this->AttackIdle=pRowData->AttackIdle;
			this->AttackWalk=pRowData->AttackWalk;
			this->AttackRun=pRowData->AttackRun;
			this->EnterGame=pRowData->EnterGame;
			this->Hit_Roll=pRowData->Hit_Roll;
			this->HitRollOffsetTimeScale=pRowData->HitRollOffsetTimeScale;
			this->HitRoll_Recover=pRowData->HitRoll_Recover;
			this->Huge=pRowData->Huge;
			this->MoveFx=pRowData->MoveFx;
			this->FreezeFx=pRowData->FreezeFx;
			this->HitFx=pRowData->HitFx;
			this->DeathFx=pRowData->DeathFx;
			this->SuperArmorRecoverySkill=pRowData->SuperArmorRecoverySkill;
			this->Feeble=pRowData->Feeble;
			this->FeebleFx=pRowData->FeebleFx;
			this->ArmorBroken=pRowData->ArmorBroken;
			this->RecoveryFX=pRowData->RecoveryFX;
			this->RecoveryHitSlowFX=pRowData->RecoveryHitSlowFX;
			this->RecoveryHitStopFX=pRowData->RecoveryHitStopFX;
			this->FishingIdle=pRowData->FishingIdle;
			this->FishingCast=pRowData->FishingCast;
			this->FishingPull=pRowData->FishingPull;
			this->BoundRadiusOffset=pRowData->BoundRadiusOffset;
			this->HugeMonsterColliders=pRowData->HugeMonsterColliders;
			this->UIAvatarScale=pRowData->UIAvatarScale;
			this->UIAvatarAngle=pRowData->UIAvatarAngle;
			this->FishingWait=pRowData->FishingWait;
			this->FishingWin=pRowData->FishingWin;
			this->FishingLose=pRowData->FishingLose;
			this->Dance=pRowData->Dance;
			this->AvatarPos=pRowData->AvatarPos;
			this->InheritActionOne=pRowData->InheritActionOne;
			this->InheritActionTwo=pRowData->InheritActionTwo;
			this->Kiss=pRowData->Kiss;
			this->RunLeft=pRowData->RunLeft;
			this->RunRight=pRowData->RunRight;
			this->AttackRunLeft=pRowData->AttackRunLeft;
			this->AttackRunRight=pRowData->AttackRunRight;
			this->Atlas=pRowData->Atlas;
			this->AngluarSpeed=pRowData->AngluarSpeed;
			this->SkillNum=pRowData->SkillNum;
			this->Atlas2=pRowData->Atlas2;
			this->Avatar2=pRowData->Avatar2;

		}
	};

	std::unordered_map<uint,int> PresentID_Index;


    RowData *GetByPresentID(const uint &key)
    {
        auto i = PresentID_Index.find(key);
        return (i == PresentID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Prefab", "A", "AA", "AAA", "AAAA", "AAAAA", "Ultra", "OtherSkills", "Hit_f", "HitFly", "Idle", "Walk", "Run", "Death", "Appear", "Hit_l", "Hit_r", "PresentID", "HitCurves", "AnimLocation", "SkillLocation", "CurveLocation", "Avatar", "DeathCurve", "HitBackOffsetTimeScale", "HitFlyOffsetTimeScale", "Scale", "BoneSuffix", "Disappear", "Shadow", "Dash", "Freeze", "HitBack_Recover", "HitFly_Bounce_GetUp", "BoundRadius", "BoundHeight", "AttackIdle", "AttackWalk", "AttackRun", "EnterGame", "Hit_Roll", "HitRollOffsetTimeScale", "HitRoll_Recover", "Huge", "MoveFx", "FreezeFx", "HitFx", "DeathFx", "SuperArmorRecoverySkill", "Feeble", "FeebleFx", "ArmorBroken", "RecoveryFX", "RecoveryHitSlowFX", "RecoveryHitStopFX", "FishingIdle", "FishingCast", "FishingPull", "BoundRadiusOffset", "HugeMonsterColliders", "UIAvatarScale", "UIAvatarAngle", "FishingWait", "FishingWin", "FishingLose", "Dance", "AvatarPos", "InheritActionOne", "InheritActionTwo", "Kiss", "RunLeft", "RunRight", "AttackRunLeft", "AttackRunRight", "Atlas", "AngluarSpeed", "SkillNum", "Atlas2", "Avatar2",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(XEntityPresentation  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByPresentID(pSrcRow->PresentID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                PresentID_Index[pNewRow->PresentID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Prefab)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.A)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AA)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AAA)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.AAAA)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.AAAAA)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Ultra)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.OtherSkills)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Hit_f)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.HitFly)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Idle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Walk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Run)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.Death)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.Appear)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.Hit_l)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.Hit_r)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.PresentID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.HitCurves)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.AnimLocation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.SkillLocation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.CurveLocation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.Avatar)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.DeathCurve)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.HitBackOffsetTimeScale)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.HitFlyOffsetTimeScale)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.Scale)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.BoneSuffix)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.Disappear)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.Shadow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.Dash)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.Freeze)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.HitBack_Recover)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[33]], row.HitFly_Bounce_GetUp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[34]], row.BoundRadius)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[35]], row.BoundHeight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[36]], row.AttackIdle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[37]], row.AttackWalk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[38]], row.AttackRun)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[39]], row.EnterGame)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[40]], row.Hit_Roll)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[41]], row.HitRollOffsetTimeScale)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[42]], row.HitRoll_Recover)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[43]], row.Huge)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[44]], row.MoveFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[45]], row.FreezeFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[46]], row.HitFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[47]], row.DeathFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[48]], row.SuperArmorRecoverySkill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[49]], row.Feeble)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[50]], row.FeebleFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[51]], row.ArmorBroken)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[52]], row.RecoveryFX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[53]], row.RecoveryHitSlowFX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[54]], row.RecoveryHitStopFX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[55]], row.FishingIdle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[56]], row.FishingCast)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[57]], row.FishingPull)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[58]], row.BoundRadiusOffset)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[59]], row.HugeMonsterColliders)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[60]], row.UIAvatarScale)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[61]], row.UIAvatarAngle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[62]], row.FishingWait)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[63]], row.FishingWin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[64]], row.FishingLose)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[65]], row.Dance)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[66]], row.AvatarPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[67]], row.InheritActionOne)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[68]], row.InheritActionTwo)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[69]], row.Kiss)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[70]], row.RunLeft)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[71]], row.RunRight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[72]], row.AttackRunLeft)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[73]], row.AttackRunRight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[74]], row.Atlas)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[75]], row.AngluarSpeed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[76]], row.SkillNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[77]], row.Atlas2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[78]], row.Avatar2)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		PresentID_Index[row.PresentID] = Table.size()-1;


		return true;
	}

	~XEntityPresentation ()
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

		PresentID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif