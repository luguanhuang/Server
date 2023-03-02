#ifndef __JadeTransferTable_H__
#define __JadeTransferTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class JadeTransferTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	JadeID;
		vector<uint>	TransferID;

		void CopyFrom(RowData *pRowData)
		{
			this->JadeID=pRowData->JadeID;
			this->TransferID=pRowData->TransferID;

		}
	};

	std::unordered_map<uint,int> JadeID_Index;


    RowData *GetByJadeID(const uint &key)
    {
        auto i = JadeID_Index.find(key);
        return (i == JadeID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "JadeID", "TransferID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(JadeTransferTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByJadeID(pSrcRow->JadeID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                JadeID_Index[pNewRow->JadeID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.JadeID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TransferID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		JadeID_Index[row.JadeID] = Table.size()-1;


		return true;
	}

	~JadeTransferTable ()
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

		JadeID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif