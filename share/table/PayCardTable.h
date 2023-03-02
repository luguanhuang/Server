#ifndef __PayCardTable_H__
#define __PayCardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PayCardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	ParamID;
		int	Price;
		int	Diamond;
		int	Type;
		int	DayAward;
		string	Name;
		string	Icon;
		string	Effect;
		string	Effect2;
		string	ServiceCode;

		void CopyFrom(RowData *pRowData)
		{
			this->ParamID=pRowData->ParamID;
			this->Price=pRowData->Price;
			this->Diamond=pRowData->Diamond;
			this->Type=pRowData->Type;
			this->DayAward=pRowData->DayAward;
			this->Name=pRowData->Name;
			this->Icon=pRowData->Icon;
			this->Effect=pRowData->Effect;
			this->Effect2=pRowData->Effect2;
			this->ServiceCode=pRowData->ServiceCode;

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
		const char *Headers[] = { "ParamID", "Price", "Diamond", "Type", "DayAward", "Name", "Icon", "Effect", "Effect2", "ServiceCode",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PayCardTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[3]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DayAward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Effect)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Effect2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ServiceCode)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ParamID_Index[row.ParamID] = Table.size()-1;


		return true;
	}

	~PayCardTable ()
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