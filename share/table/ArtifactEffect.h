#ifndef __ArtifactEffect_H__
#define __ArtifactEffect_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/16 15:55:09


class ArtifactEffect : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Quanlity;
		uint	AttrTyte;
		string	Path;

		void CopyFrom(RowData *pRowData)
		{
			this->Quanlity=pRowData->Quanlity;
			this->AttrTyte=pRowData->AttrTyte;
			this->Path=pRowData->Path;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Quanlity", "AttrTyte", "Path",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ArtifactEffect  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Quanlity)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.AttrTyte)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Path)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ArtifactEffect ()
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