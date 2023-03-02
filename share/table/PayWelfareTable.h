#ifndef __PayWelfareTable_H__
#define __PayWelfareTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PayWelfareTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Order;
		int	SysID;
		string	TabName;
		string	TabIcon;

		void CopyFrom(RowData *pRowData)
		{
			this->Order=pRowData->Order;
			this->SysID=pRowData->SysID;
			this->TabName=pRowData->TabName;
			this->TabIcon=pRowData->TabIcon;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Order", "SysID", "TabName", "TabIcon",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PayWelfareTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Order)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SysID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TabName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.TabIcon)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PayWelfareTable ()
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