#ifndef __TimeRewardTable_H__
#define __TimeRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class TimeRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	type;
		string	name;
		vector<Sequence<uint, 2>>	reward;
		vector<uint>	time;
		string	comment;
		uint	overdue;

		void CopyFrom(RowData *pRowData)
		{
			this->type=pRowData->type;
			this->name=pRowData->name;
			this->reward=pRowData->reward;
			this->time=pRowData->time;
			this->comment=pRowData->comment;
			this->overdue=pRowData->overdue;

		}
	};

	std::unordered_map<uint,int> type_Index;


    RowData *GetBytype(const uint &key)
    {
        auto i = type_Index.find(key);
        return (i == type_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "type", "name", "reward", "time", "comment", "overdue",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(TimeRewardTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBytype(pSrcRow->type);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                type_Index[pNewRow->type]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.comment)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.overdue)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		type_Index[row.type] = Table.size()-1;


		return true;
	}

	~TimeRewardTable ()
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

		type_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif