#ifndef __AttributeEmblem_H__
#define __AttributeEmblem_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/13 18:04:15


class AttributeEmblem : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EmblemID;
		uint	Position;
		uint	AttrID;
		uint	Probability;
		Sequence<uint, 2>	InitialRange;
		Sequence<uint, 2>	Range;
		vector<Sequence<int, 3>>	SmeltAttr;
		vector<Sequence<int, 3>>	SmeltAttr2;
		vector<Sequence<int, 3>>	SmeltAttr3;
		uint	SmeltExpectation;

		void CopyFrom(RowData *pRowData)
		{
			this->EmblemID=pRowData->EmblemID;
			this->Position=pRowData->Position;
			this->AttrID=pRowData->AttrID;
			this->Probability=pRowData->Probability;
			this->InitialRange=pRowData->InitialRange;
			this->Range=pRowData->Range;
			this->SmeltAttr=pRowData->SmeltAttr;
			this->SmeltAttr2=pRowData->SmeltAttr2;
			this->SmeltAttr3=pRowData->SmeltAttr3;
			this->SmeltExpectation=pRowData->SmeltExpectation;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EmblemID", "Position", "AttrID", "Probability", "InitialRange", "Range", "SmeltAttr", "SmeltAttr2", "SmeltAttr3", "SmeltExpectation",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AttributeEmblem  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.EmblemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Position)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AttrID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Probability)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.InitialRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Range)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SmeltAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SmeltAttr2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.SmeltAttr3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.SmeltExpectation)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~AttributeEmblem ()
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