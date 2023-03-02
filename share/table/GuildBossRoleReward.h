#ifndef __GuildBossRoleReward_H__
#define __GuildBossRoleReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/7/12 15:32:01


class GuildBossRoleReward : public CVSReader
{
public:

	struct RowData
	{
		uint	BossID;
		vector<Sequence<uint, 2>>	rank;
		vector<Sequence<uint, 2>>	reward;

	};

	std::unordered_map<uint,int> BossID_Index;


        RowData *GetByBossID(const uint &key)
        {
            auto i = BossID_Index.find(key);
            return (i == BossID_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BossID", "rank", "reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.BossID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		BossID_Index[row.BossID] = Table.size()-1;


		return true;
	}

	~GuildBossRoleReward ()
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