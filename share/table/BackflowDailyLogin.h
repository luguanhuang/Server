#ifndef __BackflowDailyLogin_H__
#define __BackflowDailyLogin_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/3/20 20:43:28


class BackflowDailyLogin : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SealLevel;
		uint	NeedLoginDays;
		vector<Sequence<uint, 2>>	Rewards;

		void CopyFrom(RowData *pRowData)
		{
			this->SealLevel=pRowData->SealLevel;
			this->NeedLoginDays=pRowData->NeedLoginDays;
			this->Rewards=pRowData->Rewards;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SealLevel", "NeedLoginDays", "Rewards",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BackflowDailyLogin  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.SealLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.NeedLoginDays)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Rewards)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~BackflowDailyLogin ()
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