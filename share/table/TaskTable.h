#ifndef __TaskTable_H__
#define __TaskTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/9/2 17:17:13


class TaskTable : public CVSReader
{
public:

	struct RowData
	{
		int	TaskID;
		vector<uint>	BeginTaskNPCID;
		vector<uint>	EndTaskNPCID;
		int	RequiredLevel;
		int	PassScene;
		int	Gold;
		int	Exp;
		vector<Sequence<int, 2>>	RewardItem;
		vector<string>	BeginContent1;
		vector<string>	EndContent1;
		string	InprocessContent;
		int	PreTaskID;
		vector<string>	BeginContent2;
		vector<string>	BeginContent3;
		vector<string>	EndContent2;
		vector<string>	EndContent3;
		vector<string>	BeginContent4;
		vector<string>	EndContent4;
		string	BeginDesc;
		string	InprocessDesc;
		string	EndDesc;

	};

	std::unordered_map<int,int> TaskID_Index;


        RowData *GetByTaskID(const int &key)
        {
            auto i = TaskID_Index.find(key);
            return (i == TaskID_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TaskID", "BeginTaskNPCID", "EndTaskNPCID", "RequiredLevel", "PassScene", "Gold", "Exp", "RewardItem", "BeginContent1", "EndContent1", "InprocessContent", "PreTaskID", "BeginContent2", "BeginContent3", "EndContent2", "EndContent3", "BeginContent4", "EndContent4", "BeginDesc", "InprocessDesc", "EndDesc",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.TaskID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.BeginTaskNPCID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EndTaskNPCID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.RequiredLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PassScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Gold)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Exp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.RewardItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.BeginContent1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.EndContent1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.InprocessContent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.PreTaskID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.BeginContent2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.BeginContent3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.EndContent2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.EndContent3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.BeginContent4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.EndContent4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.BeginDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.InprocessDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.EndDesc)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		TaskID_Index[row.TaskID] = Table.size()-1;


		return true;
	}

	~TaskTable ()
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