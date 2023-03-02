#ifndef __AbyssPartyTypeTable_H__
#define __AbyssPartyTypeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/11 14:46:09


class AbyssPartyTypeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	AbyssPartyId;
		string	Name;
		string	Bg;
		string	Icon;
		int	OpenLevel;
		string	SugLevel;
		int	PandoraID;
		vector<int>	TitanItemID;

		void CopyFrom(RowData *pRowData)
		{
			this->AbyssPartyId=pRowData->AbyssPartyId;
			this->Name=pRowData->Name;
			this->Bg=pRowData->Bg;
			this->Icon=pRowData->Icon;
			this->OpenLevel=pRowData->OpenLevel;
			this->SugLevel=pRowData->SugLevel;
			this->PandoraID=pRowData->PandoraID;
			this->TitanItemID=pRowData->TitanItemID;

		}
	};

	std::unordered_map<int,int> AbyssPartyId_Index;


    RowData *GetByAbyssPartyId(const int &key)
    {
        auto i = AbyssPartyId_Index.find(key);
        return (i == AbyssPartyId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "AbyssPartyId", "Name", "Bg", "Icon", "OpenLevel", "SugLevel", "PandoraID", "TitanItemID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AbyssPartyTypeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByAbyssPartyId(pSrcRow->AbyssPartyId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                AbyssPartyId_Index[pNewRow->AbyssPartyId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.AbyssPartyId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Bg)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.OpenLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SugLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.PandoraID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.TitanItemID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		AbyssPartyId_Index[row.AbyssPartyId] = Table.size()-1;


		return true;
	}

	~AbyssPartyTypeTable ()
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

		AbyssPartyId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif