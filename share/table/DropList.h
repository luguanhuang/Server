#ifndef __DropList_H__
#define __DropList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/6 15:26:01


class DropList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	DropID;
		int	ItemID;
		int	ItemCount;
		int	DropProb;
		Sequence<uint, 2>	price;
		Sequence<uint, 2>	DropLevel;
		bool	ItemBind;

		void CopyFrom(RowData *pRowData)
		{
			this->DropID=pRowData->DropID;
			this->ItemID=pRowData->ItemID;
			this->ItemCount=pRowData->ItemCount;
			this->DropProb=pRowData->DropProb;
			this->price=pRowData->price;
			this->DropLevel=pRowData->DropLevel;
			this->ItemBind=pRowData->ItemBind;

		}
	};

	std::unordered_map<int,int> DropID_Index;


    RowData *GetByDropID(const int &key)
    {
        auto i = DropID_Index.find(key);
        return (i == DropID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "DropID", "ItemID", "ItemCount", "DropProb", "price", "DropLevel", "ItemBind",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DropList  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByDropID(pSrcRow->DropID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                DropID_Index[pNewRow->DropID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.DropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ItemCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DropProb)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.price)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.DropLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ItemBind)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		DropID_Index[row.DropID] = Table.size()-1;


		return true;
	}

	~DropList ()
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

		DropID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif