#ifndef __JadeSlotTable_H__
#define __JadeSlotTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/16 21:26:46


class JadeSlotTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EquipSlot;
		vector<Sequence<uint, 2>>	JadeSlotAndLevel;

		void CopyFrom(RowData *pRowData)
		{
			this->EquipSlot=pRowData->EquipSlot;
			this->JadeSlotAndLevel=pRowData->JadeSlotAndLevel;

		}
	};

	std::unordered_map<uint,int> EquipSlot_Index;


    RowData *GetByEquipSlot(const uint &key)
    {
        auto i = EquipSlot_Index.find(key);
        return (i == EquipSlot_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EquipSlot", "JadeSlotAndLevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(JadeSlotTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByEquipSlot(pSrcRow->EquipSlot);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                EquipSlot_Index[pNewRow->EquipSlot]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EquipSlot)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.JadeSlotAndLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		EquipSlot_Index[row.EquipSlot] = Table.size()-1;


		return true;
	}

	~JadeSlotTable ()
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

		EquipSlot_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif