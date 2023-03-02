#ifndef __PVEAttrModify_H__
#define __PVEAttrModify_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/17 15:32:33


class PVEAttrModify : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SceneID;
		uint	PhysicalAtk;
		uint	MagicAtk;
		uint	Critical;
		uint	CritResist;
		uint	FireAtk;
		uint	WaterAtk;
		uint	LightAtk;
		uint	DarkAtk;
		uint	PhysicalDef;
		uint	MagicDef;
		uint	HP;
		uint	FireDef;
		uint	WaterDef;
		uint	LightDef;
		uint	DarkDef;
		uint	Combat;
		double	RatioA;
		double	RatioB;
		double	RatioC;
		double	RatioD;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneID=pRowData->SceneID;
			this->PhysicalAtk=pRowData->PhysicalAtk;
			this->MagicAtk=pRowData->MagicAtk;
			this->Critical=pRowData->Critical;
			this->CritResist=pRowData->CritResist;
			this->FireAtk=pRowData->FireAtk;
			this->WaterAtk=pRowData->WaterAtk;
			this->LightAtk=pRowData->LightAtk;
			this->DarkAtk=pRowData->DarkAtk;
			this->PhysicalDef=pRowData->PhysicalDef;
			this->MagicDef=pRowData->MagicDef;
			this->HP=pRowData->HP;
			this->FireDef=pRowData->FireDef;
			this->WaterDef=pRowData->WaterDef;
			this->LightDef=pRowData->LightDef;
			this->DarkDef=pRowData->DarkDef;
			this->Combat=pRowData->Combat;
			this->RatioA=pRowData->RatioA;
			this->RatioB=pRowData->RatioB;
			this->RatioC=pRowData->RatioC;
			this->RatioD=pRowData->RatioD;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneID", "PhysicalAtk", "MagicAtk", "Critical", "CritResist", "FireAtk", "WaterAtk", "LightAtk", "DarkAtk", "PhysicalDef", "MagicDef", "HP", "FireDef", "WaterDef", "LightDef", "DarkDef", "Combat", "RatioA", "RatioB", "RatioC", "RatioD",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PVEAttrModify  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.PhysicalAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.MagicAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Critical)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.CritResist)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.FireAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.WaterAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.LightAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.DarkAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.PhysicalDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.MagicDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.HP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.FireDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.WaterDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.LightDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.DarkDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.Combat)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.RatioA)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.RatioB)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.RatioC)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.RatioD)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PVEAttrModify ()
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