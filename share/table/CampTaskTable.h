#ifndef __CampTaskTable_H__
#define __CampTaskTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/5/18 15:31:50


class CampTaskTable : public CVSReader
{
public:

	struct RowData
	{
		uint	TaskID;
		string	TaskName;
		int	TaskType;
		int	TaskQuality;
		Sequence<int, 2>	TaskLevel;
		int	TaskPosition;
		int	TaskRange;
		vector<Sequence<int, 2>>	TaskReward;
		vector<Sequence<int, 2>>	RequireItem;
		int	RequireScene;
		string	TaskDiscribe;
		int	RequireForID;

	};

	std::unordered_map<uint,int> TaskID_Index;


        RowData *GetByTaskID(const uint &key)
        {
            auto i = TaskID_Index.find(key);
            return (i == TaskID_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TaskID", "TaskName", "TaskType", "TaskQuality", "TaskLevel", "TaskPosition", "TaskRange", "TaskReward", "RequireItem", "RequireScene", "TaskDiscribe", "RequireFortID",  };
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
		if (!Parse(Fields[m_ColMap[1]], row.TaskName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TaskType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.TaskQuality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.TaskLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.TaskPosition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.TaskRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.TaskReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.RequireItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.RequireScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.TaskDiscribe)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.RequireForID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		TaskID_Index[row.TaskID] = Table.size()-1;


		return true;
	}

	~CampTaskTable ()
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