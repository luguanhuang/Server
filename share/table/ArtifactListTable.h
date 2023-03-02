#ifndef __ArtifactListTable_H__
#define __ArtifactListTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/3 16:23:24


class ArtifactListTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ArtifactID;
		uint	ArtifactPos;
		uint	ArtifactSuit;
		vector<Sequence<uint, 4>>	Attributes1;
		vector<Sequence<uint, 4>>	Attributes2;
		vector<Sequence<uint, 4>>	Attributes3;
		vector<uint>	EffectRandomIDs;
		uint	EffectNum;
		vector<uint>	ProfessionIDs;
		string	EffectDes;
		uint	AttrType;
		uint	IsCanRecast;
		vector<Sequence<uint, 2>>	RecastMaterials;
		uint	IsCanFuse;
		vector<Sequence<uint, 2>>	FuseMaterials;
		Sequence<uint, 2>	FuseSucRate;
		uint	FuseSucRateUseStone;
		uint	IsCanInscription;
		uint	InscriptionMaxNum;
		uint	InscriptionAddNewRate;

		void CopyFrom(RowData *pRowData)
		{
			this->ArtifactID=pRowData->ArtifactID;
			this->ArtifactPos=pRowData->ArtifactPos;
			this->ArtifactSuit=pRowData->ArtifactSuit;
			this->Attributes1=pRowData->Attributes1;
			this->Attributes2=pRowData->Attributes2;
			this->Attributes3=pRowData->Attributes3;
			this->EffectRandomIDs=pRowData->EffectRandomIDs;
			this->EffectNum=pRowData->EffectNum;
			this->ProfessionIDs=pRowData->ProfessionIDs;
			this->EffectDes=pRowData->EffectDes;
			this->AttrType=pRowData->AttrType;
			this->IsCanRecast=pRowData->IsCanRecast;
			this->RecastMaterials=pRowData->RecastMaterials;
			this->IsCanFuse=pRowData->IsCanFuse;
			this->FuseMaterials=pRowData->FuseMaterials;
			this->FuseSucRate=pRowData->FuseSucRate;
			this->FuseSucRateUseStone=pRowData->FuseSucRateUseStone;
			this->IsCanInscription=pRowData->IsCanInscription;
			this->InscriptionMaxNum=pRowData->InscriptionMaxNum;
			this->InscriptionAddNewRate=pRowData->InscriptionAddNewRate;

		}
	};

	std::unordered_map<uint,int> ArtifactID_Index;


    RowData *GetByArtifactID(const uint &key)
    {
        auto i = ArtifactID_Index.find(key);
        return (i == ArtifactID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ArtifactID", "ArtifactPos", "ArtifactSuit", "Attributes1", "Attributes2", "Attributes3", "EffectRandomIDs", "EffectNum", "ProfessionIDs", "EffectDes", "AttrType", "IsCanRecast", "RecastMaterials", "IsCanFuse", "FuseMaterials", "FuseSucRate", "FuseSucRateUseStone", "IsCanInscription", "InscriptionMaxNum", "InscriptionAddNewRate",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ArtifactListTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByArtifactID(pSrcRow->ArtifactID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ArtifactID_Index[pNewRow->ArtifactID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ArtifactID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ArtifactPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ArtifactSuit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Attributes1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Attributes2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Attributes3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.EffectRandomIDs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.EffectNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ProfessionIDs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.EffectDes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.AttrType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.IsCanRecast)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.RecastMaterials)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.IsCanFuse)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.FuseMaterials)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.FuseSucRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.FuseSucRateUseStone)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.IsCanInscription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.InscriptionMaxNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.InscriptionAddNewRate)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ArtifactID_Index[row.ArtifactID] = Table.size()-1;


		return true;
	}

	~ArtifactListTable ()
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

		ArtifactID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif