#ifndef __BossTable_H__
#define __BossTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class BossTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	BossID;
		vector<Sequence<uint, 2>>	EnemyID;
		float	AttackPercent;
		float	LifePercent;

		void CopyFrom(RowData *pRowData)
		{
			this->BossID=pRowData->BossID;
			this->EnemyID=pRowData->EnemyID;
			this->AttackPercent=pRowData->AttackPercent;
			this->LifePercent=pRowData->LifePercent;

		}
	};

	std::unordered_map<uint,int> BossID_Index;


    RowData *GetByBossID(const uint &key)
    {
        auto i = BossID_Index.find(key);
        return (i == BossID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BossID", "EnemyID", "AttackPercent", "LifePercent",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BossTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByBossID(pSrcRow->BossID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                BossID_Index[pNewRow->BossID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.BossID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.EnemyID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AttackPercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.LifePercent)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		BossID_Index[row.BossID] = Table.size()-1;


		return true;
	}

	~BossTable ()
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

		BossID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif