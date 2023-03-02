﻿#ifndef __FirstPassTable_H__
#define __FirstPassTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 21:30:43


class FirstPassTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		vector<int>	SceneID;
		int	RewardID;
		vector<Sequence<int, 3>>	CommendReward;
		int	SystemId;
		string	Des;
		string	BgTexName;
		uint	DNId;
		uint	NestType;
		string	RankTittle;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->SceneID=pRowData->SceneID;
			this->RewardID=pRowData->RewardID;
			this->CommendReward=pRowData->CommendReward;
			this->SystemId=pRowData->SystemId;
			this->Des=pRowData->Des;
			this->BgTexName=pRowData->BgTexName;
			this->DNId=pRowData->DNId;
			this->NestType=pRowData->NestType;
			this->RankTittle=pRowData->RankTittle;

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
		const char *Headers[] = { "ID", "SceneID", "RewardID", "CommendReward", "SystemId", "Des", "BgTexName", "DNId", "NestType", "RankTittle",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FirstPassTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RewardID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.CommendReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SystemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Des)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.BgTexName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.DNId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.NestType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.RankTittle)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~FirstPassTable ()
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