#ifndef __FishInfo_H__
#define __FishInfo_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/26 17:27:05


class FishInfo : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	FishID;
		uint	Experience;
		int	quality;
		bool	isbind;
		bool	ShowInLevel;

		void CopyFrom(RowData *pRowData)
		{
			this->FishID=pRowData->FishID;
			this->Experience=pRowData->Experience;
			this->quality=pRowData->quality;
			this->isbind=pRowData->isbind;
			this->ShowInLevel=pRowData->ShowInLevel;

		}
	};

	std::unordered_map<uint,int> FishID_Index;


    RowData *GetByFishID(const uint &key)
    {
        auto i = FishID_Index.find(key);
        return (i == FishID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "FishID", "Experience", "quality", "isbind", "ShowInLevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FishInfo  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByFishID(pSrcRow->FishID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                FishID_Index[pNewRow->FishID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.FishID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Experience)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.isbind)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ShowInLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		FishID_Index[row.FishID] = Table.size()-1;


		return true;
	}

	~FishInfo ()
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

		FishID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif