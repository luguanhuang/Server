#ifndef __PartnerWelfare_H__
#define __PartnerWelfare_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PartnerWelfare : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Id;
		string	TittleTxt;
		string	ContentTxt;
		string	Pic;

		void CopyFrom(RowData *pRowData)
		{
			this->Id=pRowData->Id;
			this->TittleTxt=pRowData->TittleTxt;
			this->ContentTxt=pRowData->ContentTxt;
			this->Pic=pRowData->Pic;

		}
	};

	std::unordered_map<uint,int> Id_Index;


    RowData *GetById(const uint &key)
    {
        auto i = Id_Index.find(key);
        return (i == Id_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Id", "TittleTxt", "ContentTxt", "Pic",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PartnerWelfare  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetById(pSrcRow->Id);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Id_Index[pNewRow->Id]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TittleTxt)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ContentTxt)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Pic)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Id_Index[row.Id] = Table.size()-1;


		return true;
	}

	~PartnerWelfare ()
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

		Id_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif