#ifndef __SceneFxTable_H__
#define __SceneFxTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SceneFxTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	unityscene;
		string	name;
		vector<float>	position;
		vector<float>	rotation;
		vector<float>	scale;

		void CopyFrom(RowData *pRowData)
		{
			this->unityscene=pRowData->unityscene;
			this->name=pRowData->name;
			this->position=pRowData->position;
			this->rotation=pRowData->rotation;
			this->scale=pRowData->scale;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "unityscene", "name", "position", "rotation", "scale",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SceneFxTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.unityscene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.position)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.rotation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.scale)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SceneFxTable ()
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