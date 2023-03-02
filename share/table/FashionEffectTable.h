#ifndef __FashionEffectTable_H__
#define __FashionEffectTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/3 10:53:18


class FashionEffectTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Quality;
		vector<Sequence<uint, 2>>	Effect2;
		vector<Sequence<uint, 2>>	Effect3;
		vector<Sequence<uint, 2>>	Effect4;
		vector<Sequence<uint, 2>>	Effect5;
		vector<Sequence<uint, 2>>	Effect6;
		vector<Sequence<uint, 2>>	Effect7;
		bool	IsThreeSuit;

		void CopyFrom(RowData *pRowData)
		{
			this->Quality=pRowData->Quality;
			this->Effect2=pRowData->Effect2;
			this->Effect3=pRowData->Effect3;
			this->Effect4=pRowData->Effect4;
			this->Effect5=pRowData->Effect5;
			this->Effect6=pRowData->Effect6;
			this->Effect7=pRowData->Effect7;
			this->IsThreeSuit=pRowData->IsThreeSuit;

		}
	};

	std::unordered_map<uint,int> Quality_Index;


    RowData *GetByQuality(const uint &key)
    {
        auto i = Quality_Index.find(key);
        return (i == Quality_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Quality", "Effect2", "Effect3", "Effect4", "Effect5", "Effect6", "Effect7", "IsThreeSuit",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FashionEffectTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByQuality(pSrcRow->Quality);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Quality_Index[pNewRow->Quality]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Effect2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Effect3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Effect4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Effect5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Effect6)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Effect7)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.IsThreeSuit)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Quality_Index[row.Quality] = Table.size()-1;


		return true;
	}

	~FashionEffectTable ()
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

		Quality_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif