#ifndef __PetLevelTable_H__
#define __PetLevelTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PetLevelTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	PetsID;
		uint	level;
		uint	exp;
		vector<Sequence<uint, 2>>	PetsAttributes;

		void CopyFrom(RowData *pRowData)
		{
			this->PetsID=pRowData->PetsID;
			this->level=pRowData->level;
			this->exp=pRowData->exp;
			this->PetsAttributes=pRowData->PetsAttributes;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "PetsID", "level", "exp", "PetsAttributes",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PetLevelTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.PetsID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.exp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PetsAttributes)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PetLevelTable ()
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