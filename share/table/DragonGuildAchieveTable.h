#ifndef __DragonGuildAchieveTable_H__
#define __DragonGuildAchieveTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/21 16:04:26


class DragonGuildAchieveTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		uint	Type;
		string	name;
		string	description;
		string	icon;
		uint	SceneID;
		uint	count;
		uint	guildExp;
		vector<uint>	dropID;
		vector<Sequence<uint, 2>>	viewabledrop;
		uint	chestCount;
		uint	value;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Type=pRowData->Type;
			this->name=pRowData->name;
			this->description=pRowData->description;
			this->icon=pRowData->icon;
			this->SceneID=pRowData->SceneID;
			this->count=pRowData->count;
			this->guildExp=pRowData->guildExp;
			this->dropID=pRowData->dropID;
			this->viewabledrop=pRowData->viewabledrop;
			this->chestCount=pRowData->chestCount;
			this->value=pRowData->value;

		}
	};

	std::unordered_map<uint,int> ID_Index;


    RowData *GetByID(const uint &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Type", "name", "description", "icon", "SceneID", "count", "guildExp", "dropID", "viewabledrop", "chestCount", "value",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DragonGuildAchieveTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.count)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.guildExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.dropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.viewabledrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.chestCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.value)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~DragonGuildAchieveTable ()
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