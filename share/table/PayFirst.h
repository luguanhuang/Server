#ifndef __PayFirst_H__
#define __PayFirst_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PayFirst : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Money;
		int	Award;
		int	SystemID;

		void CopyFrom(RowData *pRowData)
		{
			this->Money=pRowData->Money;
			this->Award=pRowData->Award;
			this->SystemID=pRowData->SystemID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Money", "Award", "SystemID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PayFirst  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Money)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Award)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SystemID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PayFirst ()
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