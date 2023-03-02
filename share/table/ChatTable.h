#ifndef __ChatTable_H__
#define __ChatTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/28 15:13:33


class ChatTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		uint	cd;
		uint	level;
		bool	isnotice;
		bool	isrecord;
		uint	length;
		string	sprName;
		string	miniSpr;
		string	name;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->cd=pRowData->cd;
			this->level=pRowData->level;
			this->isnotice=pRowData->isnotice;
			this->isrecord=pRowData->isrecord;
			this->length=pRowData->length;
			this->sprName=pRowData->sprName;
			this->miniSpr=pRowData->miniSpr;
			this->name=pRowData->name;

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
		const char *Headers[] = { "id", "cd", "level", "isnotice", "isrecord", "length", "sprName", "miniSpr", "name",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ChatTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.cd)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.isnotice)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.isrecord)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.length)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.sprName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.miniSpr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.name)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~ChatTable ()
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