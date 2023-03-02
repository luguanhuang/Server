#ifndef __OpenServerActivity_H__
#define __OpenServerActivity_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/22 11:46:45


class OpenServerActivity : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ServerLevel;
		vector<uint>	TaskIDs;

		void CopyFrom(RowData *pRowData)
		{
			this->ServerLevel=pRowData->ServerLevel;
			this->TaskIDs=pRowData->TaskIDs;

		}
	};

	std::unordered_map<uint,int> ServerLevel_Index;


    RowData *GetByServerLevel(const uint &key)
    {
        auto i = ServerLevel_Index.find(key);
        return (i == ServerLevel_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ServerLevel", "TaskIDs",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(OpenServerActivity  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByServerLevel(pSrcRow->ServerLevel);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ServerLevel_Index[pNewRow->ServerLevel]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ServerLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TaskIDs)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ServerLevel_Index[row.ServerLevel] = Table.size()-1;


		return true;
	}

	~OpenServerActivity ()
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

		ServerLevel_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif