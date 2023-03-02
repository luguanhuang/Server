#ifndef __FashionComposeTable_H__
#define __FashionComposeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FashionComposeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	FashionID;
		int	FashionLevel;
		Sequence<uint, 2>	SrcItem1;
		Sequence<uint, 2>	SrcItem2;
		Sequence<uint, 2>	SrcItem3;
		int	ComposeCost;
		vector<Sequence<uint, 2>>	Attributes;
		vector<Sequence<uint, 2>>	Decompose;
		int	RequirLevel;

		void CopyFrom(RowData *pRowData)
		{
			this->FashionID=pRowData->FashionID;
			this->FashionLevel=pRowData->FashionLevel;
			this->SrcItem1=pRowData->SrcItem1;
			this->SrcItem2=pRowData->SrcItem2;
			this->SrcItem3=pRowData->SrcItem3;
			this->ComposeCost=pRowData->ComposeCost;
			this->Attributes=pRowData->Attributes;
			this->Decompose=pRowData->Decompose;
			this->RequirLevel=pRowData->RequirLevel;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "FashionID", "FashionLevel", "SrcItem1", "SrcItem2", "SrcItem3", "ComposeCost", "Attributes", "Decompose", "RequirLevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FashionComposeTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.FashionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.FashionLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SrcItem1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SrcItem2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SrcItem3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ComposeCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Attributes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Decompose)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.RequirLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~FashionComposeTable ()
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