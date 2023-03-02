#ifndef __TaskTableNew_H__
#define __TaskTableNew_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/15 10:25:17


class TaskTableNew : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	TaskType;
		uint	TaskID;
		uint	PreTaskID;
		vector<uint>	TaskTime;
		vector<uint>	EndTime;
		vector<uint>	BeginTaskNPCID;
		vector<uint>	EndTaskNPCID;
		uint	RequiredLevel;
		string	TaskTitle;
		string	TaskDesc;
		string	InprocessDesc;
		vector<Sequence<uint, 2>>	PassScene;
		vector<Sequence<uint, 2>>	TaskScene;
		vector<Sequence<uint, 2>>	TaskItem;
		vector<Sequence<uint, 2>>	TaskMonster;
		vector<Sequence<uint, 2>>	RewardItem;
		string	BeginDesc;
		string	EndDesc;
		vector<uint>	MailID;
		uint	ActivityID;

		void CopyFrom(RowData *pRowData)
		{
			this->TaskType=pRowData->TaskType;
			this->TaskID=pRowData->TaskID;
			this->PreTaskID=pRowData->PreTaskID;
			this->TaskTime=pRowData->TaskTime;
			this->EndTime=pRowData->EndTime;
			this->BeginTaskNPCID=pRowData->BeginTaskNPCID;
			this->EndTaskNPCID=pRowData->EndTaskNPCID;
			this->RequiredLevel=pRowData->RequiredLevel;
			this->TaskTitle=pRowData->TaskTitle;
			this->TaskDesc=pRowData->TaskDesc;
			this->InprocessDesc=pRowData->InprocessDesc;
			this->PassScene=pRowData->PassScene;
			this->TaskScene=pRowData->TaskScene;
			this->TaskItem=pRowData->TaskItem;
			this->TaskMonster=pRowData->TaskMonster;
			this->RewardItem=pRowData->RewardItem;
			this->BeginDesc=pRowData->BeginDesc;
			this->EndDesc=pRowData->EndDesc;
			this->MailID=pRowData->MailID;
			this->ActivityID=pRowData->ActivityID;

		}
	};

	std::unordered_map<uint,int> TaskID_Index;


    RowData *GetByTaskID(const uint &key)
    {
        auto i = TaskID_Index.find(key);
        return (i == TaskID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TaskType", "TaskID", "PreTaskID", "TaskTime", "EndTime", "BeginTaskNPCID", "EndTaskNPCID", "RequiredLevel", "TaskTitle", "TaskDesc", "InprocessDesc", "PassScene", "TaskScene", "TaskItem", "TaskMonster", "RewardItem", "BeginDesc", "EndDesc", "MailID", "ActivityID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(TaskTableNew  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByTaskID(pSrcRow->TaskID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                TaskID_Index[pNewRow->TaskID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.TaskType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TaskID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.PreTaskID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.TaskTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.EndTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BeginTaskNPCID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.EndTaskNPCID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.RequiredLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.TaskTitle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.TaskDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.InprocessDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.PassScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.TaskScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.TaskItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.TaskMonster)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.RewardItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.BeginDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.EndDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.MailID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.ActivityID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		TaskID_Index[row.TaskID] = Table.size()-1;


		return true;
	}

	~TaskTableNew ()
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

		TaskID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif