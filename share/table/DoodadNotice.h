#ifndef __DoodadNotice_H__
#define __DoodadNotice_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/31 14:41:18


class DoodadNotice : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SceneType;
		uint	ItemID;
		uint	ItemCount;
		uint	NoticeID;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneType=pRowData->SceneType;
			this->ItemID=pRowData->ItemID;
			this->ItemCount=pRowData->ItemCount;
			this->NoticeID=pRowData->NoticeID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneType", "ItemID", "ItemCount", "NoticeID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DoodadNotice  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.SceneType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ItemCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.NoticeID)) return false; ++m_ColNum;


		Table.push_back(pRow);



		return true;
	}

	~DoodadNotice ()
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