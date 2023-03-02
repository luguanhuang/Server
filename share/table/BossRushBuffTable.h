#ifndef __BossRushBuffTable_H__
#define __BossRushBuffTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class BossRushBuffTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	BossRushBuffID;
		Sequence<int, 2>	AttrBuff;
		float	RewardBuff;
		string	ratestring;
		string	icon;
		int	Quality;
		string	Comment;
		int	type;

		void CopyFrom(RowData *pRowData)
		{
			this->BossRushBuffID=pRowData->BossRushBuffID;
			this->AttrBuff=pRowData->AttrBuff;
			this->RewardBuff=pRowData->RewardBuff;
			this->ratestring=pRowData->ratestring;
			this->icon=pRowData->icon;
			this->Quality=pRowData->Quality;
			this->Comment=pRowData->Comment;
			this->type=pRowData->type;

		}
	};

	std::unordered_map<int,int> BossRushBuffID_Index;


    RowData *GetByBossRushBuffID(const int &key)
    {
        auto i = BossRushBuffID_Index.find(key);
        return (i == BossRushBuffID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BossRushBuffID", "AttrBuff", "RewardBuff", "ratestring", "icon", "Quality", "Comment", "type",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BossRushBuffTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByBossRushBuffID(pSrcRow->BossRushBuffID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                BossRushBuffID_Index[pNewRow->BossRushBuffID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.BossRushBuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.AttrBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RewardBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ratestring)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Comment)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.type)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		BossRushBuffID_Index[row.BossRushBuffID] = Table.size()-1;


		return true;
	}

	~BossRushBuffTable ()
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

		BossRushBuffID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif