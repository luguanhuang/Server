#ifndef __SuperActivityTask_H__
#define __SuperActivityTask_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SuperActivityTask : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	taskid;
		string	title;
		uint	basetask;
		vector<uint>	num;
		vector<Sequence<uint, 2>>	items;
		string	icon;
		uint	type;
		uint	belong;
		uint	jump;
		uint	actid;
		int	cnt;
		vector<int>	arg;
		uint	tasktype;
		vector<uint>	taskson;
		uint	taskfather;

		void CopyFrom(RowData *pRowData)
		{
			this->taskid=pRowData->taskid;
			this->title=pRowData->title;
			this->basetask=pRowData->basetask;
			this->num=pRowData->num;
			this->items=pRowData->items;
			this->icon=pRowData->icon;
			this->type=pRowData->type;
			this->belong=pRowData->belong;
			this->jump=pRowData->jump;
			this->actid=pRowData->actid;
			this->cnt=pRowData->cnt;
			this->arg=pRowData->arg;
			this->tasktype=pRowData->tasktype;
			this->taskson=pRowData->taskson;
			this->taskfather=pRowData->taskfather;

		}
	};

	std::unordered_map<uint,int> taskid_Index;


    RowData *GetBytaskid(const uint &key)
    {
        auto i = taskid_Index.find(key);
        return (i == taskid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "taskid", "title", "basetask", "num", "items", "icon", "type", "belong", "jump", "actid", "cnt", "arg", "tasktype", "taskson", "taskfather",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SuperActivityTask  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBytaskid(pSrcRow->taskid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                taskid_Index[pNewRow->taskid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.taskid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.title)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.basetask)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.num)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.items)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.belong)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.jump)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.actid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.cnt)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.arg)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.tasktype)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.taskson)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.taskfather)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		taskid_Index[row.taskid] = Table.size()-1;


		return true;
	}

	~SuperActivityTask ()
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

		taskid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif