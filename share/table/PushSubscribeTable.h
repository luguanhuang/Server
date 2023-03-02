#ifndef __PushSubscribeTable_H__
#define __PushSubscribeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PushSubscribeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	MsgId;
		string	Name;
		vector<uint>	Time;
		vector<uint>	WeekDay;
		bool	IsShow;
		string	SubscribeDescription;
		string	CancelDescription;
		uint	TxMsgId;

		void CopyFrom(RowData *pRowData)
		{
			this->MsgId=pRowData->MsgId;
			this->Name=pRowData->Name;
			this->Time=pRowData->Time;
			this->WeekDay=pRowData->WeekDay;
			this->IsShow=pRowData->IsShow;
			this->SubscribeDescription=pRowData->SubscribeDescription;
			this->CancelDescription=pRowData->CancelDescription;
			this->TxMsgId=pRowData->TxMsgId;

		}
	};

	std::unordered_map<uint,int> MsgId_Index;


    RowData *GetByMsgId(const uint &key)
    {
        auto i = MsgId_Index.find(key);
        return (i == MsgId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "MsgId", "Name", "Time", "WeekDay", "IsShow", "SubscribeDescription", "CancelDescription", "TxMsgId",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PushSubscribeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByMsgId(pSrcRow->MsgId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                MsgId_Index[pNewRow->MsgId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.MsgId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.WeekDay)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.IsShow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SubscribeDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.CancelDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.TxMsgId)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		MsgId_Index[row.MsgId] = Table.size()-1;


		return true;
	}

	~PushSubscribeTable ()
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

		MsgId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif