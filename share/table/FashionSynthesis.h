#ifndef __FashionSynthesis_H__
#define __FashionSynthesis_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/27 10:22:31


class FashionSynthesis : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	FashionID;
		uint	SuccessRate;
		vector<Sequence<uint, 2>>	ReturnItems;
		uint	SuitID;
		uint	FashinSynthesisAddSucessRate;

		void CopyFrom(RowData *pRowData)
		{
			this->FashionID=pRowData->FashionID;
			this->SuccessRate=pRowData->SuccessRate;
			this->ReturnItems=pRowData->ReturnItems;
			this->SuitID=pRowData->SuitID;
			this->FashinSynthesisAddSucessRate=pRowData->FashinSynthesisAddSucessRate;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "FashionID", "SuccessRate", "ReturnItems", "SuitID", "FashinSynthesisAddSucessRate",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FashionSynthesis  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.SuccessRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ReturnItems)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SuitID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.FashinSynthesisAddSucessRate)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~FashionSynthesis ()
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