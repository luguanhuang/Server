#ifndef __FashionSuitSpecialEffects_H__
#define __FashionSuitSpecialEffects_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/18 10:52:22


class FashionSuitSpecialEffects : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	suitid;
		uint	specialeffectsid;
		string	Fx1;
		string	Fx2;
		string	Fx3;
		string	Fx4;
		string	Fx5;
		string	Fx6;
		string	Fx7;
		string	Fx8;
		string	Name;
		string	Icon;
		vector<uint>	FashionList;

		void CopyFrom(RowData *pRowData)
		{
			this->suitid=pRowData->suitid;
			this->specialeffectsid=pRowData->specialeffectsid;
			this->Fx1=pRowData->Fx1;
			this->Fx2=pRowData->Fx2;
			this->Fx3=pRowData->Fx3;
			this->Fx4=pRowData->Fx4;
			this->Fx5=pRowData->Fx5;
			this->Fx6=pRowData->Fx6;
			this->Fx7=pRowData->Fx7;
			this->Fx8=pRowData->Fx8;
			this->Name=pRowData->Name;
			this->Icon=pRowData->Icon;
			this->FashionList=pRowData->FashionList;

		}
	};

	std::unordered_map<uint,int> suitid_Index;


    RowData *GetBysuitid(const uint &key)
    {
        auto i = suitid_Index.find(key);
        return (i == suitid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "suitid", "specialeffectsid", "Fx1", "Fx2", "Fx3", "Fx4", "Fx5", "Fx6", "Fx7", "Fx8", "Name", "Icon", "FashionList",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FashionSuitSpecialEffects  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBysuitid(pSrcRow->suitid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                suitid_Index[pNewRow->suitid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.suitid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.specialeffectsid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Fx1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Fx2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Fx3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Fx4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Fx5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Fx6)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Fx7)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Fx8)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.FashionList)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		suitid_Index[row.suitid] = Table.size()-1;


		return true;
	}

	~FashionSuitSpecialEffects ()
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

		suitid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif