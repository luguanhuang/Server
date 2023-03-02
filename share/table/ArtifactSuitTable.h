﻿#ifndef __ArtifactSuitTable_H__
#define __ArtifactSuitTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/4 15:14:58


class ArtifactSuitTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ArtifactSuitID;
		uint	Level;
		string	Name;
		vector<Sequence<uint, 2>>	Effect2;
		vector<Sequence<uint, 2>>	Effect3;
		vector<Sequence<uint, 2>>	Effect4;
		vector<Sequence<uint, 2>>	Effect5;
		vector<Sequence<uint, 2>>	Effect6;

		void CopyFrom(RowData *pRowData)
		{
			this->ArtifactSuitID=pRowData->ArtifactSuitID;
			this->Level=pRowData->Level;
			this->Name=pRowData->Name;
			this->Effect2=pRowData->Effect2;
			this->Effect3=pRowData->Effect3;
			this->Effect4=pRowData->Effect4;
			this->Effect5=pRowData->Effect5;
			this->Effect6=pRowData->Effect6;

		}
	};

	std::unordered_map<uint,int> ArtifactSuitID_Index;


    RowData *GetByArtifactSuitID(const uint &key)
    {
        auto i = ArtifactSuitID_Index.find(key);
        return (i == ArtifactSuitID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ArtifactSuitID", "Level", "Name", "Effect2", "Effect3", "Effect4", "Effect5", "Effect6",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ArtifactSuitTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByArtifactSuitID(pSrcRow->ArtifactSuitID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ArtifactSuitID_Index[pNewRow->ArtifactSuitID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ArtifactSuitID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Effect2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Effect3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Effect4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Effect5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Effect6)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ArtifactSuitID_Index[row.ArtifactSuitID] = Table.size()-1;


		return true;
	}

	~ArtifactSuitTable ()
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

		ArtifactSuitID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif