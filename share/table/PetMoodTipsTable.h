#ifndef __PetMoodTipsTable_H__
#define __PetMoodTipsTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PetMoodTipsTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	level;
		int	value;
		string	tip;
		string	icon;
		uint	comprehend;
		Sequence<int, 2>	hourchange;
		string	tips;

		void CopyFrom(RowData *pRowData)
		{
			this->level=pRowData->level;
			this->value=pRowData->value;
			this->tip=pRowData->tip;
			this->icon=pRowData->icon;
			this->comprehend=pRowData->comprehend;
			this->hourchange=pRowData->hourchange;
			this->tips=pRowData->tips;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "level", "value", "tip", "icon", "comprehend", "hourchange", "tips",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PetMoodTipsTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.value)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.tip)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.comprehend)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.hourchange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.tips)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PetMoodTipsTable ()
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