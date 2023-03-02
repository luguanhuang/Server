#ifndef __CustomBattleType_H__
#define __CustomBattleType_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/11 16:43:46


class CustomBattleType : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	typeid;
		string	name;
		string	show;
		bool	notopen;

		void CopyFrom(RowData *pRowData)
		{
			this->typeid=pRowData->typeid;
			this->name=pRowData->name;
			this->show=pRowData->show;
			this->notopen=pRowData->notopen;

		}
	};

	std::unordered_map<int,int> typeid_Index;


    RowData *GetBytypeid(const int &key)
    {
        auto i = typeid_Index.find(key);
        return (i == typeid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "typeid", "name", "show", "notopen",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CustomBattleType  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBytypeid(pSrcRow->typeid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                typeid_Index[pNewRow->typeid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.typeid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.show)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.notopen)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		typeid_Index[row.typeid] = Table.size()-1;


		return true;
	}

	~CustomBattleType ()
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

		typeid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif