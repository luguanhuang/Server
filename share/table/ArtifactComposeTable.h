#ifndef __ArtifactComposeTable_H__
#define __ArtifactComposeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/6 11:56:49


class ArtifactComposeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ArtifactLevel;
		uint	ArtifactQuality;
		vector<Sequence<uint, 2>>	ArtifactNum2DropID;

		void CopyFrom(RowData *pRowData)
		{
			this->ArtifactLevel=pRowData->ArtifactLevel;
			this->ArtifactQuality=pRowData->ArtifactQuality;
			this->ArtifactNum2DropID=pRowData->ArtifactNum2DropID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ArtifactLevel", "ArtifactQuality", "ArtifactNum2DropID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ArtifactComposeTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ArtifactLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ArtifactQuality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ArtifactNum2DropID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ArtifactComposeTable ()
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