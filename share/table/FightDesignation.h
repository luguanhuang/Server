#ifndef __FightDesignation_H__
#define __FightDesignation_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FightDesignation : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		string	Designation;
		string	Explanation;
		string	Effect;
		string	Color;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Designation=pRowData->Designation;
			this->Explanation=pRowData->Explanation;
			this->Effect=pRowData->Effect;
			this->Color=pRowData->Color;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Designation", "Explanation", "Effect", "Color",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FightDesignation  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Designation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Explanation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Effect)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Color)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~FightDesignation ()
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