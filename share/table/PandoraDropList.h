#ifndef __PandoraDropList_H__
#define __PandoraDropList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/15 11:17:05


class PandoraDropList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	PandoraDropID;
		uint	ProfID;
		uint	CommonDropID;
		uint	BetterDropID;
		vector<uint>	BestDropID;
		Sequence<uint, 2>	EnterBetterDropTimes;
		vector<Sequence<uint, 2>>	EnterBestDropTimes;

		void CopyFrom(RowData *pRowData)
		{
			this->PandoraDropID=pRowData->PandoraDropID;
			this->ProfID=pRowData->ProfID;
			this->CommonDropID=pRowData->CommonDropID;
			this->BetterDropID=pRowData->BetterDropID;
			this->BestDropID=pRowData->BestDropID;
			this->EnterBetterDropTimes=pRowData->EnterBetterDropTimes;
			this->EnterBestDropTimes=pRowData->EnterBestDropTimes;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "PandoraDropID", "ProfID", "CommonDropID", "BetterDropID", "BestDropID", "EnterBetterDropTimes", "EnterBestDropTimes",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PandoraDropList  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.PandoraDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.CommonDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.BetterDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.BestDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.EnterBetterDropTimes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.EnterBestDropTimes)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PandoraDropList ()
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