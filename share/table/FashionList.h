#ifndef __FashionList_H__
#define __FashionList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/28 16:57:50


class FashionList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ItemID;
		int	EquipPos;
		string	ModelPrefabWarrior;
		int	PresentID;
		string	ModelPrefabSorcer;
		string	ModelPrefabArcher;
		vector<int>	ReplaceID;
		string	ModelPrefabCleric;
		string	ModelPrefab5;
		string	ModelPrefab6;
		string	ModelPrefab7;
		string	ModelPrefab8;
		uint	Profession;
		vector<int>	AgainReplaceID;
		string	SuitName;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->EquipPos=pRowData->EquipPos;
			this->ModelPrefabWarrior=pRowData->ModelPrefabWarrior;
			this->PresentID=pRowData->PresentID;
			this->ModelPrefabSorcer=pRowData->ModelPrefabSorcer;
			this->ModelPrefabArcher=pRowData->ModelPrefabArcher;
			this->ReplaceID=pRowData->ReplaceID;
			this->ModelPrefabCleric=pRowData->ModelPrefabCleric;
			this->ModelPrefab5=pRowData->ModelPrefab5;
			this->ModelPrefab6=pRowData->ModelPrefab6;
			this->ModelPrefab7=pRowData->ModelPrefab7;
			this->ModelPrefab8=pRowData->ModelPrefab8;
			this->Profession=pRowData->Profession;
			this->AgainReplaceID=pRowData->AgainReplaceID;
			this->SuitName=pRowData->SuitName;

		}
	};

	std::unordered_map<int,int> ItemID_Index;


    RowData *GetByItemID(const int &key)
    {
        auto i = ItemID_Index.find(key);
        return (i == ItemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "EquipPos", "ModelPrefabWarrior", "PresentID", "ModelPrefabSorcer", "ModelPrefabArcher", "ReplaceID", "ModelPrefabCleric", "ModelPrefab5", "ModelPrefab6", "ModelPrefab7", "ModelPrefab8", "Profession", "AgainReplaceID", "SuitName",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FashionList  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByItemID(pSrcRow->ItemID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ItemID_Index[pNewRow->ItemID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.EquipPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ModelPrefabWarrior)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PresentID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ModelPrefabSorcer)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ModelPrefabArcher)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ReplaceID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ModelPrefabCleric)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ModelPrefab5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ModelPrefab6)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ModelPrefab7)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.ModelPrefab8)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.AgainReplaceID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.SuitName)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ItemID_Index[row.ItemID] = Table.size()-1;


		return true;
	}

	~FashionList ()
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

		ItemID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif