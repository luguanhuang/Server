#ifndef __GuildCamp_H__
#define __GuildCamp_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildCamp : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		string	Name;
		string	Pic;
		string	Description;
		string	Condition;
		int	Type;
		string	RankDes;
		long long	KillID;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Name=pRowData->Name;
			this->Pic=pRowData->Pic;
			this->Description=pRowData->Description;
			this->Condition=pRowData->Condition;
			this->Type=pRowData->Type;
			this->RankDes=pRowData->RankDes;
			this->KillID=pRowData->KillID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Name", "Pic", "Description", "Condition", "Type", "RankDes", "KillID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildCamp  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Pic)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Condition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.RankDes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.KillID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildCamp ()
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


	}

	std::vector<RowData*> Table;
};

#endif