#ifndef __TitleTable_H__
#define __TitleTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/14 11:22:28


class TitleTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	RankID;
		string	RankName;
		string	RankIcon;
		string	AffectRoute;
		uint	NeedPowerPoint;
		vector<Sequence<uint, 2>>	NeedItem;
		vector<Sequence<uint, 2>>	Attribute;
		string	RankPath;
		string	desc;
		uint	BasicProfession;
		string	RankAtlas;

		void CopyFrom(RowData *pRowData)
		{
			this->RankID=pRowData->RankID;
			this->RankName=pRowData->RankName;
			this->RankIcon=pRowData->RankIcon;
			this->AffectRoute=pRowData->AffectRoute;
			this->NeedPowerPoint=pRowData->NeedPowerPoint;
			this->NeedItem=pRowData->NeedItem;
			this->Attribute=pRowData->Attribute;
			this->RankPath=pRowData->RankPath;
			this->desc=pRowData->desc;
			this->BasicProfession=pRowData->BasicProfession;
			this->RankAtlas=pRowData->RankAtlas;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "RankID", "RankName", "RankIcon", "AffectRoute", "NeedPowerPoint", "NeedItem", "Attribute", "RankPath", "desc", "BasicProfession", "RankAtlas",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(TitleTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.RankID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.RankName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RankIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AffectRoute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.NeedPowerPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.NeedItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Attribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.RankPath)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.BasicProfession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.RankAtlas)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~TitleTable ()
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