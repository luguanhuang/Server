#ifndef __GuildMineralBufflist_H__
#define __GuildMineralBufflist_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildMineralBufflist : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	BuffID;
		Sequence<uint, 2>	AttrBuff;
		uint	RewardBuff;
		string	ratestring;
		uint	type;
		string	icon;
		uint	Quality;

		void CopyFrom(RowData *pRowData)
		{
			this->BuffID=pRowData->BuffID;
			this->AttrBuff=pRowData->AttrBuff;
			this->RewardBuff=pRowData->RewardBuff;
			this->ratestring=pRowData->ratestring;
			this->type=pRowData->type;
			this->icon=pRowData->icon;
			this->Quality=pRowData->Quality;

		}
	};

	std::unordered_map<uint,int> BuffID_Index;


    RowData *GetByBuffID(const uint &key)
    {
        auto i = BuffID_Index.find(key);
        return (i == BuffID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BuffID", "AttrBuff", "RewardBuff", "ratestring", "type", "icon", "Quality",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildMineralBufflist  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByBuffID(pSrcRow->BuffID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                BuffID_Index[pNewRow->BuffID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.BuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.AttrBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RewardBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ratestring)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Quality)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		BuffID_Index[row.BuffID] = Table.size()-1;


		return true;
	}

	~GuildMineralBufflist ()
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

		BuffID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif