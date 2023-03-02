#ifndef __EnchantEquip_H__
#define __EnchantEquip_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/28 17:32:38


class EnchantEquip : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EnchantID;
		vector<uint>	Pos;
		vector<Sequence<uint, 4>>	Attribute;
		vector<Sequence<uint, 2>>	Cost;
		uint	Num;
		uint	VisiblePos;
		uint	EnchantLevel;
		uint	BaodiCount;
		Sequence<uint, 3>	BaodiAttribute;

		void CopyFrom(RowData *pRowData)
		{
			this->EnchantID=pRowData->EnchantID;
			this->Pos=pRowData->Pos;
			this->Attribute=pRowData->Attribute;
			this->Cost=pRowData->Cost;
			this->Num=pRowData->Num;
			this->VisiblePos=pRowData->VisiblePos;
			this->EnchantLevel=pRowData->EnchantLevel;
			this->BaodiCount=pRowData->BaodiCount;
			this->BaodiAttribute=pRowData->BaodiAttribute;

		}
	};

	std::unordered_map<uint,int> EnchantID_Index;


    RowData *GetByEnchantID(const uint &key)
    {
        auto i = EnchantID_Index.find(key);
        return (i == EnchantID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EnchantID", "Pos", "Attribute", "Cost", "Num", "VisiblePos", "EnchantLevel", "BaodiCount", "BaodiAttribute",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EnchantEquip  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByEnchantID(pSrcRow->EnchantID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                EnchantID_Index[pNewRow->EnchantID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EnchantID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Pos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Attribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Cost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Num)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.VisiblePos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.EnchantLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.BaodiCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.BaodiAttribute)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		EnchantID_Index[row.EnchantID] = Table.size()-1;


		return true;
	}

	~EnchantEquip ()
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

		EnchantID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif