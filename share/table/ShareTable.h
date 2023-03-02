#ifndef __ShareTable_H__
#define __ShareTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ShareTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Condition;
		string	Desc;
		string	Title;
		string	Icon;
		string	Link;
		uint	type;
		vector<int>	param;

		void CopyFrom(RowData *pRowData)
		{
			this->Condition=pRowData->Condition;
			this->Desc=pRowData->Desc;
			this->Title=pRowData->Title;
			this->Icon=pRowData->Icon;
			this->Link=pRowData->Link;
			this->type=pRowData->type;
			this->param=pRowData->param;

		}
	};

	std::unordered_map<int,int> Condition_Index;


    RowData *GetByCondition(const int &key)
    {
        auto i = Condition_Index.find(key);
        return (i == Condition_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Condition", "Desc", "Title", "Icon", "Link", "type", "param",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ShareTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByCondition(pSrcRow->Condition);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Condition_Index[pNewRow->Condition]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Condition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Title)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Link)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.param)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Condition_Index[row.Condition] = Table.size()-1;


		return true;
	}

	~ShareTable ()
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

		Condition_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif