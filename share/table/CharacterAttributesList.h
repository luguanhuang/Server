#ifndef __CharacterAttributesList_H__
#define __CharacterAttributesList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CharacterAttributesList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	AttributeID;
		string	Section;

		void CopyFrom(RowData *pRowData)
		{
			this->AttributeID=pRowData->AttributeID;
			this->Section=pRowData->Section;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "AttributeID", "Section",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CharacterAttributesList  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.AttributeID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Section)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CharacterAttributesList ()
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