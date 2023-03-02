#ifndef __EnhanceEquipTable_H__
#define __EnhanceEquipTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class EnhanceEquipTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ItemID;
		uint	EnhanceLevel;
		vector<Sequence<uint, 2>>	RequiredItem;
		vector<Sequence<uint, 2>>	DecomposeItem;
		vector<Sequence<float, 2>>	SuccessRate;
		Sequence<uint, 2>	ProtectItem;
		uint	FailDowngrade;
		vector<Sequence<uint, 2>>	EnhanceAttribute;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->EnhanceLevel=pRowData->EnhanceLevel;
			this->RequiredItem=pRowData->RequiredItem;
			this->DecomposeItem=pRowData->DecomposeItem;
			this->SuccessRate=pRowData->SuccessRate;
			this->ProtectItem=pRowData->ProtectItem;
			this->FailDowngrade=pRowData->FailDowngrade;
			this->EnhanceAttribute=pRowData->EnhanceAttribute;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "EnhanceLevel", "RequiredItem", "DecomposeItem", "SuccessRate", "ProtectItem", "FailDowngrade", "EnhanceAttribute",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EnhanceEquipTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.EnhanceLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RequiredItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DecomposeItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SuccessRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ProtectItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.FailDowngrade)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.EnhanceAttribute)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EnhanceEquipTable ()
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