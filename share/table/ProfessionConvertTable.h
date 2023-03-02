#ifndef __ProfessionConvertTable_H__
#define __ProfessionConvertTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ProfessionConvertTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ProfessionID;
		int	AttributeID;
		double	PhysicalAtk;
		double	PhysicalDef;
		double	MagicAtk;
		double	MagicDef;
		double	Critical;
		double	CritResist;
		double	Stun;
		double	StunResist;
		double	Paralyze;
		double	ParaResist;
		double	MaxHP;
		double	MaxMP;
		double	CombatScore;
		double	CritDamage;

		void CopyFrom(RowData *pRowData)
		{
			this->ProfessionID=pRowData->ProfessionID;
			this->AttributeID=pRowData->AttributeID;
			this->PhysicalAtk=pRowData->PhysicalAtk;
			this->PhysicalDef=pRowData->PhysicalDef;
			this->MagicAtk=pRowData->MagicAtk;
			this->MagicDef=pRowData->MagicDef;
			this->Critical=pRowData->Critical;
			this->CritResist=pRowData->CritResist;
			this->Stun=pRowData->Stun;
			this->StunResist=pRowData->StunResist;
			this->Paralyze=pRowData->Paralyze;
			this->ParaResist=pRowData->ParaResist;
			this->MaxHP=pRowData->MaxHP;
			this->MaxMP=pRowData->MaxMP;
			this->CombatScore=pRowData->CombatScore;
			this->CritDamage=pRowData->CritDamage;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ProfessionID", "AttributeID", "PhysicalAtk", "PhysicalDef", "MagicAtk", "MagicDef", "Critical", "CritResist", "Stun", "StunResist", "Paralyze", "ParaResist", "MaxHP", "MaxMP", "CombatScore", "CritDamage",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ProfessionConvertTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ProfessionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.AttributeID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.PhysicalAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PhysicalDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.MagicAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.MagicDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Critical)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.CritResist)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Stun)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.StunResist)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Paralyze)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.ParaResist)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.MaxHP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.MaxMP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.CombatScore)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.CritDamage)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ProfessionConvertTable ()
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