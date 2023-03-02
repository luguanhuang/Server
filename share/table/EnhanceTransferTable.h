#ifndef __EnhanceTransferTable_H__
#define __EnhanceTransferTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class EnhanceTransferTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	originlevel;
		Sequence<uint, 2>	destlevel;
		uint	enhancelevel;
		vector<Sequence<uint, 2>>	percent;
		vector<Sequence<uint, 2>>	item;
		uint	dragoncoin;
		int	tipsID;

		void CopyFrom(RowData *pRowData)
		{
			this->originlevel=pRowData->originlevel;
			this->destlevel=pRowData->destlevel;
			this->enhancelevel=pRowData->enhancelevel;
			this->percent=pRowData->percent;
			this->item=pRowData->item;
			this->dragoncoin=pRowData->dragoncoin;
			this->tipsID=pRowData->tipsID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "originlevel", "destlevel", "enhancelevel", "percent", "item", "dragoncoin", "tipsID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EnhanceTransferTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.originlevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.destlevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.enhancelevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.percent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.item)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.dragoncoin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.tipsID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EnhanceTransferTable ()
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