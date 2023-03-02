#ifndef __PayListTable_H__
#define __PayListTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PayListTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	ParamID;
		int	Price;
		int	Diamond;
		int	ExtDiamond;
		string	Name;
		string	Icon;
		string	Effect;
		string	Effect2;

		void CopyFrom(RowData *pRowData)
		{
			this->ParamID=pRowData->ParamID;
			this->Price=pRowData->Price;
			this->Diamond=pRowData->Diamond;
			this->ExtDiamond=pRowData->ExtDiamond;
			this->Name=pRowData->Name;
			this->Icon=pRowData->Icon;
			this->Effect=pRowData->Effect;
			this->Effect2=pRowData->Effect2;

		}
	};

	std::unordered_map<string,int> ParamID_Index;


    RowData *GetByParamID(const string &key)
    {
        auto i = ParamID_Index.find(key);
        return (i == ParamID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ParamID", "Price", "Diamond", "ExtDiamond", "Name", "Icon", "Effect", "Effect2",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PayListTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByParamID(pSrcRow->ParamID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ParamID_Index[pNewRow->ParamID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ParamID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Price)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Diamond)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ExtDiamond)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Effect)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Effect2)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ParamID_Index[row.ParamID] = Table.size()-1;


		return true;
	}

	~PayListTable ()
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

		ParamID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif