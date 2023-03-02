#ifndef __PrefixAttributesTable_H__
#define __PrefixAttributesTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PrefixAttributesTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EquipID;
		uint	Slot;
		uint	AttrID;
		vector<Sequence<uint, 3>>	AttrRange;
		uint	Prob;

		void CopyFrom(RowData *pRowData)
		{
			this->EquipID=pRowData->EquipID;
			this->Slot=pRowData->Slot;
			this->AttrID=pRowData->AttrID;
			this->AttrRange=pRowData->AttrRange;
			this->Prob=pRowData->Prob;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EquipID", "Slot", "AttrID", "AttrRange", "Prob",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PrefixAttributesTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EquipID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Slot)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AttrID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AttrRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Prob)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PrefixAttributesTable ()
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


	}

	std::vector<RowData*> Table;
};

#endif