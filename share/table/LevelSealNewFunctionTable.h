#ifndef __LevelSealNewFunctionTable_H__
#define __LevelSealNewFunctionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class LevelSealNewFunctionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Type;
		int	OpenLevel;
		string	Tag;
		string	IconName;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->OpenLevel=pRowData->OpenLevel;
			this->Tag=pRowData->Tag;
			this->IconName=pRowData->IconName;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "OpenLevel", "Tag", "IconName",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(LevelSealNewFunctionTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.OpenLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Tag)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.IconName)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~LevelSealNewFunctionTable ()
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