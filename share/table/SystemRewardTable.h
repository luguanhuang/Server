#ifndef __SystemRewardTable_H__
#define __SystemRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SystemRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Type;
		string	Name;
		uint	SubType;
		uint	Sort;
		string	Remark;
		vector<Sequence<uint, 2>>	Reward;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->Name=pRowData->Name;
			this->SubType=pRowData->SubType;
			this->Sort=pRowData->Sort;
			this->Remark=pRowData->Remark;
			this->Reward=pRowData->Reward;

		}
	};

	std::unordered_map<uint,int> Type_Index;


    RowData *GetByType(const uint &key)
    {
        auto i = Type_Index.find(key);
        return (i == Type_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "Name", "SubType", "Sort", "Remark", "Reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SystemRewardTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SubType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Sort)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Remark)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Type_Index[row.Type] = Table.size()-1;


		return true;
	}

	~SystemRewardTable ()
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