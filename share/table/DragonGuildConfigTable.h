#ifndef __DragonGuildConfigTable_H__
#define __DragonGuildConfigTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/10 11:59:13


class DragonGuildConfigTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	DragonGuildLevel;
		uint	DragonGuildExpNeed;
		uint	DragonGuildNumber;
		uint	PresidentNum;
		uint	VicePresidentNum;

		void CopyFrom(RowData *pRowData)
		{
			this->DragonGuildLevel=pRowData->DragonGuildLevel;
			this->DragonGuildExpNeed=pRowData->DragonGuildExpNeed;
			this->DragonGuildNumber=pRowData->DragonGuildNumber;
			this->PresidentNum=pRowData->PresidentNum;
			this->VicePresidentNum=pRowData->VicePresidentNum;

		}
	};

	std::unordered_map<uint,int> DragonGuildLevel_Index;


    RowData *GetByDragonGuildLevel(const uint &key)
    {
        auto i = DragonGuildLevel_Index.find(key);
        return (i == DragonGuildLevel_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "DragonGuildLevel", "DragonGuildExpNeed", "DragonGuildNumber", "PresidentNum", "VicePresidentNum",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DragonGuildConfigTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByDragonGuildLevel(pSrcRow->DragonGuildLevel);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                DragonGuildLevel_Index[pNewRow->DragonGuildLevel]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.DragonGuildLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.DragonGuildExpNeed)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.DragonGuildNumber)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PresidentNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.VicePresidentNum)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		DragonGuildLevel_Index[row.DragonGuildLevel] = Table.size()-1;


		return true;
	}

	~DragonGuildConfigTable ()
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

		DragonGuildLevel_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif