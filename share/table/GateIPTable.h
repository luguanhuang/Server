#ifndef __GateIPTable_H__
#define __GateIPTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GateIPTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ServerID;
		string	ServerName;
		string	ZoneName;
		int	State;
		vector<string>	IPAddr;
		int	StateTxt;
		vector<uint>	Channel;

		void CopyFrom(RowData *pRowData)
		{
			this->ServerID=pRowData->ServerID;
			this->ServerName=pRowData->ServerName;
			this->ZoneName=pRowData->ZoneName;
			this->State=pRowData->State;
			this->IPAddr=pRowData->IPAddr;
			this->StateTxt=pRowData->StateTxt;
			this->Channel=pRowData->Channel;

		}
	};

	std::unordered_map<int,int> ServerID_Index;


    RowData *GetByServerID(const int &key)
    {
        auto i = ServerID_Index.find(key);
        return (i == ServerID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ServerID", "ServerName", "ZoneName", "State", "IPAddr", "StateTxt", "Channel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GateIPTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByServerID(pSrcRow->ServerID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ServerID_Index[pNewRow->ServerID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ServerID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ServerName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ZoneName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.State)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.IPAddr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.StateTxt)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Channel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ServerID_Index[row.ServerID] = Table.size()-1;


		return true;
	}

	~GateIPTable ()
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

		ServerID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif