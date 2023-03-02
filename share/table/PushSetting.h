#ifndef __PushSetting_H__
#define __PushSetting_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PushSetting : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Type;
		string	ConfigName;
		uint	TimeOrSystem;
		string	Time;
		string	WeekDay;
		string	ConfigKey;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->ConfigName=pRowData->ConfigName;
			this->TimeOrSystem=pRowData->TimeOrSystem;
			this->Time=pRowData->Time;
			this->WeekDay=pRowData->WeekDay;
			this->ConfigKey=pRowData->ConfigKey;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "ConfigName", "TimeOrSystem", "Time", "WeekDay", "ConfigKey",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PushSetting  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.ConfigName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TimeOrSystem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.WeekDay)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ConfigKey)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PushSetting ()
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