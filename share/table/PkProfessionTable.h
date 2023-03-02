#ifndef __PkProfessionTable_H__
#define __PkProfessionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PkProfessionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Profession;
		string	Name;
		double	PhysicalAtk;
		double	PhysicalDef;
		double	MaxHP;
		double	HPRecovery;
		double	MagicAtk;
		double	MagicDef;
		double	MaxMP;
		double	MPRecovery;
		double	Critical;
		double	CritResist;
		uint	Level;
		vector<int>	SceneType;
		Sequence<uint, 2>	SealRange;
		vector<Sequence<double, 2>>	AttrValue_s;

		void CopyFrom(RowData *pRowData)
		{
			this->Profession=pRowData->Profession;
			this->Name=pRowData->Name;
			this->PhysicalAtk=pRowData->PhysicalAtk;
			this->PhysicalDef=pRowData->PhysicalDef;
			this->MaxHP=pRowData->MaxHP;
			this->HPRecovery=pRowData->HPRecovery;
			this->MagicAtk=pRowData->MagicAtk;
			this->MagicDef=pRowData->MagicDef;
			this->MaxMP=pRowData->MaxMP;
			this->MPRecovery=pRowData->MPRecovery;
			this->Critical=pRowData->Critical;
			this->CritResist=pRowData->CritResist;
			this->Level=pRowData->Level;
			this->SceneType=pRowData->SceneType;
			this->SealRange=pRowData->SealRange;
			this->AttrValue_s=pRowData->AttrValue_s;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Profession", "Name", "PhysicalAtk", "PhysicalDef", "MaxHP", "HPRecovery", "MagicAtk", "MagicDef", "MaxMP", "MPRecovery", "Critical", "CritResist", "Level", "SceneType", "SealRange", "AttrValue_s",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PkProfessionTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.PhysicalAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PhysicalDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.MaxHP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.HPRecovery)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MagicAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.MagicDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.MaxMP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.MPRecovery)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Critical)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.CritResist)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.SceneType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.SealRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.AttrValue_s)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PkProfessionTable ()
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