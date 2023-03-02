#ifndef __GUILDCARD_H__
#define __GUILDCARD_H__

#include "guild/guilddef.h"
#include "guild/guildlistener.h"

#define CARDTEAM_COUNT 4
#define CARDPOINT_TOPCOUNT 5

struct CardPoint
{
	UINT64 roleid;
	int    point;
	UINT32 timestamp;

	CardPoint()
	{
		roleid = 0;
		point = 0;
		timestamp = 0;
	}

	CardPoint(UINT64 _roleid)
	:roleid(_roleid)
	{
		point = 0;
		timestamp = 0;
	}

	CardPoint(UINT64 _roleid, int _point, UINT32 _timestamp)
	:roleid(_roleid)
	,point(_point)
	,timestamp(_timestamp)
	{
	}

	bool operator<(const CardPoint& cardpoint) const
	{
		if (point > cardpoint.point)
		{
			return true;
		}
		else if (point == cardpoint.point)
		{
			if (timestamp < cardpoint.timestamp)
			{
				return true;
			}
			else if (timestamp == cardpoint.timestamp)
			{
				return roleid < cardpoint.roleid;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
};

class CRole;
class Guild;
class GuildCardMatch;
namespace KKSG
{ 
	class GuildAllDBData;
}
class CGuildCard : public IGuildSerializerInterface, public IGuildListener
{
public:
	CGuildCard(Guild* guild);
	virtual ~CGuildCard();

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData& guilddata);
	virtual void Save(KKSG::GuildAllDBData& guilddata);
	virtual const std::string ToBinary();

	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember);
	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember) { };
	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwRoleID);
	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember) { };
	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember) { };

	void OnLogin(CRole* role);
	bool Update();

	inline UINT32 GetAllCount() { return m_allcount; }
	inline UINT32 GetResult() { return m_result; }

	std::string GetBestRoleName();

	bool IsCountLimit(UINT32 count);
	void AddCount();
	bool PushCards(UINT64 roleid, UINT32 result, std::vector<UINT32>& cards);
	const std::vector<UINT32>& GetCards();

	void UpdateRecordDay();

	void ArrangeRecord();

	void UpdateRecord(UINT64 roleid, int point);
	void UpdateTopRecord(UINT64 roleid, int point);
	std::set<CardPoint>* GetRecord(UINT64 roleid);
	void AddRecord(UINT64 roleid);
	void DelRecord(UINT64 roleid);
	const std::set<CardPoint>* GetTopRecords() { return &m_toprecords; }

	bool BeginMatch();
	void EndMatch();
	bool AddMatch(UINT64 roleid);
	bool DelMatch(UINT64 roleid);
	GuildCardMatch* GetMatch() { return m_match; }

	void UpdateMatchPoints(UINT64 roleid, UINT32 point);
	void DispatchMatchReward();
	void ClearMatchPoints() { m_matchpoints.clear(); }
	const std::set<CardPoint>* GetMatchPoints() { return &m_matchpoints; }

	void Test();
	KKSG::MulActivityTimeState  GetMatchStatus(); 
private:
	bool m_ischanged;
	Guild* m_guild;
	UINT32 m_updateday;
	UINT32 m_allcount;
	UINT32 m_result;
	UINT64 m_bestroleid;
	std::vector<UINT32> m_bestcards;

	GuildCardMatch* m_match;

	///> 比赛积分
	std::set<CardPoint> m_matchpoints;
	///> 工会积分
	std::set<CardPoint> m_toprecords;
	///> 小组积分
	UINT32 m_recordindex;
	std::map<UINT64, UINT32> m_roleid2team;
	std::map<UINT32, std::set<CardPoint>> m_id2teamrecords;
};

#endif