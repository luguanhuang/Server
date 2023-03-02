#ifndef __ExpeditionTable_H__
#define __ExpeditionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/28 15:19:09


class ExpeditionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	DNExpeditionID;
		string	DNExpeditionName;
		vector<uint>	ViewableDropList;
		int	RequiredLevel;
		int	PlayerNumber;
		vector<uint>	RandomSceneIDs;
		int	GuildLevel;
		int	Type;
		vector<Sequence<int, 2>>	OpenTime;
		int	PlayerLeastNumber;
		int	Category;
		double	RobotLifePercent;
		uint	DisplayLevel;
		uint	DisplayPPT;
		uint	RobotLookupID;
		int	fastmatch;
		int	FMARobotTime;
		vector<Sequence<int, 2>>	CostItem;
		uint	LevelSealType;
		int	CanHelp;
		bool	AutoRefuse;
		int	AutoSelectPriority;
		double	RobotAtkPercent;
		vector<Sequence<uint, 3>>	CostType;
		int	CostCountType;
		uint	PreReadyScene;
		uint	LevelSealTeamCost;
		Sequence<uint, 2>	ServerOpenTime;
		uint	KickLeaderTime;
		int	SortID;
		Sequence<uint, 2>	UseTicket;
		Sequence<uint, 2>	Stars;
		bool	isCrossServerInvite;
		uint	MaxCount;
		int	MatchType;
		vector<Sequence<uint, 3>>	BackServerOpenTime;
		bool	isCross;
		vector<int>	CrossCanSee;
		bool	ShowPPT;

		void CopyFrom(RowData *pRowData)
		{
			this->DNExpeditionID=pRowData->DNExpeditionID;
			this->DNExpeditionName=pRowData->DNExpeditionName;
			this->ViewableDropList=pRowData->ViewableDropList;
			this->RequiredLevel=pRowData->RequiredLevel;
			this->PlayerNumber=pRowData->PlayerNumber;
			this->RandomSceneIDs=pRowData->RandomSceneIDs;
			this->GuildLevel=pRowData->GuildLevel;
			this->Type=pRowData->Type;
			this->OpenTime=pRowData->OpenTime;
			this->PlayerLeastNumber=pRowData->PlayerLeastNumber;
			this->Category=pRowData->Category;
			this->RobotLifePercent=pRowData->RobotLifePercent;
			this->DisplayLevel=pRowData->DisplayLevel;
			this->DisplayPPT=pRowData->DisplayPPT;
			this->RobotLookupID=pRowData->RobotLookupID;
			this->fastmatch=pRowData->fastmatch;
			this->FMARobotTime=pRowData->FMARobotTime;
			this->CostItem=pRowData->CostItem;
			this->LevelSealType=pRowData->LevelSealType;
			this->CanHelp=pRowData->CanHelp;
			this->AutoRefuse=pRowData->AutoRefuse;
			this->AutoSelectPriority=pRowData->AutoSelectPriority;
			this->RobotAtkPercent=pRowData->RobotAtkPercent;
			this->CostType=pRowData->CostType;
			this->CostCountType=pRowData->CostCountType;
			this->PreReadyScene=pRowData->PreReadyScene;
			this->LevelSealTeamCost=pRowData->LevelSealTeamCost;
			this->ServerOpenTime=pRowData->ServerOpenTime;
			this->KickLeaderTime=pRowData->KickLeaderTime;
			this->SortID=pRowData->SortID;
			this->UseTicket=pRowData->UseTicket;
			this->Stars=pRowData->Stars;
			this->isCrossServerInvite=pRowData->isCrossServerInvite;
			this->MaxCount=pRowData->MaxCount;
			this->MatchType=pRowData->MatchType;
			this->BackServerOpenTime=pRowData->BackServerOpenTime;
			this->isCross=pRowData->isCross;
			this->CrossCanSee=pRowData->CrossCanSee;
			this->ShowPPT=pRowData->ShowPPT;

		}
	};

	std::unordered_map<int,int> DNExpeditionID_Index;


    RowData *GetByDNExpeditionID(const int &key)
    {
        auto i = DNExpeditionID_Index.find(key);
        return (i == DNExpeditionID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "DNExpeditionID", "DNExpeditionName", "ViewableDropList", "RequiredLevel", "PlayerNumber", "RandomSceneIDs", "GuildLevel", "Type", "OpenTime", "PlayerLeastNumber", "Category", "RobotLifePercent", "DisplayLevel", "DisplayPPT", "RobotLookupID", "fastmatch", "FMARobotTime", "CostItem", "LevelSealType", "CanHelp", "AutoRefuse", "AutoSelectPriority", "RobotAtkPercent", "CostType", "CostCountType", "PreReadyScene", "LevelSealTeamCost", "ServerOpenTime", "KickLeaderTime", "SortID", "UseTicket", "Stars", "isCrossServerInvite", "MaxCount", "MatchType", "BackServerOpenTime", "isCross", "CrossCanSee", "ShowPPT",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ExpeditionTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByDNExpeditionID(pSrcRow->DNExpeditionID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                DNExpeditionID_Index[pNewRow->DNExpeditionID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.DNExpeditionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.DNExpeditionName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ViewableDropList)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.RequiredLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PlayerNumber)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.RandomSceneIDs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.GuildLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.OpenTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.PlayerLeastNumber)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Category)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.RobotLifePercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.DisplayLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.DisplayPPT)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.RobotLookupID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.fastmatch)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.FMARobotTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.CostItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.LevelSealType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.CanHelp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.AutoRefuse)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.AutoSelectPriority)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.RobotAtkPercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.CostType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.CostCountType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.PreReadyScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.LevelSealTeamCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.ServerOpenTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.KickLeaderTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.SortID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.UseTicket)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.Stars)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.isCrossServerInvite)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[33]], row.MaxCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[34]], row.MatchType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[35]], row.BackServerOpenTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[36]], row.isCross)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[37]], row.CrossCanSee)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[38]], row.ShowPPT)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		DNExpeditionID_Index[row.DNExpeditionID] = Table.size()-1;


		return true;
	}

	~ExpeditionTable ()
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

		DNExpeditionID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif