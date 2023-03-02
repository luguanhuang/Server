#ifndef __BackflowActivity_H__
#define __BackflowActivity_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/19 15:02:04


class BackflowActivity : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Type;
		uint	TaskId;
		Sequence<uint, 2>	WorldLevel;
		uint	Point;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->TaskId=pRowData->TaskId;
			this->WorldLevel=pRowData->WorldLevel;
			this->Point=pRowData->Point;

		}
	};

	std::unordered_map<uint,int> TaskId_Index;


    RowData *GetByTaskId(const uint &key)
    {
        auto i = TaskId_Index.find(key);
        return (i == TaskId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "TaskId", "WorldLevel", "Point",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BackflowActivity  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByTaskId(pSrcRow->TaskId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                TaskId_Index[pNewRow->TaskId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TaskId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.WorldLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Point)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		TaskId_Index[row.TaskId] = Table.size()-1;


		return true;
	}

	~BackflowActivity ()
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

		TaskId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif