#ifndef __CarnivalList_H__
#define __CarnivalList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CarnivalList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	id;
		string	title;
		vector<Sequence<uint, 2>>	items;
		string	icon;
		int	type;
		int	belong;
		int	jump;
		int	cnt;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->title=pRowData->title;
			this->items=pRowData->items;
			this->icon=pRowData->icon;
			this->type=pRowData->type;
			this->belong=pRowData->belong;
			this->jump=pRowData->jump;
			this->cnt=pRowData->cnt;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "title", "items", "icon", "type", "belong", "jump", "cnt",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CarnivalList  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.title)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.items)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.belong)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.jump)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.cnt)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CarnivalList ()
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