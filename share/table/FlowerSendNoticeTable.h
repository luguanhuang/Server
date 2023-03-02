#ifndef __FlowerSendNoticeTable_H__
#define __FlowerSendNoticeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FlowerSendNoticeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ItemID;
		int	Num;
		uint	NoticeID;
		string	ThanksWords;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->Num=pRowData->Num;
			this->NoticeID=pRowData->NoticeID;
			this->ThanksWords=pRowData->ThanksWords;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "Num", "NoticeID", "ThanksWords",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FlowerSendNoticeTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Num)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.NoticeID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ThanksWords)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~FlowerSendNoticeTable ()
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