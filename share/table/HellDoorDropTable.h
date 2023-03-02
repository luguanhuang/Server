#ifndef __HellDoorDropTable_H__
#define __HellDoorDropTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/7 11:02:48


class HellDoorDropTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	HellDoorDropID;
		uint	ProfID;
		vector<uint>	CommonDropID;
		vector<uint>	WorseDropID;
		vector<uint>	WorstDropID;
		Sequence<uint, 2>	EnterWorseDropTimes;
		Sequence<uint, 2>	EnterWorstDropTimes;

		void CopyFrom(RowData *pRowData)
		{
			this->HellDoorDropID=pRowData->HellDoorDropID;
			this->ProfID=pRowData->ProfID;
			this->CommonDropID=pRowData->CommonDropID;
			this->WorseDropID=pRowData->WorseDropID;
			this->WorstDropID=pRowData->WorstDropID;
			this->EnterWorseDropTimes=pRowData->EnterWorseDropTimes;
			this->EnterWorstDropTimes=pRowData->EnterWorstDropTimes;

		}
	};

	std::unordered_map<uint,int> HellDoorDropID_Index;


    RowData *GetByHellDoorDropID(const uint &key)
    {
        auto i = HellDoorDropID_Index.find(key);
        return (i == HellDoorDropID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "HellDoorDropID", "ProfID", "CommonDropID", "WorseDropID", "WorstDropID", "EnterWorseDropTimes", "EnterWorstDropTimes",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(HellDoorDropTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByHellDoorDropID(pSrcRow->HellDoorDropID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                HellDoorDropID_Index[pNewRow->HellDoorDropID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.HellDoorDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.CommonDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.WorseDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.WorstDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.EnterWorseDropTimes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.EnterWorstDropTimes)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		HellDoorDropID_Index[row.HellDoorDropID] = Table.size()-1;


		return true;
	}

	~HellDoorDropTable ()
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

		HellDoorDropID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif