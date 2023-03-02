#ifndef __PartnerLivenessTable_H__
#define __PartnerLivenessTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PartnerLivenessTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	liveness;
		Sequence<uint, 2>	level;
		vector<uint>	dropid;
		string	dropdesc;
		vector<Sequence<uint, 2>>	viewabledrop;
		uint	index;
		string	boxPic;

		void CopyFrom(RowData *pRowData)
		{
			this->liveness=pRowData->liveness;
			this->level=pRowData->level;
			this->dropid=pRowData->dropid;
			this->dropdesc=pRowData->dropdesc;
			this->viewabledrop=pRowData->viewabledrop;
			this->index=pRowData->index;
			this->boxPic=pRowData->boxPic;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "liveness", "level", "dropid", "dropdesc", "viewabledrop", "index", "boxPic",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PartnerLivenessTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.liveness)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.dropid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.dropdesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.viewabledrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.index)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.boxPic)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PartnerLivenessTable ()
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