#ifndef __ActivityTable_H__
#define __ActivityTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/23 15:58:52


class ActivityTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		uint	count;
		uint	value;
		string	name;
		string	icon;
		Sequence<uint, 2>	level;
		vector<uint>	weekday;
		string	description;
		vector<Sequence<int, 2>>	item;
		uint	sortid;
		uint	random;
		string	title;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->count=pRowData->count;
			this->value=pRowData->value;
			this->name=pRowData->name;
			this->icon=pRowData->icon;
			this->level=pRowData->level;
			this->weekday=pRowData->weekday;
			this->description=pRowData->description;
			this->item=pRowData->item;
			this->sortid=pRowData->sortid;
			this->random=pRowData->random;
			this->title=pRowData->title;

		}
	};

	std::unordered_map<uint,int> sortid_Index;


    RowData *GetBysortid(const uint &key)
    {
        auto i = sortid_Index.find(key);
        return (i == sortid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "count", "value", "name", "icon", "level", "weekday", "description", "item", "sortid", "random", "title",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ActivityTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBysortid(pSrcRow->sortid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                sortid_Index[pNewRow->sortid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.count)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.value)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.weekday)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.item)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.sortid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.random)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.title)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		sortid_Index[row.sortid] = Table.size()-1;


		return true;
	}

	~ActivityTable ()
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

		sortid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif