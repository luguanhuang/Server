#ifndef __GuildBossConfig_H__
#define __GuildBossConfig_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/7/18 11:44:03


class GuildBossConfig : public CVSReader
{
public:

	struct RowData
	{
		uint	GuildLevel;
		uint	BossID;
		uint	MonsterID;
		vector<Sequence<uint, 2>>	FirsttKillReward;
		vector<Sequence<uint, 2>>	JoinReward;
		vector<Sequence<uint, 2>>	KillReward;
		string	WinCutScene;

	};

	std::unordered_map<string,int> WinCutScene_Index;


        RowData *GetByWinCutScene(const string &key)
        {
            auto i = WinCutScene_Index.find(key);
            return (i == WinCutScene_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GuildLevel", "BossID", "MonsterID", "FirsttKillReward", "JoinReward", "KillReward", "WinCutScene",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.GuildLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.BossID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.MonsterID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.FirsttKillReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.JoinReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.KillReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.WinCutScene)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		WinCutScene_Index[row.WinCutScene] = Table.size()-1;


		return true;
	}

	~GuildBossConfig ()
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