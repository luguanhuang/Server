#ifndef __DesignationTable_H__
#define __DesignationTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/31 21:29:04


class DesignationTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		string	Designation;
		int	Type;
		string	Explanation;
		int	CompleteType;
		vector<int>	CompleteValue;
		int	Pragmaticality;
		string	Effect;
		vector<Sequence<uint, 2>>	Attribute;
		string	Color;
		int	GainShowIcon;
		int	SortID;
		Sequence<int, 2>	Level;
		bool	ShowInChat;
		int	Channel;
		string	Atlas;
		bool	Special;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Designation=pRowData->Designation;
			this->Type=pRowData->Type;
			this->Explanation=pRowData->Explanation;
			this->CompleteType=pRowData->CompleteType;
			this->CompleteValue=pRowData->CompleteValue;
			this->Pragmaticality=pRowData->Pragmaticality;
			this->Effect=pRowData->Effect;
			this->Attribute=pRowData->Attribute;
			this->Color=pRowData->Color;
			this->GainShowIcon=pRowData->GainShowIcon;
			this->SortID=pRowData->SortID;
			this->Level=pRowData->Level;
			this->ShowInChat=pRowData->ShowInChat;
			this->Channel=pRowData->Channel;
			this->Atlas=pRowData->Atlas;
			this->Special=pRowData->Special;

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
		const char *Headers[] = { "ID", "Designation", "Type", "Explanation", "CompleteType", "CompleteValue", "Pragmaticality", "Effect", "Attribute", "Color", "GainShowIcon", "SortID", "Level", "ShowInChat", "Channel", "Atlas", "Special",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DesignationTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Designation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Explanation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.CompleteType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.CompleteValue)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Pragmaticality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Effect)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Attribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Color)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.GainShowIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.SortID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.ShowInChat)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.Channel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.Atlas)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.Special)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~DesignationTable ()
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