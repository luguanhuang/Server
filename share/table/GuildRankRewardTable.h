#ifndef __GuildRankRewardTable_H__
#define __GuildRankRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildRankRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	Rank;
		vector<Sequence<uint, 2>>	LeaderReward;
		vector<Sequence<uint, 2>>	OfficerRreward;
		vector<Sequence<uint, 2>>	MemberReward;
		vector<Sequence<uint, 2>>	guildreward;

		void CopyFrom(RowData *pRowData)
		{
			this->Rank=pRowData->Rank;
			this->LeaderReward=pRowData->LeaderReward;
			this->OfficerRreward=pRowData->OfficerRreward;
			this->MemberReward=pRowData->MemberReward;
			this->guildreward=pRowData->guildreward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Rank", "LeaderReward", "OfficerRreward", "MemberReward", "guildreward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildRankRewardTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.LeaderReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.OfficerRreward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.MemberReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.guildreward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildRankRewardTable ()
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