#ifndef __GaoJiGuFishTable_H__
#define __GaoJiGuFishTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GaoJiGuFishTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	fishID;
		string	name;
		int	quality;
		int	getExp;
		int	dropID;
		int	effect;

		void CopyFrom(RowData *pRowData)
		{
			this->fishID=pRowData->fishID;
			this->name=pRowData->name;
			this->quality=pRowData->quality;
			this->getExp=pRowData->getExp;
			this->dropID=pRowData->dropID;
			this->effect=pRowData->effect;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "fishID", "name", "quality", "getExp", "dropID", "effect",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GaoJiGuFishTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.fishID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.getExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.dropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.effect)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GaoJiGuFishTable ()
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