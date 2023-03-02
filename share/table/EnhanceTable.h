#ifndef __EnhanceTable_H__
#define __EnhanceTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class EnhanceTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EquipPos;
		uint	EnhanceLevel;
		vector<Sequence<uint, 2>>	NeedItem;
		uint	SuccessRate;
		uint	UpRate;
		uint	IsNeedBreak;

		void CopyFrom(RowData *pRowData)
		{
			this->EquipPos=pRowData->EquipPos;
			this->EnhanceLevel=pRowData->EnhanceLevel;
			this->NeedItem=pRowData->NeedItem;
			this->SuccessRate=pRowData->SuccessRate;
			this->UpRate=pRowData->UpRate;
			this->IsNeedBreak=pRowData->IsNeedBreak;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EquipPos", "EnhanceLevel", "NeedItem", "SuccessRate", "UpRate", "IsNeedBreak",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EnhanceTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.EquipPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.EnhanceLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.NeedItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SuccessRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.UpRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.IsNeedBreak)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EnhanceTable ()
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