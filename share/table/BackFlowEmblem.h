#ifndef __BackFlowEmblem_H__
#define __BackFlowEmblem_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/19 20:07:40


class BackFlowEmblem : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ServerLevel;
		uint	Profession;
		vector<uint>	EmblemIDs;
		uint	SmeltRate;

		void CopyFrom(RowData *pRowData)
		{
			this->ServerLevel=pRowData->ServerLevel;
			this->Profession=pRowData->Profession;
			this->EmblemIDs=pRowData->EmblemIDs;
			this->SmeltRate=pRowData->SmeltRate;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ServerLevel", "Profession", "EmblemIDs", "SmeltRate",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BackFlowEmblem  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ServerLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EmblemIDs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SmeltRate)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~BackFlowEmblem ()
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