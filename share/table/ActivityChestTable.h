#ifndef __ActivityChestTable_H__
#define __ActivityChestTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/9 21:21:37


class ActivityChestTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	chest;
		Sequence<uint, 2>	level;
		vector<uint>	dropid;
		string	dropdesc;
		vector<Sequence<uint, 2>>	viewabledrop;

		void CopyFrom(RowData *pRowData)
		{
			this->chest=pRowData->chest;
			this->level=pRowData->level;
			this->dropid=pRowData->dropid;
			this->dropdesc=pRowData->dropdesc;
			this->viewabledrop=pRowData->viewabledrop;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "chest", "level", "dropid", "dropdesc", "viewabledrop",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ActivityChestTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.chest)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.dropid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.dropdesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.viewabledrop)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ActivityChestTable ()
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