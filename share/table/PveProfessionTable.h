#ifndef __PveProfessionTable_H__
#define __PveProfessionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/17 11:19:22


class PveProfessionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	Profession;
		uint	SceneID;
		Sequence<uint, 2>	BuffID;
		vector<Sequence<double, 2>>	AttrValue_s;
		double	PhysicalAtk;
		double	PhysicalDef;
		double	HPRecovery;
		double	MagicAtk;
		double	MagicDef;
		double	MaxMP;
		double	MPRecovery;
		double	Critical;
		double	CritResist;
		double	MaxHP;
		uint	ProfessionID;

		void CopyFrom(RowData *pRowData)
		{
			this->Profession=pRowData->Profession;
			this->SceneID=pRowData->SceneID;
			this->BuffID=pRowData->BuffID;
			this->AttrValue_s=pRowData->AttrValue_s;
			this->PhysicalAtk=pRowData->PhysicalAtk;
			this->PhysicalDef=pRowData->PhysicalDef;
			this->HPRecovery=pRowData->HPRecovery;
			this->MagicAtk=pRowData->MagicAtk;
			this->MagicDef=pRowData->MagicDef;
			this->MaxMP=pRowData->MaxMP;
			this->MPRecovery=pRowData->MPRecovery;
			this->Critical=pRowData->Critical;
			this->CritResist=pRowData->CritResist;
			this->MaxHP=pRowData->MaxHP;
			this->ProfessionID=pRowData->ProfessionID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Profession", "SceneID", "BuffID", "AttrValue_s", "PhysicalAtk", "PhysicalDef", "HPRecovery", "MagicAtk", "MagicDef", "MaxMP", "MPRecovery", "Critical", "CritResist", "MaxHP", "ProfessionID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PveProfessionTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.BuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AttrValue_s)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PhysicalAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.PhysicalDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.HPRecovery)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.MagicAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.MagicDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.MaxMP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.MPRecovery)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Critical)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.CritResist)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.MaxHP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.ProfessionID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PveProfessionTable ()
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