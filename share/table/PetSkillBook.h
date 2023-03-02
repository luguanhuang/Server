#ifndef __PetSkillBook_H__
#define __PetSkillBook_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PetSkillBook : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	itemid;
		uint	skillid;
		vector<Sequence<uint, 2>>	pro;
		string	Description;

		void CopyFrom(RowData *pRowData)
		{
			this->itemid=pRowData->itemid;
			this->skillid=pRowData->skillid;
			this->pro=pRowData->pro;
			this->Description=pRowData->Description;

		}
	};

	std::unordered_map<uint,int> itemid_Index;


    RowData *GetByitemid(const uint &key)
    {
        auto i = itemid_Index.find(key);
        return (i == itemid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "itemid", "skillid", "pro", "Description",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PetSkillBook  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByitemid(pSrcRow->itemid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                itemid_Index[pNewRow->itemid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.itemid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.skillid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.pro)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Description)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		itemid_Index[row.itemid] = Table.size()-1;


		return true;
	}

	~PetSkillBook ()
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

		itemid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif