#ifndef __AbyssPartyListTable_H__
#define __AbyssPartyListTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/6 15:20:03


class AbyssPartyListTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	AbyssPartyId;
		int	Index;
		string	Name;
		string	Icon;
		Sequence<int, 2>	Cost;
		uint	SugPPT;
		int	RandomID;
		int	ID;

		void CopyFrom(RowData *pRowData)
		{
			this->AbyssPartyId=pRowData->AbyssPartyId;
			this->Index=pRowData->Index;
			this->Name=pRowData->Name;
			this->Icon=pRowData->Icon;
			this->Cost=pRowData->Cost;
			this->SugPPT=pRowData->SugPPT;
			this->RandomID=pRowData->RandomID;
			this->ID=pRowData->ID;

		}
	};

	std::unordered_map<int,int> ID_Index;


    RowData *GetByID(const int &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "AbyssPartyId", "Index", "Name", "Icon", "Cost", "SugPPT", "RandomID", "ID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AbyssPartyListTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByID(pSrcRow->ID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ID_Index[pNewRow->ID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.AbyssPartyId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Index)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Cost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SugPPT)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.RandomID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~AbyssPartyListTable ()
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

		ID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif