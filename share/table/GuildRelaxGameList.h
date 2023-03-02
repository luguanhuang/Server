#ifndef __GuildRelaxGameList_H__
#define __GuildRelaxGameList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildRelaxGameList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	GameBg;
		string	GameTitle;
		string	GameName;
		int	ModuleID;

		void CopyFrom(RowData *pRowData)
		{
			this->GameBg=pRowData->GameBg;
			this->GameTitle=pRowData->GameTitle;
			this->GameName=pRowData->GameName;
			this->ModuleID=pRowData->ModuleID;

		}
	};

	std::unordered_map<int,int> ModuleID_Index;


    RowData *GetByModuleID(const int &key)
    {
        auto i = ModuleID_Index.find(key);
        return (i == ModuleID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GameBg", "GameTitle", "GameName", "ModuleID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildRelaxGameList  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByModuleID(pSrcRow->ModuleID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ModuleID_Index[pNewRow->ModuleID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.GameBg)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.GameTitle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.GameName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ModuleID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ModuleID_Index[row.ModuleID] = Table.size()-1;


		return true;
	}

	~GuildRelaxGameList ()
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

		ModuleID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif