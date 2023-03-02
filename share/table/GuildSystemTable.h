#ifndef __GuildSystemTable_H__
#define __GuildSystemTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildSystemTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	system;
		string	desc;

		void CopyFrom(RowData *pRowData)
		{
			this->system=pRowData->system;
			this->desc=pRowData->desc;

		}
	};

	std::unordered_map<uint,int> system_Index;


    RowData *GetBysystem(const uint &key)
    {
        auto i = system_Index.find(key);
        return (i == system_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "system", "desc",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildSystemTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBysystem(pSrcRow->system);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                system_Index[pNewRow->system]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.system)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.desc)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		system_Index[row.system] = Table.size()-1;


		return true;
	}

	~GuildSystemTable ()
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

		system_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif