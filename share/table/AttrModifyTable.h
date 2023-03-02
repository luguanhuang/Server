#ifndef __AttrModifyTable_H__
#define __AttrModifyTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/21 11:26:24


class AttrModifyTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ProfessionID;
		vector<int>	SceneID;
		vector<int>	SceneType;
		vector<Sequence<float, 3>>	PhysicalAtk;
		vector<Sequence<float, 3>>	PhysicalDef;
		vector<Sequence<float, 3>>	MaxHP;
		vector<Sequence<float, 3>>	MagicAtk;
		vector<Sequence<float, 3>>	MagicDef;
		vector<Sequence<float, 3>>	Critical;
		vector<Sequence<float, 3>>	CritResist;

		void CopyFrom(RowData *pRowData)
		{
			this->ProfessionID=pRowData->ProfessionID;
			this->SceneID=pRowData->SceneID;
			this->SceneType=pRowData->SceneType;
			this->PhysicalAtk=pRowData->PhysicalAtk;
			this->PhysicalDef=pRowData->PhysicalDef;
			this->MaxHP=pRowData->MaxHP;
			this->MagicAtk=pRowData->MagicAtk;
			this->MagicDef=pRowData->MagicDef;
			this->Critical=pRowData->Critical;
			this->CritResist=pRowData->CritResist;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ProfessionID", "SceneID", "SceneType", "PhysicalAtk", "PhysicalDef", "MaxHP", "MagicAtk", "MagicDef", "Critical", "CritResist",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AttrModifyTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SceneType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PhysicalAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PhysicalDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.MaxHP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MagicAtk)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.MagicDef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Critical)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.CritResist)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~AttrModifyTable ()
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