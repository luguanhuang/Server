#ifndef __GuildAuctReward_H__
#define __GuildAuctReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/16 20:57:21


class GuildAuctReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		string	Comment;
		int	ActType;
		Sequence<int, 2>	Rank;
		Sequence<int, 2>	WorldLevel;
		int	BonusType;
		vector<int>	DropID;
		vector<uint>	RewardShow;
		string	ActName;
		int	Flow;
		vector<Sequence<uint, 2>>	SItem;
		vector<Sequence<float, 3>>	SItemPro;
		uint	DropNeedNum;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Comment=pRowData->Comment;
			this->ActType=pRowData->ActType;
			this->Rank=pRowData->Rank;
			this->WorldLevel=pRowData->WorldLevel;
			this->BonusType=pRowData->BonusType;
			this->DropID=pRowData->DropID;
			this->RewardShow=pRowData->RewardShow;
			this->ActName=pRowData->ActName;
			this->Flow=pRowData->Flow;
			this->SItem=pRowData->SItem;
			this->SItemPro=pRowData->SItemPro;
			this->DropNeedNum=pRowData->DropNeedNum;

		}
	};

	std::unordered_map<int,int> ID_Index;


    RowData *GetByID(const int &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Comment", "ActType", "Rank", "WorldLevel", "BonusType", "DropID", "RewardShow", "ActName", "Flow", "SItem", "SItemPro", "DropNeedNum",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildAuctReward  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Comment)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ActType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.WorldLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BonusType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.DropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.RewardShow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ActName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Flow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.SItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.SItemPro)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.DropNeedNum)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~GuildAuctReward ()
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