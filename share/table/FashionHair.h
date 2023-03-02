#ifndef __FashionHair_H__
#define __FashionHair_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/14 10:02:19


class FashionHair : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	HairID;
		uint	ProfessionID;
		uint	DefaultColorID;
		vector<uint>	UnLookColorID;
		vector<Sequence<uint, 3>>	Cost;

		void CopyFrom(RowData *pRowData)
		{
			this->HairID=pRowData->HairID;
			this->ProfessionID=pRowData->ProfessionID;
			this->DefaultColorID=pRowData->DefaultColorID;
			this->UnLookColorID=pRowData->UnLookColorID;
			this->Cost=pRowData->Cost;

		}
	};

	std::unordered_map<uint,int> HairID_Index;


    RowData *GetByHairID(const uint &key)
    {
        auto i = HairID_Index.find(key);
        return (i == HairID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "HairID", "ProfessionID", "DefaultColorID", "UnLookColorID", "Cost",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FashionHair  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByHairID(pSrcRow->HairID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                HairID_Index[pNewRow->HairID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.HairID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ProfessionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.DefaultColorID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.UnLookColorID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Cost)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		HairID_Index[row.HairID] = Table.size()-1;


		return true;
	}

	~FashionHair ()
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

		HairID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif