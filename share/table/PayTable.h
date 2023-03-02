#ifndef __PayTable_H__
#define __PayTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PayTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	PayID;
		int	PayCount;
		int	BuyDragonCoin;
		int	FirstTimeDouble;
		Sequence<uint, 2>	MonthCardReward;
		string	Description;

		void CopyFrom(RowData *pRowData)
		{
			this->PayID=pRowData->PayID;
			this->PayCount=pRowData->PayCount;
			this->BuyDragonCoin=pRowData->BuyDragonCoin;
			this->FirstTimeDouble=pRowData->FirstTimeDouble;
			this->MonthCardReward=pRowData->MonthCardReward;
			this->Description=pRowData->Description;

		}
	};

	std::unordered_map<int,int> PayID_Index;


    RowData *GetByPayID(const int &key)
    {
        auto i = PayID_Index.find(key);
        return (i == PayID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "PayID", "PayCount", "BuyDragonCoin", "FirstTimeDouble", "MonthCardReward", "Description",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PayTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByPayID(pSrcRow->PayID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                PayID_Index[pNewRow->PayID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.PayID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.PayCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.BuyDragonCoin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.FirstTimeDouble)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.MonthCardReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Description)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		PayID_Index[row.PayID] = Table.size()-1;


		return true;
	}

	~PayTable ()
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

		PayID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif