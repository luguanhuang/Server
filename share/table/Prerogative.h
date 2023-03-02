#ifndef __Prerogative_H__
#define __Prerogative_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/19 10:48:55


class Prerogative : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Level;
		uint	MinDiamond;
		uint	MinScore;
		string	PreSprite;
		vector<uint>	PreAuthor;
		Sequence<string, 2>	PreIcon;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->MinDiamond=pRowData->MinDiamond;
			this->MinScore=pRowData->MinScore;
			this->PreSprite=pRowData->PreSprite;
			this->PreAuthor=pRowData->PreAuthor;
			this->PreIcon=pRowData->PreIcon;

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
		const char *Headers[] = { "Level", "MinDiamond", "MinScore", "PreSprite", "PreAuthor", "PreIcon",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(Prerogative  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.MinDiamond)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.MinScore)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PreSprite)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PreAuthor)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.PreIcon)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Level_Index[row.Level] = Table.size()-1;


		return true;
	}

	~Prerogative ()
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