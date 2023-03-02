#ifndef __EffectDesTable_H__
#define __EffectDesTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/8 17:23:49


class EffectDesTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EffectID;
		string	EffectDes;
		vector<float>	ParamCoefficient;
		vector<string>	ColorDes;
		uint	BaseProf;
		uint	EffectType;

		void CopyFrom(RowData *pRowData)
		{
			this->EffectID=pRowData->EffectID;
			this->EffectDes=pRowData->EffectDes;
			this->ParamCoefficient=pRowData->ParamCoefficient;
			this->ColorDes=pRowData->ColorDes;
			this->BaseProf=pRowData->BaseProf;
			this->EffectType=pRowData->EffectType;

		}
	};

	std::unordered_map<uint,int> EffectID_Index;


    RowData *GetByEffectID(const uint &key)
    {
        auto i = EffectID_Index.find(key);
        return (i == EffectID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EffectID", "EffectDes", "ParamCoefficient", "ColorDes", "BaseProf", "EffectType",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EffectDesTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByEffectID(pSrcRow->EffectID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                EffectID_Index[pNewRow->EffectID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EffectID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.EffectDes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ParamCoefficient)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ColorDes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.BaseProf)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.EffectType)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		EffectID_Index[row.EffectID] = Table.size()-1;


		return true;
	}

	~EffectDesTable ()
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

		EffectID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif