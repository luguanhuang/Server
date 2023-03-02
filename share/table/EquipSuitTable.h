#ifndef __EquipSuitTable_H__
#define __EquipSuitTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class EquipSuitTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	SuitID;
		string	SuitName;
		int	SuitQuality;
		vector<int>	EquipID;
		Sequence<float, 2>	Effect1;
		Sequence<float, 2>	Effect2;
		Sequence<float, 2>	Effect3;
		Sequence<float, 2>	Effect4;
		Sequence<float, 2>	Effect5;
		Sequence<float, 2>	Effect6;
		Sequence<float, 2>	Effect7;
		Sequence<float, 2>	Effect8;
		Sequence<float, 2>	Effect9;
		Sequence<float, 2>	Effect10;
		int	ProfID;
		int	Level;
		bool	IsCreateShow;

		void CopyFrom(RowData *pRowData)
		{
			this->SuitID=pRowData->SuitID;
			this->SuitName=pRowData->SuitName;
			this->SuitQuality=pRowData->SuitQuality;
			this->EquipID=pRowData->EquipID;
			this->Effect1=pRowData->Effect1;
			this->Effect2=pRowData->Effect2;
			this->Effect3=pRowData->Effect3;
			this->Effect4=pRowData->Effect4;
			this->Effect5=pRowData->Effect5;
			this->Effect6=pRowData->Effect6;
			this->Effect7=pRowData->Effect7;
			this->Effect8=pRowData->Effect8;
			this->Effect9=pRowData->Effect9;
			this->Effect10=pRowData->Effect10;
			this->ProfID=pRowData->ProfID;
			this->Level=pRowData->Level;
			this->IsCreateShow=pRowData->IsCreateShow;

		}
	};

	std::unordered_map<int,int> SuitID_Index;


    RowData *GetBySuitID(const int &key)
    {
        auto i = SuitID_Index.find(key);
        return (i == SuitID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SuitID", "SuitName", "SuitQuality", "EquipID", "Effect1", "Effect2", "Effect3", "Effect4", "Effect5", "Effect6", "Effect7", "Effect8", "Effect9", "Effect10", "ProfID", "Level", "IsCreateShow",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EquipSuitTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySuitID(pSrcRow->SuitID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SuitID_Index[pNewRow->SuitID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SuitID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SuitName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SuitQuality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.EquipID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Effect1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Effect2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Effect3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Effect4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Effect5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Effect6)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Effect7)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Effect8)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Effect9)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.Effect10)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.IsCreateShow)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SuitID_Index[row.SuitID] = Table.size()-1;


		return true;
	}

	~EquipSuitTable ()
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

		SuitID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif