#ifndef __ForgeAttributes_H__
#define __ForgeAttributes_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/8 21:12:00


class ForgeAttributes : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EquipID;
		uint	Slot;
		uint	AttrID;
		uint	Prob;
		Sequence<uint, 2>	InitialRange;
		Sequence<uint, 2>	Range;
		vector<Sequence<int, 3>>	SmeltAttr1;
		vector<Sequence<int, 3>>	SmeltAttr2;
		vector<Sequence<int, 3>>	SmeltAttr3;
		uint	SmeltExpectation;
		int	CanSmelt;

		void CopyFrom(RowData *pRowData)
		{
			this->EquipID=pRowData->EquipID;
			this->Slot=pRowData->Slot;
			this->AttrID=pRowData->AttrID;
			this->Prob=pRowData->Prob;
			this->InitialRange=pRowData->InitialRange;
			this->Range=pRowData->Range;
			this->SmeltAttr1=pRowData->SmeltAttr1;
			this->SmeltAttr2=pRowData->SmeltAttr2;
			this->SmeltAttr3=pRowData->SmeltAttr3;
			this->SmeltExpectation=pRowData->SmeltExpectation;
			this->CanSmelt=pRowData->CanSmelt;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EquipID", "Slot", "AttrID", "Prob", "InitialRange", "Range", "SmeltAttr1", "SmeltAttr2", "SmeltAttr3", "SmeltExpectation", "CanSmelt",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ForgeAttributes  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[3]], row.Prob)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.InitialRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Range)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SmeltAttr1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SmeltAttr2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.SmeltAttr3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.SmeltExpectation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.CanSmelt)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ForgeAttributes ()
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