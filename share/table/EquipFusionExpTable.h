#ifndef __EquipFusionExpTable_H__
#define __EquipFusionExpTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/19 16:26:03


class EquipFusionExpTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	CoreItemId;
		vector<Sequence<uint, 2>>	AssistItemId;
		uint	AddExp;

		void CopyFrom(RowData *pRowData)
		{
			this->CoreItemId=pRowData->CoreItemId;
			this->AssistItemId=pRowData->AssistItemId;
			this->AddExp=pRowData->AddExp;

		}
	};

	std::unordered_map<uint,int> CoreItemId_Index;


    RowData *GetByCoreItemId(const uint &key)
    {
        auto i = CoreItemId_Index.find(key);
        return (i == CoreItemId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "CoreItemId", "AssistItemId", "AddExp",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EquipFusionExpTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByCoreItemId(pSrcRow->CoreItemId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                CoreItemId_Index[pNewRow->CoreItemId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.CoreItemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.AssistItemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AddExp)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		CoreItemId_Index[row.CoreItemId] = Table.size()-1;


		return true;
	}

	~EquipFusionExpTable ()
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

		CoreItemId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif