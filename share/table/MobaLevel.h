#ifndef __MobaLevel_H__
#define __MobaLevel_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/15 14:49:45


class MobaLevel : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Level;
		double	Exp;
		uint	ReviveTime;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->Exp=pRowData->Exp;
			this->ReviveTime=pRowData->ReviveTime;

		}
	};

	std::unordered_map<uint,int> Level_Index;


    RowData *GetByLevel(const uint &key)
    {
        auto i = Level_Index.find(key);
        return (i == Level_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "Exp", "ReviveTime",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(MobaLevel  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByLevel(pSrcRow->Level);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Level_Index[pNewRow->Level]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Exp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ReviveTime)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Level_Index[row.Level] = Table.size()-1;


		return true;
	}

	~MobaLevel ()
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

		Level_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif