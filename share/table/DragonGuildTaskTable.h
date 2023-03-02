#ifndef __DragonGuildTaskTable_H__
#define __DragonGuildTaskTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/21 16:02:58


class DragonGuildTaskTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	taskID;
		uint	taskType;
		string	name;
		uint	SceneID;
		string	icon;
		Sequence<uint, 2>	worldlevel;
		uint	count;
		uint	guildExp;
		vector<Sequence<uint, 2>>	viewabledrop;
		vector<uint>	dropID;
		uint	value;

		void CopyFrom(RowData *pRowData)
		{
			this->taskID=pRowData->taskID;
			this->taskType=pRowData->taskType;
			this->name=pRowData->name;
			this->SceneID=pRowData->SceneID;
			this->icon=pRowData->icon;
			this->worldlevel=pRowData->worldlevel;
			this->count=pRowData->count;
			this->guildExp=pRowData->guildExp;
			this->viewabledrop=pRowData->viewabledrop;
			this->dropID=pRowData->dropID;
			this->value=pRowData->value;

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
		const char *Headers[] = { "taskID", "taskType", "name", "SceneID", "icon", "worldlevel", "count", "guildExp", "viewabledrop", "dropID", "value",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DragonGuildTaskTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.taskType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.worldlevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.count)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.guildExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.viewabledrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.dropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.value)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		taskID_Index[row.taskID] = Table.size()-1;


		return true;
	}

	~DragonGuildTaskTable ()
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