#ifndef __EquipFusionTable_H__
#define __EquipFusionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/19 16:26:08


class EquipFusionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Profession;
		uint	Slot;
		uint	EquipType;
		uint	BreakNum;
		uint	LevelNum;
		uint	NeedExpPerLevel;
		vector<Sequence<uint, 2>>	LevelAddAttr;
		vector<Sequence<uint, 2>>	BreakAddAttr;
		vector<Sequence<uint, 2>>	BreakNeedMaterial;

		void CopyFrom(RowData *pRowData)
		{
			this->Profession=pRowData->Profession;
			this->Slot=pRowData->Slot;
			this->EquipType=pRowData->EquipType;
			this->BreakNum=pRowData->BreakNum;
			this->LevelNum=pRowData->LevelNum;
			this->NeedExpPerLevel=pRowData->NeedExpPerLevel;
			this->LevelAddAttr=pRowData->LevelAddAttr;
			this->BreakAddAttr=pRowData->BreakAddAttr;
			this->BreakNeedMaterial=pRowData->BreakNeedMaterial;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Profession", "Slot", "EquipType", "BreakNum", "LevelNum", "NeedExpPerLevel", "LevelAddAttr", "BreakAddAttr", "BreakNeedMaterial",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EquipFusionTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Slot)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EquipType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.BreakNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.LevelNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.NeedExpPerLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.LevelAddAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.BreakAddAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.BreakNeedMaterial)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EquipFusionTable ()
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