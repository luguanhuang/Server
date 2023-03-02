#ifndef __IBSurvery_H__
#define __IBSurvery_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class IBSurvery : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		int	type;
		string	name;
		uint	icon;
		int	pos;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->type=pRowData->type;
			this->name=pRowData->name;
			this->icon=pRowData->icon;
			this->pos=pRowData->pos;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "type", "name", "icon", "pos",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(IBSurvery  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.pos)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~IBSurvery ()
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