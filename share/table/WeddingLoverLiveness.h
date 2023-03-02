#ifndef __WeddingLoverLiveness_H__
#define __WeddingLoverLiveness_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/28 20:53:52


class WeddingLoverLiveness : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	index;
		uint	liveness;
		vector<uint>	dropid;
		string	dropdesc;
		vector<Sequence<uint, 2>>	viewabledrop;
		string	boxPic;

		void CopyFrom(RowData *pRowData)
		{
			this->index=pRowData->index;
			this->liveness=pRowData->liveness;
			this->dropid=pRowData->dropid;
			this->dropdesc=pRowData->dropdesc;
			this->viewabledrop=pRowData->viewabledrop;
			this->boxPic=pRowData->boxPic;

		}
	};

	std::unordered_map<uint,int> index_Index;


    RowData *GetByindex(const uint &key)
    {
        auto i = index_Index.find(key);
        return (i == index_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "index", "liveness", "dropid", "dropdesc", "viewabledrop", "boxPic",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(WeddingLoverLiveness  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByindex(pSrcRow->index);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                index_Index[pNewRow->index]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.index)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.liveness)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.dropid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.dropdesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.viewabledrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.boxPic)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		index_Index[row.index] = Table.size()-1;


		return true;
	}

	~WeddingLoverLiveness ()
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

		index_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif