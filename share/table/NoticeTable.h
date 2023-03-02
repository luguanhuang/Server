#ifndef __NoticeTable_H__
#define __NoticeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class NoticeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		int	channel;
		string	info;
		vector<uint>	timebegin;
		vector<uint>	timeend;
		uint	timespan;
		uint	priority;
		uint	linkparam;
		string	linkcontent;
		uint	level;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->channel=pRowData->channel;
			this->info=pRowData->info;
			this->timebegin=pRowData->timebegin;
			this->timeend=pRowData->timeend;
			this->timespan=pRowData->timespan;
			this->priority=pRowData->priority;
			this->linkparam=pRowData->linkparam;
			this->linkcontent=pRowData->linkcontent;
			this->level=pRowData->level;

		}
	};

	std::unordered_map<uint,int> id_Index;


    RowData *GetByid(const uint &key)
    {
        auto i = id_Index.find(key);
        return (i == id_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "channel", "info", "timebegin", "timeend", "timespan", "priority", "linkparam", "linkcontent", "level",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NoticeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByid(pSrcRow->id);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                id_Index[pNewRow->id]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.channel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.info)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.timebegin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.timeend)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.timespan)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.priority)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.linkparam)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.linkcontent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.level)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~NoticeTable ()
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

		id_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif