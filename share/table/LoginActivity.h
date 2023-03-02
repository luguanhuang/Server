#ifndef __LoginActivity_H__
#define __LoginActivity_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/26 16:19:56


class LoginActivity : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	Time;
		Sequence<uint, 2>	WorldLevel;
		vector<Sequence<uint, 2>>	Award;

		void CopyFrom(RowData *pRowData)
		{
			this->Time=pRowData->Time;
			this->WorldLevel=pRowData->WorldLevel;
			this->Award=pRowData->Award;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Time", "WorldLevel", "Award",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(LoginActivity  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.WorldLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Award)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~LoginActivity ()
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