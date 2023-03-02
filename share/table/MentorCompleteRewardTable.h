#ifndef __MentorCompleteRewardTable_H__
#define __MentorCompleteRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class MentorCompleteRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Type;
		vector<Sequence<int, 2>>	MasterReward;
		vector<Sequence<int, 2>>	StudentReward;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->MasterReward=pRowData->MasterReward;
			this->StudentReward=pRowData->StudentReward;

		}
	};

	std::unordered_map<int,int> Type_Index;


    RowData *GetByType(const int &key)
    {
        auto i = Type_Index.find(key);
        return (i == Type_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "MasterReward", "StudentReward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(MentorCompleteRewardTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.MasterReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.StudentReward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Type_Index[row.Type] = Table.size()-1;


		return true;
	}

	~MentorCompleteRewardTable ()
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