#ifndef __MobaHero_H__
#define __MobaHero_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/15 20:33:07


class MobaHero : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	HeroID;
		uint	Level;
		vector<Sequence<double, 2>>	AddAttr;

		void CopyFrom(RowData *pRowData)
		{
			this->HeroID=pRowData->HeroID;
			this->Level=pRowData->Level;
			this->AddAttr=pRowData->AddAttr;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "HeroID", "Level", "AddAttr",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(MobaHero  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.HeroID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AddAttr)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~MobaHero ()
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