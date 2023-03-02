#ifndef __EffectTypeTable_H__
#define __EffectTypeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/6 11:41:25


class EffectTypeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EffectType;
		bool	CanOverlap;

		void CopyFrom(RowData *pRowData)
		{
			this->EffectType=pRowData->EffectType;
			this->CanOverlap=pRowData->CanOverlap;

		}
	};

	std::unordered_map<uint,int> EffectType_Index;


    RowData *GetByEffectType(const uint &key)
    {
        auto i = EffectType_Index.find(key);
        return (i == EffectType_Index.end()) ? NULL : Table[i->second];
    }


	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EffectType", "CanOverlap",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EffectTypeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByEffectType(pSrcRow->EffectType);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                EffectType_Index[pNewRow->EffectType]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EffectType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.CanOverlap)) return false; ++m_ColNum;


		Table.push_back(pRow);

		EffectType_Index[row.EffectType] = Table.size()-1;


		return true;
	}

	~EffectTypeTable ()
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

		EffectType_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif