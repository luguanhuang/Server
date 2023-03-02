#ifndef __MobaKillExp_H__
#define __MobaKillExp_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/8 14:59:47


class MobaKillExp : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	AssistNum;
		uint	RangeNum;
		Sequence<double, 2>	KillerExp;
		Sequence<double, 2>	AssistExp;
		Sequence<double, 2>	RangeExp;

		void CopyFrom(RowData *pRowData)
		{
			this->AssistNum=pRowData->AssistNum;
			this->RangeNum=pRowData->RangeNum;
			this->KillerExp=pRowData->KillerExp;
			this->AssistExp=pRowData->AssistExp;
			this->RangeExp=pRowData->RangeExp;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "AssistNum", "RangeNum", "KillerExp", "AssistExp", "RangeExp",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(MobaKillExp  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.AssistNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.RangeNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.KillerExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AssistExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.RangeExp)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~MobaKillExp ()
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