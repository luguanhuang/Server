#ifndef __GuildCheckinBoxTable_H__
#define __GuildCheckinBoxTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildCheckinBoxTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	process;
		vector<Sequence<uint, 2>>	reward;
		vector<Sequence<uint, 2>>	viewabledrop;

		void CopyFrom(RowData *pRowData)
		{
			this->process=pRowData->process;
			this->reward=pRowData->reward;
			this->viewabledrop=pRowData->viewabledrop;

		}
	};

	std::unordered_map<uint,int> process_Index;


    RowData *GetByprocess(const uint &key)
    {
        auto i = process_Index.find(key);
        return (i == process_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "process", "reward", "viewabledrop",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildCheckinBoxTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByprocess(pSrcRow->process);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                process_Index[pNewRow->process]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.process)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.viewabledrop)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		process_Index[row.process] = Table.size()-1;


		return true;
	}

	~GuildCheckinBoxTable ()
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

		process_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif