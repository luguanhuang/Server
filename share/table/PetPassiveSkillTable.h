#ifndef __PetPassiveSkillTable_H__
#define __PetPassiveSkillTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PetPassiveSkillTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		string	name;
		uint	quality;
		string	tips;
		vector<Sequence<uint, 2>>	attrs;
		uint	probability;
		string	Icon;
		string	Detail;
		vector<Sequence<uint, 2>>	attrlvl;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->name=pRowData->name;
			this->quality=pRowData->quality;
			this->tips=pRowData->tips;
			this->attrs=pRowData->attrs;
			this->probability=pRowData->probability;
			this->Icon=pRowData->Icon;
			this->Detail=pRowData->Detail;
			this->attrlvl=pRowData->attrlvl;

		}
	};

	std::unordered_map<uint,int> id_Index;


    RowData *GetByid(const uint &key)
    {
        auto i = id_Index.find(key);
        return (i == id_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "name", "quality", "tips", "attrs", "probability", "Icon", "Detail", "attrlvl",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PetPassiveSkillTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByid(pSrcRow->id);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                id_Index[pNewRow->id]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.tips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.attrs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.probability)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Detail)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.attrlvl)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~PetPassiveSkillTable ()
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

		id_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif