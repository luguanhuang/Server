#ifndef __CombatParamTable_H__
#define __CombatParamTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CombatParamTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	PlayerLevel;
		int	CriticalBase;
		int	CritResistBase;
		int	ParalyzeBase;
		int	ParaResistBase;
		int	StunBase;
		int	StunResistBase;
		int	CritDamageBase;
		int	FinalDamageBase;
		int	PhysicalDef;
		int	MagicDef;
		int	ElementAtk;
		int	ElementDef;

		void CopyFrom(RowData *pRowData)
		{
			this->PlayerLevel=pRowData->PlayerLevel;
			this->CriticalBase=pRowData->CriticalBase;
			this->CritResistBase=pRowData->CritResistBase;
			this->ParalyzeBase=pRowData->ParalyzeBase;
			this->ParaResistBase=pRowData->ParaResistBase;
			this->StunBase=pRowData->StunBase;
			this->StunResistBase=pRowData->StunResistBase;
			this->CritDamageBase=pRowData->CritDamageBase;
			this->FinalDamageBase=pRowData->FinalDamageBase;
			this->PhysicalDef=pRowData->PhysicalDef;
			this->MagicDef=pRowData->MagicDef;
			this->ElementAtk=pRowData->ElementAtk;
			this->ElementDef=pRowData->ElementDef;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "PlayerLevel", "CriticalBase", "CritResistBase", "ParalyzeBase", "ParaResistBase", "StunBase", "StunResistBase", "CritDamageBase", "FinalDamageBase", "PhysicalDef", "MagicDef", "ElementAtk", "ElementDef",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CombatParamTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.PlayerLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.CriticalBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.CritResistBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ParalyzeBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ParaResistBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.StunBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.StunResistBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.CritDamageBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.FinalDamageBase)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.PhysicalDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.MagicDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.ElementAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.ElementDef)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CombatParamTable ()
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