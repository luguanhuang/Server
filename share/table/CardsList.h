#ifndef __CardsList_H__
#define __CardsList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CardsList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	CardId;
		string	CardName;
		uint	GroupId;
		string	Description;
		uint	Avatar;
		uint	MapID;

		void CopyFrom(RowData *pRowData)
		{
			this->CardId=pRowData->CardId;
			this->CardName=pRowData->CardName;
			this->GroupId=pRowData->GroupId;
			this->Description=pRowData->Description;
			this->Avatar=pRowData->Avatar;
			this->MapID=pRowData->MapID;

		}
	};

	std::unordered_map<uint,int> CardId_Index;


    RowData *GetByCardId(const uint &key)
    {
        auto i = CardId_Index.find(key);
        return (i == CardId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "CardId", "CardName", "GroupId", "Description", "Avatar", "MapID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CardsList  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByCardId(pSrcRow->CardId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                CardId_Index[pNewRow->CardId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.CardId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.CardName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.GroupId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Avatar)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.MapID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		CardId_Index[row.CardId] = Table.size()-1;


		return true;
	}

	~CardsList ()
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

		CardId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif