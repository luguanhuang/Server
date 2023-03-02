#ifndef __DailyTask_H__
#define __DailyTask_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/15 16:51:46


class DailyTask : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	taskID;
		Sequence<uint, 2>	level;
		uint	taskquality;
		uint	tasktype;
		vector<uint>	conditionId;
		string	taskdescription;
		uint	conditionNum;
		uint	NPCID;
		vector<Sequence<uint, 2>>	BQ;
		int	Option;
		uint	category;
		uint	score;
		string	TaskIcon;
		string	AtlasName;

		void CopyFrom(RowData *pRowData)
		{
			this->taskID=pRowData->taskID;
			this->level=pRowData->level;
			this->taskquality=pRowData->taskquality;
			this->tasktype=pRowData->tasktype;
			this->conditionId=pRowData->conditionId;
			this->taskdescription=pRowData->taskdescription;
			this->conditionNum=pRowData->conditionNum;
			this->NPCID=pRowData->NPCID;
			this->BQ=pRowData->BQ;
			this->Option=pRowData->Option;
			this->category=pRowData->category;
			this->score=pRowData->score;
			this->TaskIcon=pRowData->TaskIcon;
			this->AtlasName=pRowData->AtlasName;

		}
	};

	std::unordered_map<uint,int> taskID_Index;


    RowData *GetBytaskID(const uint &key)
    {
        auto i = taskID_Index.find(key);
        return (i == taskID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "taskID", "level", "taskquality", "tasktype", "conditionId", "taskdescription", "conditionNum", "NPCID", "BQ", "Option", "category", "score", "TaskIcon", "AtlasName",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DailyTask  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBytaskID(pSrcRow->taskID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                taskID_Index[pNewRow->taskID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.taskID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.taskquality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.tasktype)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.conditionId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.taskdescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.conditionNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.NPCID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.BQ)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Option)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.category)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.score)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.TaskIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.AtlasName)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		taskID_Index[row.taskID] = Table.size()-1;


		return true;
	}

	~DailyTask ()
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

		taskID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif