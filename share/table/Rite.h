#ifndef __Rite_H__
#define __Rite_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class Rite : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		uint	starttime;
		uint	endtime;
		uint	cd;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->starttime=pRowData->starttime;
			this->endtime=pRowData->endtime;
			this->cd=pRowData->cd;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "starttime", "endtime", "cd",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(Rite  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.starttime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.endtime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.cd)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~Rite ()
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