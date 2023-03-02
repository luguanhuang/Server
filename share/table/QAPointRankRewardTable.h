#ifndef __QAPointRankRewardTable_H__
#define __QAPointRankRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/9 10:48:46


class QAPointRankRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		vector<Sequence<uint, 2>>	Reward;
		string	Title;
		string	Content;
		uint	RANK;
		uint	QAType;
		uint	MinLevel;
		uint	MaxLevel;
		uint	LevelSeal;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Reward=pRowData->Reward;
			this->Title=pRowData->Title;
			this->Content=pRowData->Content;
			this->RANK=pRowData->RANK;
			this->QAType=pRowData->QAType;
			this->MinLevel=pRowData->MinLevel;
			this->MaxLevel=pRowData->MaxLevel;
			this->LevelSeal=pRowData->LevelSeal;

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
		const char *Headers[] = { "ID", "Reward", "Title", "Content", "RANK", "QAType", "MinLevel", "MaxLevel", "LevelSeal",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(QAPointRankRewardTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Title)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Content)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.RANK)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.QAType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MinLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.MaxLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.LevelSeal)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~QAPointRankRewardTable ()
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