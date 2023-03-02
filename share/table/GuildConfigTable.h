#ifndef __GuildConfigTable_H__
#define __GuildConfigTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildConfigTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	GuildLevel;
		uint	GuildExpNeed;
		int	GuildNumber;
		int	PokerTimes;
		int	PokerReplaceTimes;
		int	GuildSign;
		int	GuildWelfare;
		int	GuildStore;
		int	GuildSkill;
		int	GuildStage;
		uint	GuildPokerTimes;
		uint	AllGuildSign;
		uint	AllGuildStage;
		uint	GuildDragon;
		uint	GuildWar;
		int	GuildActivity;
		int	PresidentNum;
		int	VicePresidentNum;
		int	OfficialNum;
		int	EliteMemberNum;
		vector<Sequence<int, 2>>	GuildCheckInBonus;
		uint	StudySkillTimes;
		vector<Sequence<int, 2>>	GuildChallengeReward;
		int	GuildArena;
		vector<Sequence<uint, 3>>	GuildChallengeGExp;
		vector<Sequence<uint, 2>>	GuildChallengeSolo;
		int	GuildChallenge;
		int	GuildJokerMatch;
		int	GuildSalay;
		int	GuildMine;
		int	GuildTerritory;

		void CopyFrom(RowData *pRowData)
		{
			this->GuildLevel=pRowData->GuildLevel;
			this->GuildExpNeed=pRowData->GuildExpNeed;
			this->GuildNumber=pRowData->GuildNumber;
			this->PokerTimes=pRowData->PokerTimes;
			this->PokerReplaceTimes=pRowData->PokerReplaceTimes;
			this->GuildSign=pRowData->GuildSign;
			this->GuildWelfare=pRowData->GuildWelfare;
			this->GuildStore=pRowData->GuildStore;
			this->GuildSkill=pRowData->GuildSkill;
			this->GuildStage=pRowData->GuildStage;
			this->GuildPokerTimes=pRowData->GuildPokerTimes;
			this->AllGuildSign=pRowData->AllGuildSign;
			this->AllGuildStage=pRowData->AllGuildStage;
			this->GuildDragon=pRowData->GuildDragon;
			this->GuildWar=pRowData->GuildWar;
			this->GuildActivity=pRowData->GuildActivity;
			this->PresidentNum=pRowData->PresidentNum;
			this->VicePresidentNum=pRowData->VicePresidentNum;
			this->OfficialNum=pRowData->OfficialNum;
			this->EliteMemberNum=pRowData->EliteMemberNum;
			this->GuildCheckInBonus=pRowData->GuildCheckInBonus;
			this->StudySkillTimes=pRowData->StudySkillTimes;
			this->GuildChallengeReward=pRowData->GuildChallengeReward;
			this->GuildArena=pRowData->GuildArena;
			this->GuildChallengeGExp=pRowData->GuildChallengeGExp;
			this->GuildChallengeSolo=pRowData->GuildChallengeSolo;
			this->GuildChallenge=pRowData->GuildChallenge;
			this->GuildJokerMatch=pRowData->GuildJokerMatch;
			this->GuildSalay=pRowData->GuildSalay;
			this->GuildMine=pRowData->GuildMine;
			this->GuildTerritory=pRowData->GuildTerritory;

		}
	};

	std::unordered_map<int,int> GuildLevel_Index;


    RowData *GetByGuildLevel(const int &key)
    {
        auto i = GuildLevel_Index.find(key);
        return (i == GuildLevel_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GuildLevel", "GuildExpNeed", "GuildNumber", "PokerTimes", "PokerReplaceTimes", "GuildSign", "GuildWelfare", "GuildStore", "GuildSkill", "GuildStage", "GuildPokerTimes", "AllGuildSign", "AllGuildStage", "GuildDragon", "GuildWar", "GuildActivity", "PresidentNum", "VicePresidentNum", "OfficialNum", "EliteMemberNum", "GuildCheckInBonus", "StudySkillTimes", "GuildChallengeReward", "GuildArena", "GuildChallengeGExp", "GuildChallengeSolo", "GuildChallenge", "GuildJokerMatch", "GuildSalay", "GuildMine", "GuildTerritory",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildConfigTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByGuildLevel(pSrcRow->GuildLevel);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                GuildLevel_Index[pNewRow->GuildLevel]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.GuildLevel)) 
		{return false;}++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.GuildExpNeed)) 
			{return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.GuildNumber)) 
			{return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PokerTimes)) 
			{return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PokerReplaceTimes)) 
			{return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.GuildSign)) 
			{return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.GuildWelfare)) 
			{return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.GuildStore)) 
			{return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.GuildSkill)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.GuildStage)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.GuildPokerTimes)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.AllGuildSign)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.AllGuildStage)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.GuildDragon)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.GuildWar)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.GuildActivity)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.PresidentNum)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.VicePresidentNum)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.OfficialNum)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.EliteMemberNum)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.GuildCheckInBonus)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.StudySkillTimes)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.GuildChallengeReward)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.GuildArena)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.GuildChallengeGExp)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.GuildChallengeSolo)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.GuildChallenge)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.GuildJokerMatch)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.GuildSalay)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.GuildMine)) {return false;} ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.GuildTerritory)) {return false;} ++m_ColNum;

		
		Table.push_back(pRow);

		GuildLevel_Index[row.GuildLevel] = Table.size()-1;


		return true;
	}

	~GuildConfigTable ()
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

		GuildLevel_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif