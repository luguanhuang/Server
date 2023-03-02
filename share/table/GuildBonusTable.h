#ifndef __GuildBonusTable_H__
#define __GuildBonusTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/13 20:31:59


class GuildBonusTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	GuildBonusID;
		uint	GuildBonusType;
		string	GuildBonusName;
		string	GuildBonusDesc;
		uint	GuildBonusMaxPeopleNum;
		bool	GuildBonusIsRandomAccess;
		uint	GuildBonusOpenTime;
		Sequence<uint, 2>	GuildBonusReward;
		uint	GuildBonusVar;
		bool	GuildBonusIsManualSend;
		string	TriggerTime;
		int	DayMaxNum;
		int	ForeverMaxNum;

		void CopyFrom(RowData *pRowData)
		{
			this->GuildBonusID=pRowData->GuildBonusID;
			this->GuildBonusType=pRowData->GuildBonusType;
			this->GuildBonusName=pRowData->GuildBonusName;
			this->GuildBonusDesc=pRowData->GuildBonusDesc;
			this->GuildBonusMaxPeopleNum=pRowData->GuildBonusMaxPeopleNum;
			this->GuildBonusIsRandomAccess=pRowData->GuildBonusIsRandomAccess;
			this->GuildBonusOpenTime=pRowData->GuildBonusOpenTime;
			this->GuildBonusReward=pRowData->GuildBonusReward;
			this->GuildBonusVar=pRowData->GuildBonusVar;
			this->GuildBonusIsManualSend=pRowData->GuildBonusIsManualSend;
			this->TriggerTime=pRowData->TriggerTime;
			this->DayMaxNum=pRowData->DayMaxNum;
			this->ForeverMaxNum=pRowData->ForeverMaxNum;

		}
	};

	std::unordered_map<uint,int> GuildBonusID_Index;


    RowData *GetByGuildBonusID(const uint &key)
    {
        auto i = GuildBonusID_Index.find(key);
        return (i == GuildBonusID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GuildBonusID", "GuildBonusType", "GuildBonusName", "GuildBonusDesc", "GuildBonusMaxPeopleNum", "GuildBonusIsRandomAccess", "GuildBonusOpenTime", "GuildBonusReward", "GuildBonusVar", "GuildBonusIsManualSend", "TriggerTime", "DayMaxNum", "ForeverMaxNum",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildBonusTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByGuildBonusID(pSrcRow->GuildBonusID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                GuildBonusID_Index[pNewRow->GuildBonusID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.GuildBonusID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.GuildBonusType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.GuildBonusName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.GuildBonusDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.GuildBonusMaxPeopleNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.GuildBonusIsRandomAccess)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.GuildBonusOpenTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.GuildBonusReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.GuildBonusVar)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.GuildBonusIsManualSend)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.TriggerTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.DayMaxNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.ForeverMaxNum)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		GuildBonusID_Index[row.GuildBonusID] = Table.size()-1;


		return true;
	}

	~GuildBonusTable ()
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

		GuildBonusID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif