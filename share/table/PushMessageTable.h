#ifndef __PushMessageTable_H__
#define __PushMessageTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PushMessageTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Type;
		uint	SubType;
		vector<uint>	TimeOffset;
		string	Title;
		string	Content;
		uint	IsCommonGlobal;
		vector<uint>	Time;
		uint	Date;
		vector<uint>	WeekDay;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->SubType=pRowData->SubType;
			this->TimeOffset=pRowData->TimeOffset;
			this->Title=pRowData->Title;
			this->Content=pRowData->Content;
			this->IsCommonGlobal=pRowData->IsCommonGlobal;
			this->Time=pRowData->Time;
			this->Date=pRowData->Date;
			this->WeekDay=pRowData->WeekDay;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "SubType", "TimeOffset", "Title", "Content", "IsCommonGlobal", "Time", "Date", "WeekDay",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PushMessageTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SubType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TimeOffset)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Title)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Content)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.IsCommonGlobal)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Date)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.WeekDay)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PushMessageTable ()
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