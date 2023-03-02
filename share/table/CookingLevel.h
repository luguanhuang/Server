#ifndef __CookingLevel_H__
#define __CookingLevel_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CookingLevel : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	CookLevel;
		uint	Experiences;

		void CopyFrom(RowData *pRowData)
		{
			this->CookLevel=pRowData->CookLevel;
			this->Experiences=pRowData->Experiences;

		}
	};

	std::unordered_map<uint,int> CookLevel_Index;
	std::unordered_map<uint,int> Experiences_Index;


    RowData *GetByCookLevel(const uint &key)
    {
        auto i = CookLevel_Index.find(key);
        return (i == CookLevel_Index.end()) ? NULL : Table[i->second];
    }
    
    RowData *GetByExperiences(const uint &key)
    {
        auto i = Experiences_Index.find(key);
        return (i == Experiences_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "CookLevel", "Experiences",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CookingLevel  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByCookLevel(pSrcRow->CookLevel);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                CookLevel_Index[pNewRow->CookLevel]=Table.size()-1;
                Experiences_Index[pNewRow->Experiences]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.CookLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Experiences)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		CookLevel_Index[row.CookLevel] = Table.size()-1;
		Experiences_Index[row.Experiences] = Table.size()-1;


		return true;
	}

	~CookingLevel ()
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

		CookLevel_Index.clear();
		Experiences_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif