#ifndef __CardRewardTable_H__
#define __CardRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CardRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	type;
		vector<Sequence<uint, 2>>	reward;
		bool	packet;
		uint	exp;
		uint	point;
		vector<Sequence<uint, 2>>	matchreward;
		vector<Sequence<uint, 2>>	jokerreward;

		void CopyFrom(RowData *pRowData)
		{
			this->type=pRowData->type;
			this->reward=pRowData->reward;
			this->packet=pRowData->packet;
			this->exp=pRowData->exp;
			this->point=pRowData->point;
			this->matchreward=pRowData->matchreward;
			this->jokerreward=pRowData->jokerreward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "type", "reward", "packet", "exp", "point", "matchreward", "jokerreward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CardRewardTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.packet)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.exp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.point)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.matchreward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.jokerreward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CardRewardTable ()
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