#ifndef __GardenBanquetCfg_H__
#define __GardenBanquetCfg_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GardenBanquetCfg : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	BanquetID;
		vector<Sequence<uint, 2>>	Stuffs;
		Sequence<uint, 2>	StuffBuffID;
		vector<Sequence<uint, 2>>	BanquetAwards;
		string	BanquetName;
		uint	VoiceOver1Duration;
		uint	VoiceOver2Duration;
		uint	VoiceOver3Duration;
		uint	VoiceOver4Duration;
		string	VoiceOver1;
		string	VoiceOver2;
		string	VoiceOver3;
		string	VoiceOver4;
		string	Desc;

		void CopyFrom(RowData *pRowData)
		{
			this->BanquetID=pRowData->BanquetID;
			this->Stuffs=pRowData->Stuffs;
			this->StuffBuffID=pRowData->StuffBuffID;
			this->BanquetAwards=pRowData->BanquetAwards;
			this->BanquetName=pRowData->BanquetName;
			this->VoiceOver1Duration=pRowData->VoiceOver1Duration;
			this->VoiceOver2Duration=pRowData->VoiceOver2Duration;
			this->VoiceOver3Duration=pRowData->VoiceOver3Duration;
			this->VoiceOver4Duration=pRowData->VoiceOver4Duration;
			this->VoiceOver1=pRowData->VoiceOver1;
			this->VoiceOver2=pRowData->VoiceOver2;
			this->VoiceOver3=pRowData->VoiceOver3;
			this->VoiceOver4=pRowData->VoiceOver4;
			this->Desc=pRowData->Desc;

		}
	};

	std::unordered_map<uint,int> BanquetID_Index;


    RowData *GetByBanquetID(const uint &key)
    {
        auto i = BanquetID_Index.find(key);
        return (i == BanquetID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BanquetID", "Stuffs", "StuffBuffID", "BanquetAwards", "BanquetName", "VoiceOver1Duration", "VoiceOver2Duration", "VoiceOver3Duration", "VoiceOver4Duration", "VoiceOver1", "VoiceOver2", "VoiceOver3", "VoiceOver4", "Desc",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GardenBanquetCfg  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByBanquetID(pSrcRow->BanquetID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                BanquetID_Index[pNewRow->BanquetID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.BanquetID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Stuffs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.StuffBuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.BanquetAwards)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.BanquetName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.VoiceOver1Duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.VoiceOver2Duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.VoiceOver3Duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.VoiceOver4Duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.VoiceOver1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.VoiceOver2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.VoiceOver3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.VoiceOver4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.Desc)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		BanquetID_Index[row.BanquetID] = Table.size()-1;


		return true;
	}

	~GardenBanquetCfg ()
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

		BanquetID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif