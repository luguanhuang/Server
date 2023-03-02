#ifndef __RandomSceneTable_H__
#define __RandomSceneTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RandomSceneTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	RandomID;
		uint	SceneID;
		uint	Prob;

		void CopyFrom(RowData *pRowData)
		{
			this->RandomID=pRowData->RandomID;
			this->SceneID=pRowData->SceneID;
			this->Prob=pRowData->Prob;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "RandomID", "SceneID", "Prob",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RandomSceneTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.RandomID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Prob)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~RandomSceneTable ()
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