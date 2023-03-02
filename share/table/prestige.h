#ifndef __prestige_H__
#define __prestige_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class prestige : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	BossID;
		Sequence<uint, 2>	rank;
		vector<Sequence<uint, 2>>	reward;
		string	prestige;

		void CopyFrom(RowData *pRowData)
		{
			this->BossID=pRowData->BossID;
			this->rank=pRowData->rank;
			this->reward=pRowData->reward;
			this->prestige=pRowData->prestige;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BossID", "rank", "reward", "prestige",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(prestige  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.BossID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.prestige)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~prestige ()
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