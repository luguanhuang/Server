#ifndef __SceneTable_H__
#define __SceneTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/13 14:27:29


class SceneTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	id;
		int	type;
		string	configFile;
		vector<Sequence<float, 3>>	StartPos;
		vector<float>	StartRot;
		string	sceneFile;
		int	syncMode;
		string	BlockFilePath;
		vector<int>	UIPos;
		int	Exp;
		int	Money;
		bool	IsBoss;
		int	Chapter;
		string	Comment;
		int	RecommendPower;
		vector<int>	Drop1;
		vector<int>	Drop2;
		vector<int>	Drop3;
		vector<int>	Drop4;
		vector<int>	Drop5;
		int	RequiredLevel;
		string	UIIcon;
		int	FirstDownExp;
		vector<int>	FirstDownDrop;
		int	FirstDownMoney;
		vector<int>	ViewableDropList;
		string	BossAvatar;
		vector<Sequence<int, 2>>	FatigueCost;
		string	EndCutScene;
		float	EndCutSceneTime;
		uint	GoldDropID;
		uint	SilverDropID;
		uint	CopperDropID;
		vector<Sequence<int, 2>>	WinCondition;
		vector<Sequence<int, 2>>	LoseCondition;
		int	DayLimit;
		bool	CanDrawBox;
		bool	HasFlyOut;
		uint	DayLimitGroupID;
		string	DynamicScene;
		bool	IsStaticScene;
		Sequence<uint, 2>	ReviveTime;
		bool	CanPause;
		vector<int>	OperationSettings;
		string	BossIcon;
		int	CoolDown;
		string	BGM;
		bool	ShowUp;
		vector<Sequence<uint, 2>>	FirstSSS;
		vector<int>	PreScene;
		int	SceneChest;
		vector<int>	BoxUIPos;
		int	LineRoleCount;
		vector<string>	LoadingTips;
		vector<string>	LoadingPic;
		bool	SceneCanNavi;
		int	RandomTaskType;
		vector<int>	RandomTaskSpecify;
		bool	UseSupplement;
		float	HurtCoef;
		string	MiniMap;
		vector<int>	MiniMapSize;
		int	MiniMapRotation;
		int	PreTask;
		bool	SwitchToSelf;
		string	SceneAI;
		bool	ShowAutoFight;
		float	PPTCoff;
		float	GuildExpBounus;
		string	FailText;
		int	WinDelayTime;
		string	RecommendHint;
		uint	TeamInfoDefaultTab;
		uint	CombatType;
		int	SweepNeedPPT;
		uint	ReviveNumb;
		vector<Sequence<uint, 2>>	ReviveCost;
		bool	CanRevive;
		uint	DiamondDropID;
		vector<Sequence<uint, 2>>	SBox;
		vector<Sequence<uint, 2>>	SSBox;
		vector<Sequence<uint, 2>>	SSSBox;
		vector<int>	TimeCounter;
		bool	HasComboBuff;
		bool	DisplayPet;
		int	AutoReturn;
		uint	StoryDriver;
		uint	MinPassTime;
		vector<Sequence<uint, 2>>	ReviveMoneyCost;
		vector<Sequence<uint, 2>>	ReviveBuff;
		string	LeaveSceneTip;
		string	ReviveBuffTip;
		vector<Sequence<uint, 2>>	ExpSealReward;
		bool	ShowSkill;
		string	WinConditionTips;
		float	DelayTransfer;
		vector<Sequence<uint, 2>>	Buff;
		Sequence<double, 2>	DPS;
		vector<float>	StartFace;
		bool	IsCanQuit;
		uint	CanVIPRevive;
		bool	ShowNormalAttack;
		bool	HideTeamIndicate;
		string	BattleExplainTips;
		vector<int>	ShieldSight;
		string	ScenePath;
		vector<Sequence<int, 3>>	EnvSet;
		float	SpecifiedTargetLocatedRange;
		vector<int>	SpactivityActiveDrop;
		vector<Sequence<uint, 2>>	SpActivityDrop;
		vector<float>	StaticMiniMapCenter;
		uint	VipReviveLimit;
		vector<float>	MiniMapOutSize;
		bool	CanReconnect;
		uint	EndTimeOut;
		bool	ShowBattleStatistics;
		Sequence<uint, 2>	PeerBox;
		Sequence<uint, 2>	SelectBoxTime;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->type=pRowData->type;
			this->configFile=pRowData->configFile;
			this->StartPos=pRowData->StartPos;
			this->StartRot=pRowData->StartRot;
			this->sceneFile=pRowData->sceneFile;
			this->syncMode=pRowData->syncMode;
			this->BlockFilePath=pRowData->BlockFilePath;
			this->UIPos=pRowData->UIPos;
			this->Exp=pRowData->Exp;
			this->Money=pRowData->Money;
			this->IsBoss=pRowData->IsBoss;
			this->Chapter=pRowData->Chapter;
			this->Comment=pRowData->Comment;
			this->RecommendPower=pRowData->RecommendPower;
			this->Drop1=pRowData->Drop1;
			this->Drop2=pRowData->Drop2;
			this->Drop3=pRowData->Drop3;
			this->Drop4=pRowData->Drop4;
			this->Drop5=pRowData->Drop5;
			this->RequiredLevel=pRowData->RequiredLevel;
			this->UIIcon=pRowData->UIIcon;
			this->FirstDownExp=pRowData->FirstDownExp;
			this->FirstDownDrop=pRowData->FirstDownDrop;
			this->FirstDownMoney=pRowData->FirstDownMoney;
			this->ViewableDropList=pRowData->ViewableDropList;
			this->BossAvatar=pRowData->BossAvatar;
			this->FatigueCost=pRowData->FatigueCost;
			this->EndCutScene=pRowData->EndCutScene;
			this->EndCutSceneTime=pRowData->EndCutSceneTime;
			this->GoldDropID=pRowData->GoldDropID;
			this->SilverDropID=pRowData->SilverDropID;
			this->CopperDropID=pRowData->CopperDropID;
			this->WinCondition=pRowData->WinCondition;
			this->LoseCondition=pRowData->LoseCondition;
			this->DayLimit=pRowData->DayLimit;
			this->CanDrawBox=pRowData->CanDrawBox;
			this->HasFlyOut=pRowData->HasFlyOut;
			this->DayLimitGroupID=pRowData->DayLimitGroupID;
			this->DynamicScene=pRowData->DynamicScene;
			this->IsStaticScene=pRowData->IsStaticScene;
			this->ReviveTime=pRowData->ReviveTime;
			this->CanPause=pRowData->CanPause;
			this->OperationSettings=pRowData->OperationSettings;
			this->BossIcon=pRowData->BossIcon;
			this->CoolDown=pRowData->CoolDown;
			this->BGM=pRowData->BGM;
			this->ShowUp=pRowData->ShowUp;
			this->FirstSSS=pRowData->FirstSSS;
			this->PreScene=pRowData->PreScene;
			this->SceneChest=pRowData->SceneChest;
			this->BoxUIPos=pRowData->BoxUIPos;
			this->LineRoleCount=pRowData->LineRoleCount;
			this->LoadingTips=pRowData->LoadingTips;
			this->LoadingPic=pRowData->LoadingPic;
			this->SceneCanNavi=pRowData->SceneCanNavi;
			this->RandomTaskType=pRowData->RandomTaskType;
			this->RandomTaskSpecify=pRowData->RandomTaskSpecify;
			this->UseSupplement=pRowData->UseSupplement;
			this->HurtCoef=pRowData->HurtCoef;
			this->MiniMap=pRowData->MiniMap;
			this->MiniMapSize=pRowData->MiniMapSize;
			this->MiniMapRotation=pRowData->MiniMapRotation;
			this->PreTask=pRowData->PreTask;
			this->SwitchToSelf=pRowData->SwitchToSelf;
			this->SceneAI=pRowData->SceneAI;
			this->ShowAutoFight=pRowData->ShowAutoFight;
			this->PPTCoff=pRowData->PPTCoff;
			this->GuildExpBounus=pRowData->GuildExpBounus;
			this->FailText=pRowData->FailText;
			this->WinDelayTime=pRowData->WinDelayTime;
			this->RecommendHint=pRowData->RecommendHint;
			this->TeamInfoDefaultTab=pRowData->TeamInfoDefaultTab;
			this->CombatType=pRowData->CombatType;
			this->SweepNeedPPT=pRowData->SweepNeedPPT;
			this->ReviveNumb=pRowData->ReviveNumb;
			this->ReviveCost=pRowData->ReviveCost;
			this->CanRevive=pRowData->CanRevive;
			this->DiamondDropID=pRowData->DiamondDropID;
			this->SBox=pRowData->SBox;
			this->SSBox=pRowData->SSBox;
			this->SSSBox=pRowData->SSSBox;
			this->TimeCounter=pRowData->TimeCounter;
			this->HasComboBuff=pRowData->HasComboBuff;
			this->DisplayPet=pRowData->DisplayPet;
			this->AutoReturn=pRowData->AutoReturn;
			this->StoryDriver=pRowData->StoryDriver;
			this->MinPassTime=pRowData->MinPassTime;
			this->ReviveMoneyCost=pRowData->ReviveMoneyCost;
			this->ReviveBuff=pRowData->ReviveBuff;
			this->LeaveSceneTip=pRowData->LeaveSceneTip;
			this->ReviveBuffTip=pRowData->ReviveBuffTip;
			this->ExpSealReward=pRowData->ExpSealReward;
			this->ShowSkill=pRowData->ShowSkill;
			this->WinConditionTips=pRowData->WinConditionTips;
			this->DelayTransfer=pRowData->DelayTransfer;
			this->Buff=pRowData->Buff;
			this->DPS=pRowData->DPS;
			this->StartFace=pRowData->StartFace;
			this->IsCanQuit=pRowData->IsCanQuit;
			this->CanVIPRevive=pRowData->CanVIPRevive;
			this->ShowNormalAttack=pRowData->ShowNormalAttack;
			this->HideTeamIndicate=pRowData->HideTeamIndicate;
			this->BattleExplainTips=pRowData->BattleExplainTips;
			this->ShieldSight=pRowData->ShieldSight;
			this->ScenePath=pRowData->ScenePath;
			this->EnvSet=pRowData->EnvSet;
			this->SpecifiedTargetLocatedRange=pRowData->SpecifiedTargetLocatedRange;
			this->SpactivityActiveDrop=pRowData->SpactivityActiveDrop;
			this->SpActivityDrop=pRowData->SpActivityDrop;
			this->StaticMiniMapCenter=pRowData->StaticMiniMapCenter;
			this->VipReviveLimit=pRowData->VipReviveLimit;
			this->MiniMapOutSize=pRowData->MiniMapOutSize;
			this->CanReconnect=pRowData->CanReconnect;
			this->EndTimeOut=pRowData->EndTimeOut;
			this->ShowBattleStatistics=pRowData->ShowBattleStatistics;
			this->PeerBox=pRowData->PeerBox;
			this->SelectBoxTime=pRowData->SelectBoxTime;

		}
	};

	std::unordered_map<int,int> id_Index;


    RowData *GetBySceneID(const int &key)
    {
        auto i = id_Index.find(key);
        return (i == id_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneID", "SceneType", "SceneFile", "StartPos", "StartRot", "UnitySceneFile", "SyncMode", "BlockFilePath", "UIPos", "Exp", "Money", "IsBoss", "Chapter", "Comment", "RecommendPower", "Drop1", "Drop2", "Drop3", "Drop4", "Drop5", "RequiredLevel", "UIIcon", "FirstDownExp", "FirstDownDrop", "FirstDownMoney", "ViewableDropList", "BossAvatar", "FatigueCost", "EndCutScene", "EndCutSceneTime", "GoldDropID", "SilverDropID", "CopperDropID", "WinCondition", "LoseCondition", "DayLimit", "CanDrawBox", "HasFlyOut", "DayLimitGroupID", "DynamicScene", "IsStaticScene", "ReviveTime", "CanPause", "OperationSettings", "BossIcon", "CoolDown", "BGM", "ShowUp", "FirstSSS", "PreScene", "SceneChest", "BoxUIPos", "LineRoleCount", "LoadingTips", "LoadingPic", "SceneCanNavi", "RandomTaskType", "RandomTaskSpecify", "UseSupplement", "HurtCoef", "MiniMap", "MiniMapSize", "MiniMapRotation", "PreTask", "SwitchToSelf", "SceneAI", "ShowAutoFight", "PPTCoff", "GuildExpBounus", "FailText", "WinDelayTime", "RecommendHint", "TeamInfoDefaultTab", "CombatType", "SweepNeedPPT", "ReviveNumb", "ReviveCost", "CanRevive", "DiamondDropID", "SBox", "SSBox", "SSSBox", "TimeCounter", "HasComboBuff", "DisplayPet", "AutoReturn", "StoryDriver", "MinPassTime", "ReviveMoneyCost", "ReviveBuff", "LeaveSceneTip", "ReviveBuffTip", "ExpSealReward", "ShowSkill", "WinConditionTips", "DelayTransfer", "Buff", "DPS", "StartFace", "IsCanQuit", "CanVIPRevive", "ShowNormalAttack", "HideTeamIndicate", "BattleExplainTips", "ShieldSight", "ScenePath", "EnvSet", "SpecifiedTargetLocatedRange", "SpactivityActiveDrop", "SpActivityDrop", "StaticMiniMapCenter", "VipReviveLimit", "MiniMapOutSize", "CanReconnect", "EndTimeOut", "ShowBattleStatistics", "PeerBox", "SelectBoxTime",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SceneTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySceneID(pSrcRow->id);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                id_Index[pNewRow->id]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.configFile)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.StartPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.StartRot)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.sceneFile)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.syncMode)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.BlockFilePath)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.UIPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Exp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Money)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.IsBoss)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Chapter)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.Comment)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.RecommendPower)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.Drop1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.Drop2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.Drop3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.Drop4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.Drop5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.RequiredLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.UIIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.FirstDownExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.FirstDownDrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.FirstDownMoney)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.ViewableDropList)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.BossAvatar)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.FatigueCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.EndCutScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.EndCutSceneTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.GoldDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.SilverDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.CopperDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[33]], row.WinCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[34]], row.LoseCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[35]], row.DayLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[36]], row.CanDrawBox)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[37]], row.HasFlyOut)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[38]], row.DayLimitGroupID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[39]], row.DynamicScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[40]], row.IsStaticScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[41]], row.ReviveTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[42]], row.CanPause)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[43]], row.OperationSettings)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[44]], row.BossIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[45]], row.CoolDown)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[46]], row.BGM)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[47]], row.ShowUp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[48]], row.FirstSSS)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[49]], row.PreScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[50]], row.SceneChest)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[51]], row.BoxUIPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[52]], row.LineRoleCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[53]], row.LoadingTips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[54]], row.LoadingPic)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[55]], row.SceneCanNavi)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[56]], row.RandomTaskType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[57]], row.RandomTaskSpecify)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[58]], row.UseSupplement)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[59]], row.HurtCoef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[60]], row.MiniMap)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[61]], row.MiniMapSize)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[62]], row.MiniMapRotation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[63]], row.PreTask)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[64]], row.SwitchToSelf)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[65]], row.SceneAI)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[66]], row.ShowAutoFight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[67]], row.PPTCoff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[68]], row.GuildExpBounus)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[69]], row.FailText)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[70]], row.WinDelayTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[71]], row.RecommendHint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[72]], row.TeamInfoDefaultTab)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[73]], row.CombatType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[74]], row.SweepNeedPPT)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[75]], row.ReviveNumb)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[76]], row.ReviveCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[77]], row.CanRevive)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[78]], row.DiamondDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[79]], row.SBox)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[80]], row.SSBox)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[81]], row.SSSBox)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[82]], row.TimeCounter)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[83]], row.HasComboBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[84]], row.DisplayPet)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[85]], row.AutoReturn)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[86]], row.StoryDriver)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[87]], row.MinPassTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[88]], row.ReviveMoneyCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[89]], row.ReviveBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[90]], row.LeaveSceneTip)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[91]], row.ReviveBuffTip)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[92]], row.ExpSealReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[93]], row.ShowSkill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[94]], row.WinConditionTips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[95]], row.DelayTransfer)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[96]], row.Buff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[97]], row.DPS)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[98]], row.StartFace)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[99]], row.IsCanQuit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[100]], row.CanVIPRevive)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[101]], row.ShowNormalAttack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[102]], row.HideTeamIndicate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[103]], row.BattleExplainTips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[104]], row.ShieldSight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[105]], row.ScenePath)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[106]], row.EnvSet)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[107]], row.SpecifiedTargetLocatedRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[108]], row.SpactivityActiveDrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[109]], row.SpActivityDrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[110]], row.StaticMiniMapCenter)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[111]], row.VipReviveLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[112]], row.MiniMapOutSize)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[113]], row.CanReconnect)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[114]], row.EndTimeOut)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[115]], row.ShowBattleStatistics)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[116]], row.PeerBox)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[117]], row.SelectBoxTime)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~SceneTable ()
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

		id_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif