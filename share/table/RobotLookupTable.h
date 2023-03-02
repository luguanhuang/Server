#ifndef __RobotLookupTable_H__
#define __RobotLookupTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RobotLookupTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	RobotLookupID;
		vector<int>	LookupRule;
		vector<int>	Level;
		vector<int>	CombatScore;
		int	Camp;
		int	Profession;
		int	Sex;
		uint	ExistSelectLimit;

		void CopyFrom(RowData *pRowData)
		{
			this->RobotLookupID=pRowData->RobotLookupID;
			this->LookupRule=pRowData->LookupRule;
			this->Level=pRowData->Level;
			this->CombatScore=pRowData->CombatScore;
			this->Camp=pRowData->Camp;
			this->Profession=pRowData->Profession;
			this->Sex=pRowData->Sex;
			this->ExistSelectLimit=pRowData->ExistSelectLimit;

		}
	};

	std::unordered_map<int,int> RobotLookupID_Index;


    RowData *GetByRobotLookupID(const int &key)
    {
        auto i = RobotLookupID_Index.find(key);
        return (i == RobotLookupID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "RobotLookupID", "LookupRule", "Level", "CombatScore", "Camp", "Profession", "Sex", "ExistSelectLimit",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RobotLookupTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByRobotLookupID(pSrcRow->RobotLookupID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                RobotLookupID_Index[pNewRow->RobotLookupID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.RobotLookupID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.LookupRule)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.CombatScore)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Camp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Sex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ExistSelectLimit)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		RobotLookupID_Index[row.RobotLookupID] = Table.size()-1;


		return true;
	}

	~RobotLookupTable ()
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

		RobotLookupID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif