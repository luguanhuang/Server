#ifndef __FriendTable_H__
#define __FriendTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FriendTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	level;
		string	name;
		string	teamname;
		string	icon;
		uint	degree;
		uint	totaldegree;
		Sequence<uint, 2>	buf;
		uint	dropid;
		uint	noticeid;

		void CopyFrom(RowData *pRowData)
		{
			this->level=pRowData->level;
			this->name=pRowData->name;
			this->teamname=pRowData->teamname;
			this->icon=pRowData->icon;
			this->degree=pRowData->degree;
			this->totaldegree=pRowData->totaldegree;
			this->buf=pRowData->buf;
			this->dropid=pRowData->dropid;
			this->noticeid=pRowData->noticeid;

		}
	};

	std::unordered_map<uint,int> level_Index;


    RowData *GetBylevel(const uint &key)
    {
        auto i = level_Index.find(key);
        return (i == level_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "level", "name", "teamname", "icon", "degree", "totaldegree", "buf", "dropid", "noticeid",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FriendTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBylevel(pSrcRow->level);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                level_Index[pNewRow->level]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.teamname)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.degree)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.totaldegree)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.buf)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.dropid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.noticeid)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		level_Index[row.level] = Table.size()-1;


		return true;
	}

	~FriendTable ()
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

		level_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif