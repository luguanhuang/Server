#ifndef __CustomBattleSystemTable_H__
#define __CustomBattleSystemTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/17 10:55:47


class CustomBattleSystemTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		uint	type;
		Sequence<uint, 2>	end;
		Sequence<uint, 2>	ticket;
		uint	levellimit;
		string	desc;
		string	TitleSpriteName;
		string	IconSpritePath;
		int	ExpID;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->type=pRowData->type;
			this->end=pRowData->end;
			this->ticket=pRowData->ticket;
			this->levellimit=pRowData->levellimit;
			this->desc=pRowData->desc;
			this->TitleSpriteName=pRowData->TitleSpriteName;
			this->IconSpritePath=pRowData->IconSpritePath;
			this->ExpID=pRowData->ExpID;

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
		const char *Headers[] = { "id", "type", "end", "ticket", "levellimit", "desc", "TitleSpriteName", "IconSpritePath", "ExpID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CustomBattleSystemTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.end)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ticket)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.levellimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.TitleSpriteName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.IconSpritePath)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ExpID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~CustomBattleSystemTable ()
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