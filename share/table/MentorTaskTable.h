#ifndef __MentorTaskTable_H__
#define __MentorTaskTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/27 14:58:14


class MentorTaskTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	TaskID;
		uint	TaskType;
		string	TaskName;
		Sequence<int, 2>	TaskVar;
		vector<Sequence<int, 2>>	MasterReward;
		vector<Sequence<int, 2>>	StudentReward;
		int	UseThisTask;

		void CopyFrom(RowData *pRowData)
		{
			this->TaskID=pRowData->TaskID;
			this->TaskType=pRowData->TaskType;
			this->TaskName=pRowData->TaskName;
			this->TaskVar=pRowData->TaskVar;
			this->MasterReward=pRowData->MasterReward;
			this->StudentReward=pRowData->StudentReward;
			this->UseThisTask=pRowData->UseThisTask;

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
		const char *Headers[] = { "TaskID", "TaskType", "TaskName", "TaskVar", "MasterReward", "StudentReward", "UseThisTask",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(MentorTaskTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.TaskID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TaskType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TaskName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.TaskVar)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.MasterReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.StudentReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.UseThisTask)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		TaskID_Index[row.TaskID] = Table.size()-1;


		return true;
	}

	~MentorTaskTable ()
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