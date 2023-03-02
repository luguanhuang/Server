#ifndef __PayAileenTable_H__
#define __PayAileenTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PayAileenTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	ParamID;
		int	Days;
		int	Price;
		int	VipLimit;
		string	Name;
		string	Icon;
		string	Desc;
		int	Diamond;
		vector<int>	LevelSealGiftID;
		int	MemberLimit;
		string	ServiceCode;

		void CopyFrom(RowData *pRowData)
		{
			this->ParamID=pRowData->ParamID;
			this->Days=pRowData->Days;
			this->Price=pRowData->Price;
			this->VipLimit=pRowData->VipLimit;
			this->Name=pRowData->Name;
			this->Icon=pRowData->Icon;
			this->Desc=pRowData->Desc;
			this->Diamond=pRowData->Diamond;
			this->LevelSealGiftID=pRowData->LevelSealGiftID;
			this->MemberLimit=pRowData->MemberLimit;
			this->ServiceCode=pRowData->ServiceCode;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ParamID", "Days", "Price", "VipLimit", "Name", "Icon", "Desc", "Diamond", "LevelSealGiftID", "MemberLimit", "ServiceCode",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PayAileenTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ParamID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Days)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Price)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.VipLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Diamond)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.LevelSealGiftID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.MemberLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ServiceCode)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PayAileenTable ()
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