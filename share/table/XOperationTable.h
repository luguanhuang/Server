#ifndef __XOperationTable_H__
#define __XOperationTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class XOperationTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		float	Angle;
		float	Distance;
		bool	Vertical;
		bool	Horizontal;
		float	MaxV;
		float	MinV;
		bool	OffSolo;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Angle=pRowData->Angle;
			this->Distance=pRowData->Distance;
			this->Vertical=pRowData->Vertical;
			this->Horizontal=pRowData->Horizontal;
			this->MaxV=pRowData->MaxV;
			this->MinV=pRowData->MinV;
			this->OffSolo=pRowData->OffSolo;

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
		const char *Headers[] = { "ID", "Angle", "Distance", "Vertical", "Horizontal", "MaxV", "MinV", "OffSolo",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(XOperationTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Angle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Distance)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Vertical)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Horizontal)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.MaxV)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MinV)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.OffSolo)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~XOperationTable ()
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