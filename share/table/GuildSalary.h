#ifndef __GuildSalary_H__
#define __GuildSalary_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/10/25 17:28:07


class GuildSalary : public CVSReader
{
public:

	struct RowData
	{
		uint	GuildLevel;
		Sequence<uint, 4>	GuildReview;
		uint	AwardPenalty;
		vector<Sequence<uint, 2>>	NumberTransformation;
		vector<Sequence<uint, 2>>	PrestigeTransformation;
		vector<Sequence<uint, 2>>	ActiveTransformation;
		vector<Sequence<uint, 2>>	EXPTransformation;
		vector<Sequence<uint, 2>>	SSalary1;
		vector<Sequence<uint, 2>>	SSalary2;
		vector<Sequence<uint, 2>>	SSalary3;
		vector<Sequence<uint, 2>>	SSalary4;
		vector<Sequence<uint, 2>>	SSalary5;
		vector<Sequence<uint, 2>>	ASalary1;
		vector<Sequence<uint, 2>>	ASalary2;
		vector<Sequence<uint, 2>>	ASalary3;
		vector<Sequence<uint, 2>>	ASalary4;
		vector<Sequence<uint, 2>>	ASalary5;
		vector<Sequence<uint, 2>>	BSalary1;
		vector<Sequence<uint, 2>>	BSalary2;
		vector<Sequence<uint, 2>>	BSalary3;
		vector<Sequence<uint, 2>>	BSalary4;
		vector<Sequence<uint, 2>>	BSalary5;
		vector<Sequence<uint, 2>>	CSalary1;
		vector<Sequence<uint, 2>>	CSalary2;
		vector<Sequence<uint, 2>>	CSalary3;
		vector<Sequence<uint, 2>>	CSalary4;
		vector<Sequence<uint, 2>>	CSalary5;
		vector<Sequence<uint, 2>>	DSalary1;
		vector<Sequence<uint, 2>>	DSalary2;
		vector<Sequence<uint, 2>>	DSalary3;
		vector<Sequence<uint, 2>>	DSalary4;
		vector<Sequence<uint, 2>>	DSalary5;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GuildLevel", "GuildReview", "AwardPenalty", "NumberTransformation", "PrestigeTransformation", "ActiveTransformation", "EXPTransformation", "SSalary1", "SSalary2", "SSalary3", "SSalary4", "SSalary5", "ASalary1", "ASalary2", "ASalary3", "ASalary4", "ASalary5", "BSalary1", "BSalary2", "BSalary3", "BSalary4", "BSalary5", "CSalary1", "CSalary2", "CSalary3", "CSalary4", "CSalary5", "DSalary1", "DSalary2", "DSalary3", "DSalary4", "DSalary5",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.GuildLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.GuildReview)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AwardPenalty)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.NumberTransformation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PrestigeTransformation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ActiveTransformation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.EXPTransformation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SSalary1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.SSalary2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.SSalary3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.SSalary4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.SSalary5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.ASalary1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.ASalary2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.ASalary3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.ASalary4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.ASalary5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.BSalary1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.BSalary2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.BSalary3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.BSalary4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.BSalary5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.CSalary1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.CSalary2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.CSalary3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.CSalary4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.CSalary5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.DSalary1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.DSalary2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.DSalary3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.DSalary4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.DSalary5)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildSalary ()
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