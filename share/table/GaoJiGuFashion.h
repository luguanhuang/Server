#ifndef __GaoJiGuFashion_H__
#define __GaoJiGuFashion_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GaoJiGuFashion : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Quality;
		int	EquipPos;
		int	FashionLevel;
		int	FashionScore;
		float	FashionFactor;

		void CopyFrom(RowData *pRowData)
		{
			this->Quality=pRowData->Quality;
			this->EquipPos=pRowData->EquipPos;
			this->FashionLevel=pRowData->FashionLevel;
			this->FashionScore=pRowData->FashionScore;
			this->FashionFactor=pRowData->FashionFactor;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Quality", "EquipPos", "FashionLevel", "FashionScore", "FashionFactor",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GaoJiGuFashion  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.EquipPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.FashionLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.FashionScore)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.FashionFactor)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GaoJiGuFashion ()
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