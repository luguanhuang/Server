#ifndef __OnlineRewardTable_H__
#define __OnlineRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class OnlineRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	time;
		vector<Sequence<uint, 2>>	reward;
		string	RewardTip;

		void CopyFrom(RowData *pRowData)
		{
			this->time=pRowData->time;
			this->reward=pRowData->reward;
			this->RewardTip=pRowData->RewardTip;

		}
	};

	std::unordered_map<uint,int> time_Index;


    RowData *GetBytime(const uint &key)
    {
        auto i = time_Index.find(key);
        return (i == time_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "time", "reward", "RewardTip",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(OnlineRewardTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBytime(pSrcRow->time);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                time_Index[pNewRow->time]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RewardTip)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		time_Index[row.time] = Table.size()-1;


		return true;
	}

	~OnlineRewardTable ()
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

		time_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif