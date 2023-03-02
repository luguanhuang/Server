#ifndef __ReinforceTable_H__
#define __ReinforceTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ReinforceTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ReinforceLv;
		vector<Sequence<uint, 2>>	ReinforceCost;
		vector<Sequence<uint, 2>>	ReinforceT;
		vector<Sequence<uint, 2>>	ReinforceY;
		vector<Sequence<uint, 2>>	ReinforceK;
		vector<Sequence<uint, 2>>	ReinforceS;
		vector<Sequence<uint, 2>>	ReinforceX;
		vector<Sequence<uint, 2>>	ReinforceW;
		vector<Sequence<uint, 2>>	ReinforceF;
		vector<Sequence<uint, 2>>	ReinforceL;
		vector<Sequence<uint, 2>>	ReinforceE;
		vector<Sequence<uint, 2>>	ReinforceJ;
		vector<Sequence<uint, 2>>	ReinforceDecompose;
		bool	Special;

		void CopyFrom(RowData *pRowData)
		{
			this->ReinforceLv=pRowData->ReinforceLv;
			this->ReinforceCost=pRowData->ReinforceCost;
			this->ReinforceT=pRowData->ReinforceT;
			this->ReinforceY=pRowData->ReinforceY;
			this->ReinforceK=pRowData->ReinforceK;
			this->ReinforceS=pRowData->ReinforceS;
			this->ReinforceX=pRowData->ReinforceX;
			this->ReinforceW=pRowData->ReinforceW;
			this->ReinforceF=pRowData->ReinforceF;
			this->ReinforceL=pRowData->ReinforceL;
			this->ReinforceE=pRowData->ReinforceE;
			this->ReinforceJ=pRowData->ReinforceJ;
			this->ReinforceDecompose=pRowData->ReinforceDecompose;
			this->Special=pRowData->Special;

		}
	};

	std::unordered_map<uint,int> ReinforceLv_Index;


    RowData *GetByReinforceLv(const uint &key)
    {
        auto i = ReinforceLv_Index.find(key);
        return (i == ReinforceLv_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ReinforceLv", "ReinforceCost", "ReinforceT", "ReinforceY", "ReinforceK", "ReinforceS", "ReinforceX", "ReinforceW", "ReinforceF", "ReinforceL", "ReinforceE", "ReinforceJ", "ReinforceDecompose", "Special",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ReinforceTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByReinforceLv(pSrcRow->ReinforceLv);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ReinforceLv_Index[pNewRow->ReinforceLv]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ReinforceLv)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ReinforceCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ReinforceT)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ReinforceY)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ReinforceK)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ReinforceS)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ReinforceX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ReinforceW)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ReinforceF)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ReinforceL)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ReinforceE)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.ReinforceJ)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.ReinforceDecompose)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.Special)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ReinforceLv_Index[row.ReinforceLv] = Table.size()-1;


		return true;
	}

	~ReinforceTable ()
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

		ReinforceLv_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif