#ifndef __EffectRandomList_H__
#define __EffectRandomList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/19 19:45:22


class EffectRandomList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EffectRandomID;
		uint	Profession;
		uint	Weight;
		uint	EffectID;

		void CopyFrom(RowData *pRowData)
		{
			this->EffectRandomID=pRowData->EffectRandomID;
			this->Profession=pRowData->Profession;
			this->Weight=pRowData->Weight;
			this->EffectID=pRowData->EffectID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EffectRandomID", "Profession", "Weight", "EffectID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EffectRandomList  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.EffectRandomID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Weight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.EffectID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EffectRandomList ()
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