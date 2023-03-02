#ifndef __GaoJiGuTable_H__
#define __GaoJiGuTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GaoJiGuTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	level;
		int	needExp;
		int	modelNum;
		int	fishCount;
		int	fishingDropID;

		void CopyFrom(RowData *pRowData)
		{
			this->level=pRowData->level;
			this->needExp=pRowData->needExp;
			this->modelNum=pRowData->modelNum;
			this->fishCount=pRowData->fishCount;
			this->fishingDropID=pRowData->fishingDropID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "level", "needExp", "modelNum", "fishCount", "fishingDropID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GaoJiGuTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.needExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.modelNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.fishCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.fishingDropID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GaoJiGuTable ()
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