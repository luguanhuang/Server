#ifndef __SlotAttrTable_H__
#define __SlotAttrTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SlotAttrTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	slotlevel;
		vector<Sequence<uint, 2>>	cost;
		bool	special;
		vector<Sequence<uint, 2>>	sloth;
		vector<Sequence<uint, 2>>	sloty;
		vector<Sequence<uint, 2>>	slotk;
		vector<Sequence<uint, 2>>	slots;
		vector<Sequence<uint, 2>>	slotx;
		vector<Sequence<uint, 2>>	slotw;
		vector<Sequence<uint, 2>>	slotf;
		vector<Sequence<uint, 2>>	slotn;
		vector<Sequence<uint, 2>>	slote;
		vector<Sequence<uint, 2>>	slotj;

		void CopyFrom(RowData *pRowData)
		{
			this->slotlevel=pRowData->slotlevel;
			this->cost=pRowData->cost;
			this->special=pRowData->special;
			this->sloth=pRowData->sloth;
			this->sloty=pRowData->sloty;
			this->slotk=pRowData->slotk;
			this->slots=pRowData->slots;
			this->slotx=pRowData->slotx;
			this->slotw=pRowData->slotw;
			this->slotf=pRowData->slotf;
			this->slotn=pRowData->slotn;
			this->slote=pRowData->slote;
			this->slotj=pRowData->slotj;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "slotlevel", "cost", "special", "sloth", "sloty", "slotk", "slots", "slotx", "slotw", "slotf", "slotn", "slote", "slotj",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SlotAttrTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.slotlevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.cost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.special)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.sloth)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.sloty)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.slotk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.slots)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.slotx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.slotw)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.slotf)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.slotn)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.slote)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.slotj)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SlotAttrTable ()
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