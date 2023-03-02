#ifndef __AuctItemRecovery_H__
#define __AuctItemRecovery_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/24 15:58:48


class AuctItemRecovery : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ItemID;
		uint	AuctCartItemAllCount;
		uint	ItemThreshold;
		uint	RecoveryCount;
		uint	AuctPrevent;
		Sequence<int, 2>	WorldLevel;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->AuctCartItemAllCount=pRowData->AuctCartItemAllCount;
			this->ItemThreshold=pRowData->ItemThreshold;
			this->RecoveryCount=pRowData->RecoveryCount;
			this->AuctPrevent=pRowData->AuctPrevent;
			this->WorldLevel=pRowData->WorldLevel;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "AuctCartItemAllCount", "ItemThreshold", "RecoveryCount", "AuctPrevent", "WorldLevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AuctItemRecovery  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.AuctCartItemAllCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ItemThreshold)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.RecoveryCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.AuctPrevent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.WorldLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~AuctItemRecovery ()
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