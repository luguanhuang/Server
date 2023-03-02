#ifndef __ChestList_H__
#define __ChestList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ChestList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ItemID;
		vector<uint>	DropID;
		int	SelMode;
		int	VipLevelReq;
		int	DailyUseCount;
		int	Profession;
		bool	MultiOpen;
		vector<uint>	Times;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->DropID=pRowData->DropID;
			this->SelMode=pRowData->SelMode;
			this->VipLevelReq=pRowData->VipLevelReq;
			this->DailyUseCount=pRowData->DailyUseCount;
			this->Profession=pRowData->Profession;
			this->MultiOpen=pRowData->MultiOpen;
			this->Times=pRowData->Times;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "DropID", "SelMode", "VipLevelReq", "DailyUseCount", "Profession", "MultiOpen", "Times",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ChestList  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.DropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SelMode)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.VipLevelReq)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DailyUseCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MultiOpen)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Times)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ChestList ()
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