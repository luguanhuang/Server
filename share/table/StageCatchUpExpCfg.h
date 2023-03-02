#ifndef __StageCatchUpExpCfg_H__
#define __StageCatchUpExpCfg_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class StageCatchUpExpCfg : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ServerDays;
		uint	level01;
		uint	level02;
		uint	level03;
		uint	level04;
		uint	ExpRate01;
		uint	ExpRate02;
		uint	ExpRate03;
		uint	ExpRate04;

		void CopyFrom(RowData *pRowData)
		{
			this->ServerDays=pRowData->ServerDays;
			this->level01=pRowData->level01;
			this->level02=pRowData->level02;
			this->level03=pRowData->level03;
			this->level04=pRowData->level04;
			this->ExpRate01=pRowData->ExpRate01;
			this->ExpRate02=pRowData->ExpRate02;
			this->ExpRate03=pRowData->ExpRate03;
			this->ExpRate04=pRowData->ExpRate04;

		}
	};

	std::unordered_map<uint,int> ServerDays_Index;


    RowData *GetByServerDays(const uint &key)
    {
        auto i = ServerDays_Index.find(key);
        return (i == ServerDays_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ServerDays", "level01", "level02", "level03", "level04", "ExpRate01", "ExpRate02", "ExpRate03", "ExpRate04",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(StageCatchUpExpCfg  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByServerDays(pSrcRow->ServerDays);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ServerDays_Index[pNewRow->ServerDays]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ServerDays)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.level01)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.level02)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.level03)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.level04)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ExpRate01)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ExpRate02)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ExpRate03)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ExpRate04)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ServerDays_Index[row.ServerDays] = Table.size()-1;


		return true;
	}

	~StageCatchUpExpCfg ()
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

		ServerDays_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif