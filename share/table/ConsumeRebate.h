#ifndef __ConsumeRebate_H__
#define __ConsumeRebate_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/30 11:14:11


class ConsumeRebate : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	datetype;
		uint	startdate;
		uint	duration;
		uint	rebaterate;

		void CopyFrom(RowData *pRowData)
		{
			this->datetype=pRowData->datetype;
			this->startdate=pRowData->startdate;
			this->duration=pRowData->duration;
			this->rebaterate=pRowData->rebaterate;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "datetype", "startdate", "duration", "rebaterate",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ConsumeRebate  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.datetype)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.startdate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.rebaterate)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ConsumeRebate ()
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