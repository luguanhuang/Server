#ifndef __RandomTaskTable_H__
#define __RandomTaskTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RandomTaskTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	TaskID;
		string	TaskTitle;
		string	TaskDescription;
		int	TaskCondition;
		int	TaskParam;
		vector<Sequence<int, 2>>	TaskReward;
		int	TaskType;

		void CopyFrom(RowData *pRowData)
		{
			this->TaskID=pRowData->TaskID;
			this->TaskTitle=pRowData->TaskTitle;
			this->TaskDescription=pRowData->TaskDescription;
			this->TaskCondition=pRowData->TaskCondition;
			this->TaskParam=pRowData->TaskParam;
			this->TaskReward=pRowData->TaskReward;
			this->TaskType=pRowData->TaskType;

		}
	};

	std::unordered_map<int,int> TaskID_Index;


    RowData *GetByTaskID(const int &key)
    {
        auto i = TaskID_Index.find(key);
        return (i == TaskID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TaskID", "TaskTitle", "TaskDescription", "TaskCondition", "TaskParam", "TaskReward", "TaskType",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RandomTaskTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.TaskTitle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TaskDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.TaskCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.TaskParam)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.TaskReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.TaskType)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		TaskID_Index[row.TaskID] = Table.size()-1;


		return true;
	}

	~RandomTaskTable ()
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