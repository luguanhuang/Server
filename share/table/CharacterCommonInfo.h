#ifndef __CharacterCommonInfo_H__
#define __CharacterCommonInfo_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/15 20:56:22


class CharacterCommonInfo : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	ShowText;
		uint	Type;

		void CopyFrom(RowData *pRowData)
		{
			this->ShowText=pRowData->ShowText;
			this->Type=pRowData->Type;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ShowText", "Type",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CharacterCommonInfo  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ShowText)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Type)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CharacterCommonInfo ()
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