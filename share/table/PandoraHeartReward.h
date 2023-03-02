#ifndef __PandoraHeartReward_H__
#define __PandoraHeartReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PandoraHeartReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	pandoraid;
		uint	itemid;
		uint	itemcount;
		string	itemname;
		uint	quality;
		uint	notice;
		uint	praise;
		Sequence<uint, 2>	showlevel;

		void CopyFrom(RowData *pRowData)
		{
			this->pandoraid=pRowData->pandoraid;
			this->itemid=pRowData->itemid;
			this->itemcount=pRowData->itemcount;
			this->itemname=pRowData->itemname;
			this->quality=pRowData->quality;
			this->notice=pRowData->notice;
			this->praise=pRowData->praise;
			this->showlevel=pRowData->showlevel;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "pandoraid", "itemid", "itemcount", "itemname", "quality", "notice", "praise", "showlevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PandoraHeartReward  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.pandoraid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.itemid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.itemcount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.itemname)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.notice)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.praise)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.showlevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PandoraHeartReward ()
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