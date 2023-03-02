#ifndef __LevelSealTypeTable_H__
#define __LevelSealTypeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/22 16:03:27


class LevelSealTypeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Type;
		uint	Level;
		uint	Time;
		vector<uint>	UnlockBossID;
		string	UnlockBossName;
		uint	UnlockBossCount;
		string	NowSealImage;
		string	NextSealImageL;
		string	NextSealImageR;
		string	NextSealImageBig;
		string	MailTitle;
		string	MailContent;
		vector<Sequence<uint, 2>>	Award;
		Sequence<uint, 2>	ExchangeInfo;
		vector<Sequence<uint, 2>>	CollectAward;
		vector<Sequence<string, 2>>	CollectAwardMailText;
		vector<Sequence<uint, 2>>	PlayerAward;
		Sequence<string, 2>	PlayerAwardText;
		uint	ClearFindBackMinLevel;
		int	ApplyStudentLevel;
		int	ApplyMasterPromptLevel;
		Sequence<int, 2>	ApplyMasterMaxLevel;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->Level=pRowData->Level;
			this->Time=pRowData->Time;
			this->UnlockBossID=pRowData->UnlockBossID;
			this->UnlockBossName=pRowData->UnlockBossName;
			this->UnlockBossCount=pRowData->UnlockBossCount;
			this->NowSealImage=pRowData->NowSealImage;
			this->NextSealImageL=pRowData->NextSealImageL;
			this->NextSealImageR=pRowData->NextSealImageR;
			this->NextSealImageBig=pRowData->NextSealImageBig;
			this->MailTitle=pRowData->MailTitle;
			this->MailContent=pRowData->MailContent;
			this->Award=pRowData->Award;
			this->ExchangeInfo=pRowData->ExchangeInfo;
			this->CollectAward=pRowData->CollectAward;
			this->CollectAwardMailText=pRowData->CollectAwardMailText;
			this->PlayerAward=pRowData->PlayerAward;
			this->PlayerAwardText=pRowData->PlayerAwardText;
			this->ClearFindBackMinLevel=pRowData->ClearFindBackMinLevel;
			this->ApplyStudentLevel=pRowData->ApplyStudentLevel;
			this->ApplyMasterPromptLevel=pRowData->ApplyMasterPromptLevel;
			this->ApplyMasterMaxLevel=pRowData->ApplyMasterMaxLevel;

		}
	};

	std::unordered_map<uint,int> Type_Index;


    RowData *GetByType(const uint &key)
    {
        auto i = Type_Index.find(key);
        return (i == Type_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "Level", "Time", "UnlockBossID", "UnlockBossName", "UnLockBossCount", "NowSealImage", "NextSealImageL", "NextSealImageR", "NextSealImageBig", "MailTitle", "MailContent", "Award", "ExchangeInfo", "CollectAward", "CollectAwardMailText", "PlayerAward", "PlayerAwardText", "ClearFindBackMinLevel", "ApplyStudentLevel", "ApplyMasterPromptLevel", "ApplyMasterMaxLevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(LevelSealTypeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByType(pSrcRow->Type);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Type_Index[pNewRow->Type]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.UnlockBossID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.UnlockBossName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.UnlockBossCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.NowSealImage)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.NextSealImageL)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.NextSealImageR)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.NextSealImageBig)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.MailTitle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.MailContent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Award)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.ExchangeInfo)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.CollectAward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.CollectAwardMailText)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.PlayerAward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.PlayerAwardText)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.ClearFindBackMinLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.ApplyStudentLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.ApplyMasterPromptLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.ApplyMasterMaxLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Type_Index[row.Type] = Table.size()-1;


		return true;
	}

	~LevelSealTypeTable ()
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

		Type_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif