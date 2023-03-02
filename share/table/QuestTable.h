#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2014/9/28 17:05:29


class QuestTable : public CVSReader
{
public:

	struct RowData
	{
		int	m_Qid;
		string	m_Description;
		vector<string>	m_Condition1;
		vector<string>	m_Condition2;
		vector<string>	m_Condition3;
		vector<string>	m_Condition4;
		vector<string>	m_SubState1;
		vector<string>	m_SubState2;
		vector<string>	m_SubState3;
		vector<string>	m_SubState4;
		int	m_RewardMoney;
		int	m_RewardExp;
		map<int, int>	m_RewardItems;

	};

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "QuestID", "QuestDescription", "TakeCond1", "TakeCond2", "TakeCond3", "TakeCond4", "SubState1", "SubState2", "SubState3", "SubState4", "RewardMoney", "RewardExp", "RewardItems",  };
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
		RowData row;

		if (!Parse(Fields[m_ColMap[0]], row.m_Qid)) return false;
		if (!Parse(Fields[m_ColMap[1]], row.m_Description)) return false;
		if (!Parse(Fields[m_ColMap[2]], row.m_Condition1)) return false;
		if (!Parse(Fields[m_ColMap[3]], row.m_Condition2)) return false;
		if (!Parse(Fields[m_ColMap[4]], row.m_Condition3)) return false;
		if (!Parse(Fields[m_ColMap[5]], row.m_Condition4)) return false;
		if (!Parse(Fields[m_ColMap[6]], row.m_SubState1)) return false;
		if (!Parse(Fields[m_ColMap[7]], row.m_SubState2)) return false;
		if (!Parse(Fields[m_ColMap[8]], row.m_SubState3)) return false;
		if (!Parse(Fields[m_ColMap[9]], row.m_SubState4)) return false;
		if (!Parse(Fields[m_ColMap[10]], row.m_RewardMoney)) return false;
		if (!Parse(Fields[m_ColMap[11]], row.m_RewardExp)) return false;
		if (!Parse(Fields[m_ColMap[12]], row.m_RewardItems)) return false;

		
		Table.push_back(row);
		return true;
	}

	std::vector<RowData> Table;
};

